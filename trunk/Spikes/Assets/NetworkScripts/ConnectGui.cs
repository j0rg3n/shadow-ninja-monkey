using UnityEngine;
using System.Collections;

public class ConnectGui : MonoBehaviour 
{
	//DontDestroyOnLoad(this);
	public string remoteIP = "192.168.49.80";
	public int remotePort = 25000;
	public int listenPort = 25000;
	public bool useNAT = false;

	public Font menuFont;
	public Font textfieldFont;
	public Texture2D blackBG;
	
	void Awake() 
	{
		//if (FindObjectOfType<ConnectGuiMasterServer>())
		//{
		//	this.enabled = false;
		//}
	}

	void OnGUI ()
	{		
		GUIStyle menuStyle = new GUIStyle();
		menuStyle.font = menuFont;
		menuStyle.normal.textColor = new Color(1,1,1,1);
		menuStyle.alignment = TextAnchor.LowerLeft;	
		
		GUIStyle textFieldStyle = new GUIStyle();
		textFieldStyle.font = textfieldFont;
		textFieldStyle.normal.textColor = new Color(1,1,1,1);
		textFieldStyle.normal.background = blackBG;
		
			
		if (Network.peerType == NetworkPeerType.Disconnected)
		{
			int titleWidth = Screen.height*2;
			GUI.BeginGroup (new Rect ((Screen.width/2) - (Screen.height/2), 0, titleWidth, Screen.height));
				
			if (GUI.Button(new Rect (15,(Screen.height-225),150,60), "Start", menuStyle))
			{		
				Network.useNat = useNAT;
				Network.InitializeServer(32, listenPort);
				
				// Notify our objects that the level and the network is ready
				foreach (GameObject go in FindObjectsOfType(typeof(GameObject)))
				{
					go.SendMessage("OnNetworkLoadedLevel", SendMessageOptions.DontRequireReceiver);		
				}
			}
	
			if(GUI.Button(new Rect (15,(Screen.height-145),105,60), "Join", menuStyle))
			{
				//Debug.Log("Join FIGHT!");
				Network.useNat = useNAT;
				Network.Connect(remoteIP, remotePort);
			}
	
			
			textFieldStyle.alignment = TextAnchor.MiddleLeft;
			textFieldStyle.padding.left = 10;
			remoteIP = GUI.TextField(new Rect (130,(Screen.height-145),230,52), remoteIP, textFieldStyle);			
			remotePort = int.Parse(GUI.TextField(new Rect (370,(Screen.height-145),100,52), remotePort.ToString(), textFieldStyle));

			GUI.EndGroup ();
		}
		else
		{
			textFieldStyle.alignment = TextAnchor.LowerLeft;
			//textFieldStyle.normal.background = null;
			if (GUI.Button (new Rect (0, Screen.height-30,180,30), "Disconnect", textFieldStyle))
			{
				Network.Disconnect(200);
			}
		}
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