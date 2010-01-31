using UnityEngine;
using System.Collections;

public class ConnectGui : MonoBehaviour 
{
	//DontDestroyOnLoad(this);
	public string remoteIP = "192.168.49.80";
	public int remotePort = 25000;
	public int listenPort = 25000;
	public bool useNAT = false;

	void Awake() 
	{
		//if (FindObjectOfType<ConnectGuiMasterServer>())
		//{
		//	this.enabled = false;
		//}
	}

	void OnGUI ()
	{
		GUILayout.Space(10);

		GUILayout.BeginHorizontal();
		GUILayout.Space(10);
		
		if (Network.peerType == NetworkPeerType.Disconnected)
		{
			GUILayout.BeginVertical();
			if (GUILayout.Button ("Connect"))
			{
				Network.useNat = useNAT;
				Network.Connect(remoteIP, remotePort);
			}
			if (GUILayout.Button ("Start Server"))
			{
				Network.useNat = useNAT;
				Network.InitializeServer(32, listenPort);
				
				// Notify our objects that the level and the network is ready
				foreach (GameObject go in FindObjectsOfType(typeof(GameObject)))
				{
					go.SendMessage("OnNetworkLoadedLevel", SendMessageOptions.DontRequireReceiver);		
				}
			}
			GUILayout.EndVertical();
			remoteIP = GUILayout.TextField(remoteIP, GUILayout.MinWidth(100));
			remotePort = int.Parse(GUILayout.TextField(remotePort.ToString()));
		}
		else
		{
			if (GUILayout.Button ("Disconnect"))
			{
				Network.Disconnect(200);
			}
		}
		
		GUILayout.FlexibleSpace();
		GUILayout.EndHorizontal();
	}

	void OnConnectedToServer() {
		// Notify our objects that the level and the network is ready
		foreach (GameObject go in FindObjectsOfType(typeof(GameObject)))
		{
			go.SendMessage("OnNetworkLoadedLevel", SendMessageOptions.DontRequireReceiver);		
		}
	}

	void OnDisconnectedFromServer () {
		if (this.enabled != false)
			Application.LoadLevel(Application.loadedLevel);
		else
			((NetworkLevelLoad) FindObjectOfType(typeof(NetworkLevelLoad))).OnDisconnectedFromServer();
	}
}