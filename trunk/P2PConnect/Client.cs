using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Sockets;
using System.Threading;
using System.Net;
using System.Diagnostics;

namespace P2PConnect
{
    internal enum EndPointLocation
    {
        Public,
        Private
    };

    internal sealed class Client : IDisposable
    {
        public Client(string id, string sessionID)
        {
            this.id = id;
            this.sessionID = sessionID;
        }

        public event Action<string, string> MessageLogged;

        public void Start()
        {
            mediationThread = new Thread(Mediate);
            mediationThread.Name = id + " mediator.";
            mediationThread.Start();
        }

        public void Dispose()
        {
            lock (mediateSocketMutex)
            {
                DisposeIfNotNull(mediateSocket);
            }

            DisposeIfNotNull(listenSocket);
            DisposeIfNotNull(incomingSocket);
            DisposeIfNotNull(outgoingSockets[EndPointLocation.Private]);
            DisposeIfNotNull(outgoingSockets[EndPointLocation.Public]);

            JoinAndNull(ref mediationThread);
            JoinAndNull(ref connectToPrivateEndPointThread);
            JoinAndNull(ref connectToPublicEndPointThread);
            JoinAndNull(ref listenThread);
        }

        private static void JoinAndNull(ref Thread thread)
        {
            if (thread != null)
            {
                thread.Join();
                thread = null;
            }
        }

        private static void DisposeIfNotNull(IDisposable disposable)
        {
            if (disposable != null)
            {
                disposable.Dispose();
            }
        }

        private void Mediate()
        {
            try
            {
                Log("Client {0} up, connecting...", id);
                mediateSocket = CreateAddressReuseSocket();
                mediateSocket.Connect("fabeljet.com", 80);
                Log("Connected.", id);

                Log("Sending mediation request.");
                SendMediationRequest();

                Log("Receiving from mediator...");
                string mediatorResponse = ReceiveMediationData();

                Log("Mediator response received, parsing endpoints.");
                IPEndPoint otherPrivateEndPoint;
                IPEndPoint otherPublicEndPoint;
                GetEndPoints(mediatorResponse, out otherPrivateEndPoint, out otherPublicEndPoint);

                Log("Other client private endpoint: " + otherPrivateEndPoint);
                Log("Other client public endpoint: " + otherPublicEndPoint);

                endPoints[EndPointLocation.Private] = otherPrivateEndPoint;
                outgoingSockets[EndPointLocation.Private] = null;
                endPoints[EndPointLocation.Public] = otherPublicEndPoint;
                outgoingSockets[EndPointLocation.Public] = null;

                connectToPrivateEndPointThread = CreateAndStartConnectThread(EndPointLocation.Private);
                connectToPublicEndPointThread = CreateAndStartConnectThread(EndPointLocation.Public);

                listenThread = CreateAndStartListenThread();

                authenticatedEvent.WaitOne();

                lock (authenticationMutex)
                {
                    foreach (KeyValuePair<string, Socket> socket in AllSockets)
                    {
                        if (socket.Value == authenticatedSocket)
                        {
                            Log("{0} socket authenticated.", socket.Key);
                        }
                        else
                        {
                            Log("{0} socket not authenticated, closing.", socket.Key);
                            if (socket.Value != null)
                            {
                                socket.Value.Close();
                            }
                        }
                    }
                }
            }
            finally
            {
                lock (mediateSocketMutex)
                {
                    Log("Closing mediator connection...");
                    mediateSocket.Close();
                    mediateSocket = null;

                    Log("Mediator connection closed.");
                }
            }
            Log("Mediator thread exiting.");
        }

        private void SendMediationRequest()
        {
            string privateEndPoint = mediateSocket.LocalEndPoint.ToString();

            StringBuilder buf = new StringBuilder();
            buf.Append("GET ");
            buf.Append("/index.php");
            buf.Append("?ClientID=" + id);
            buf.Append("&SessionID=" + sessionID);
            buf.Append("&PrivateEndPoint=" + privateEndPoint);
            buf.AppendLine(" HTTP/1.1");
            buf.AppendLine("Host: punch.fabeljet.com");
            buf.AppendLine();

            SendString(mediateSocket, buf.ToString());
        }

        private string ReceiveMediationData()
        {
            StringBuilder buf = new StringBuilder();
            byte[] data = new byte[1024];
            while (true)
            {
                int readCount = mediateSocket.Receive(data);
                if (readCount == 0)
                {
                    break;
                }

                string text = Encoding.ASCII.GetString(data, 0, readCount);
                buf.Append(text);
                Log("Received '{0}'", text);
            }
            return buf.ToString();
        }

        private Thread CreateAndStartListenThread()
        {
            Thread thread = new Thread(Listen)
            {
                Name = id + " listener."
            };
            thread.Start();
            return thread;
        }

        private void Listen()
        {
            Log("Listening for incoming...");
            try
            {
                EndPoint localEndPoint = GetLocalEndPoint();
                if (localEndPoint == null)
                {
                    return;
                }

                listenSocket = CreateAddressReuseSocket();
                listenSocket.Bind(localEndPoint);
                listenSocket.Listen(1);

                incomingSocket = listenSocket.Accept();
                Authenticate(incomingSocket);
            }
            catch (SocketException e)
            {
                Log("Listen failed: {0}", e.Message);
            }
            catch (ObjectDisposedException e)
            {
                Log("Listen socket disposed: {0}", e.Message);
            }
            finally
            {
                if (incomingSocket != null)
                {
                    incomingSocket.Close();
                }

                if (listenSocket != null)
                {
                    listenSocket.Close();
                }
            }

            Log("Listen thread exiting.");
        }

        private Thread CreateAndStartConnectThread(EndPointLocation location)
        {
            Thread thread = new Thread(ConnectToEndPoint)
            {
                Name = string.Format("{0} connect to {1}.", id, location)
            };
            thread.Start(location);
            return thread;
        }

        private void ConnectToEndPoint(object param)
        {
            EndPointLocation location = (EndPointLocation)param;
            IPEndPoint endPoint = endPoints[location];

            try
            {
                EndPoint localEndPoint = GetLocalEndPoint();
                if (localEndPoint == null)
                {
                    return;
                }

                outgoingSockets[location] = CreateAddressReuseSocket();
                outgoingSockets[location].Bind(localEndPoint);

                Log("Connecting to {0} ({1})...", location, endPoint);
                outgoingSockets[location].Connect(endPoint);
                Authenticate(outgoingSockets[location]);
            }
            catch (SocketException e)
            {
                Log("Connect to {0} failed: {1}", location, e.Message);
            }
            catch (ObjectDisposedException e)
            {
                Log("Outgoing to {0} socket disposed: {1}", location, e.Message);
            }
            finally
            {
                if (outgoingSockets[location] != null)
                {
                    outgoingSockets[location].Close();
                }
            }

            Log("Connect to {0} thread exiting.", location);
        }

        private EndPoint GetLocalEndPoint()
        {
            lock (mediateSocketMutex)
            lock (authenticationMutex)
            {
                if (authenticatedSocket != null)
                {
                    return null;
                }

                Debug.Assert(mediateSocket != null);

                return mediateSocket.LocalEndPoint;
            }
        }

        private static Socket CreateAddressReuseSocket()
        {
            Socket socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            socket.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.ReuseAddress, true);
            return socket;
        }

        private void Log(string format, params object[] args)
        {
            string message = string.Format(format, args);
            if (MessageLogged != null)
            {
                MessageLogged(id, message);
            }
        }

        private static void SendString(Socket socket, string text)
        {
            byte[] data = Encoding.ASCII.GetBytes(text);

            int sendPos = 0;
            while (sendPos < data.Length)
            {
                int sendCount = socket.Send(data, sendPos, data.Length - sendPos, SocketFlags.None);
                sendPos += sendCount;
            }
        }

        private static void GetEndPoints(string response, out IPEndPoint otherPrivateEndPoint, out IPEndPoint otherPublicEndPoint)
        {
            string[] responseParts = response.ToString().Split(new[] { "\r\n\r\n" }, StringSplitOptions.None);
            string[] bodyLines = responseParts[1].Split('\n');
            string[] lastEndPointInfoParts = bodyLines[bodyLines.Length - 1].Split(' ');

            string otherClientID = lastEndPointInfoParts[0];
            string sessionID = lastEndPointInfoParts[1];
            string otherPrivateEndPointString = lastEndPointInfoParts[2];
            string otherPublicEndPointString = lastEndPointInfoParts[3];

            otherPrivateEndPoint = ParseEndPoint(otherPrivateEndPointString);
            otherPublicEndPoint = ParseEndPoint(otherPublicEndPointString);
        }

        private static IPEndPoint ParseEndPoint(string endPointString)
        {
            string[] parts = endPointString.Split(':');
            return new IPEndPoint(IPAddress.Parse(parts[0]), int.Parse(parts[1]));
        }

        private IEnumerable<KeyValuePair<string, Socket>> AllSockets
        {
            get
            {
                yield return new KeyValuePair<string, Socket>("Incoming", 
                    listenSocket);
                yield return new KeyValuePair<string, Socket>("Outgoing to public", 
                    outgoingSockets[EndPointLocation.Public]);
                yield return new KeyValuePair<string, Socket>("Outgoing to private", 
                    outgoingSockets[EndPointLocation.Private]);
            }
        }

        private void Authenticate(Socket socket)
        {
            lock (authenticationMutex)
            {
                authenticatedSocket = socket;
                authenticatedEvent.Set();
            }

            // Send and receive some data to make it real.

            string name = "INCOGNITO";
            foreach (var item in AllSockets)
            {
                if (item.Value == socket)
                {
                    name = item.Key;
                }
            }

            Log("Connected to {0}, sending message...", name);
            SendString(socket, string.Format("Hi, I'm Mr. {0}. DONE.", id));

            Log("Done sending, trying to read from {0}...", name);
            StringBuilder buf = new StringBuilder();
            byte[] data = new byte[1024];
            while (true)
            {
                int readCount = socket.Receive(data);
                if (readCount == 0)
                {
                    break;
                }

                string text = Encoding.ASCII.GetString(data, 0, readCount);
                buf.Append(text);
                Log("Received from {0}: '{1}'", name, text);

                if (buf.ToString().EndsWith("DONE."))
                {
                    break;
                }
            }

            Log("Done reading from {0}.", name);
        }

        private readonly string id;
        private readonly string sessionID;

        private readonly Dictionary<EndPointLocation, IPEndPoint> endPoints = new Dictionary<EndPointLocation, IPEndPoint>();
        
        private ManualResetEvent authenticatedEvent = new ManualResetEvent(false);
        private readonly object authenticationMutex = new object();
        private Socket authenticatedSocket;

        private readonly object mediateSocketMutex = new object();
        private Socket mediateSocket;
        private Socket listenSocket;
        private Socket incomingSocket;
        private readonly Dictionary<EndPointLocation, Socket> outgoingSockets = new Dictionary<EndPointLocation, Socket>();

        private Thread mediationThread;
        private Thread connectToPrivateEndPointThread;
        private Thread connectToPublicEndPointThread;
        private Thread listenThread;

    }
}
