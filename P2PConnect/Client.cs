using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Sockets;
using System.Threading;
using System.Net;

namespace P2PConnect
{
    internal sealed class Client : IDisposable
    {
        public Client(string id, string sessionID)
        {
            this.id = id;
            this.sessionID = sessionID;

            socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            socket.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.ReuseAddress, true);
        }

        public event Action<string, string> MessageLogged;

        public void Start()
        {
            senderThread = new Thread(Send);
            senderThread.Name = id + " sender.";
            senderThread.Start();

            listenThread = new Thread(Listen);
            listenThread.Name = id + " listener.";
            listenThread.Start();
        }

        public void Dispose()
        {
            if (socket != null)
            {
                socket.Dispose();
                socket = null;
            }

            if (senderThread != null)
            {
                senderThread.Join();
                senderThread = null;
            }

            if (listenThread != null)
            {
                listenThread.Join();
                listenThread = null;
            }
        }

        private void Send()
        {
            Log("Client {0} up, connecting...", id);
            socket.Connect("fabeljet.com", 80);

            Log("Connected.", id);
            connectedEvent.Set();

            string privateEndPoint = "unk";

            StringBuilder buf = new StringBuilder();
            buf.Append("GET ");
            buf.Append("/index.php");
            buf.Append("?ClientID=" + id);
            buf.Append("&SessionID=" + sessionID);
            buf.Append("&PrivateEndPoint=" + privateEndPoint);
            buf.AppendLine(" HTTP/1.1");

            // TODO: Add additional headers here.

            buf.AppendLine("Host: punch.fabeljet.com");

            buf.AppendLine();

            Log("Sending HTTP request.", id);
            SendString(socket, buf.ToString());
        }

        private void SendString(Socket socket, string text)
        {
            byte[] data = Encoding.ASCII.GetBytes(text);

            int sendPos = 0;
            while (sendPos < data.Length)
            {
                int sendCount = socket.Send(data, sendPos, data.Length - sendPos, SocketFlags.None);
                sendPos += sendCount;
            }
        }

        private void Listen()
        {
            connectedEvent.WaitOne();

            byte[] data = new byte[1024];

            Log("Receiving...", id);
            while (true)
            {
                int readCount = socket.Receive(data);
                if (readCount == 0)
                {
                    return;
                }

                string text = Encoding.ASCII.GetString(data, 0, readCount);
                Log("Received '{0}'", text);
            }
            /*
            socket.Bind(new IPEndPoint(IPAddress.Loopback, 5000));
            Log("Listening...");
            socket.Listen(4);
            Log("Accepting...");
            Socket incomingSocket = socket.Accept();
            Log("Socket accepted.");
             */
        }

        private void Log(string format, params object[] args)
        {
            string message = string.Format(format, args);
            if (MessageLogged != null)
            {
                MessageLogged(id, message);
            }
        }

        private ManualResetEvent connectedEvent = new ManualResetEvent(false);
        private readonly string id;
        private readonly string sessionID;
        private Socket socket;

        private Thread senderThread;
        private Thread listenThread;
    }
}
