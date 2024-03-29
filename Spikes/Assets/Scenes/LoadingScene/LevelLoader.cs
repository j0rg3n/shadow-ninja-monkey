using UnityEngine;
using System.Collections;

public class LevelLoader : MonoBehaviour {

	public Camera mainCamera;
	public GameObject level002;
	public GameObject tutorialLevel;
	public GameObject ninjaCharacter;
	private NinjaBehaviour localNinja = null;
	
	private NinjaLevel level = null;

	public NinjaLevel CurrentLevel
	{
		get {return level;}
	}
	
	public NinjaLevel LoadTutorialLevel(bool instantiateOnNetwork)
	{
		return LoadSomeLevel(tutorialLevel, instantiateOnNetwork);
	}
	
	public NinjaLevel LoadLevel002(bool instantiateOnNetwork)
	{
		return LoadSomeLevel(level002, instantiateOnNetwork);
	}
	
	private NinjaLevel LoadSomeLevel(GameObject originalLevel, bool instantiateOnNetwork)
	{
		if (instantiateOnNetwork)
		{
			if (Network.peerType == NetworkPeerType.Server)
			{
				level = FlexiInstantiate<NinjaLevel>(originalLevel, new Vector3(0,0,0), Quaternion.identity, instantiateOnNetwork);
				mainCamera.transform.position = level.cameraPosition;
			}
		}
		else
		{
			level = FlexiInstantiate<NinjaLevel>(originalLevel, new Vector3(0,0,0), Quaternion.identity, instantiateOnNetwork);
			mainCamera.transform.position = level.cameraPosition;

			localNinja = CreateNinja(NinjaBehaviour.NinjaColor.White, level.whiteNinjaPosition, instantiateOnNetwork);
			CreateNinja(NinjaBehaviour.NinjaColor.Black, level.blackNinjaPosition, instantiateOnNetwork);
		}
		
		//Set global game state to fighting
		GetComponent<GlobalGameState>().SetFightingState();
		
		((GameHUD)FindObjectOfType(typeof(GameHUD))).showMe = true;
		
		return level;
	}
	
	[RPC]
	public void CreateYourBlackClientNinja(Vector3 blackNinjaPosition)
	{
		Debug.Log("WE ARE CREATING A BLACK CLIENT NINJA....");
		const bool instantiateOnNetwork = true;
		localNinja = CreateNinja(NinjaBehaviour.NinjaColor.Black, blackNinjaPosition, instantiateOnNetwork);
	}
	
	public void OnPlayerConnected(NetworkPlayer player)
	{
		if(localNinja!=null)
		{
			//Ninjas already instantiated:
			ServerRespawnGame();
			return;
		}
		const bool instantiateOnNetwork = true;
		localNinja = CreateNinja(NinjaBehaviour.NinjaColor.White, level.whiteNinjaPosition, instantiateOnNetwork);
		networkView.RPC("CreateYourBlackClientNinja", RPCMode.Others, level.blackNinjaPosition);
		
	}

	private NinjaBehaviour CreateNinja(NinjaBehaviour.NinjaColor color, Vector3 position, bool instantiateOnNetwork)
	{		
		Debug.Log("Creating " + color + " local");
		NinjaBehaviour ninja = FlexiInstantiate<NinjaBehaviour>(ninjaCharacter, position, Quaternion.identity, instantiateOnNetwork);
		ninja.Initialize(color, instantiateOnNetwork);
		GetComponent<GlobalGameState>().SetFightingState();
		return ninja;
	}
	/*
	public void Update()
	{
		if(Input.GetKey("t"))
		{
			ServerRespawnGame();
		}
	}
	*/
	public void ServerRespawnGame()
	{
		Debug.Log("Server respawn");
		localNinja.gameObject.transform.position = level.whiteNinjaPosition;
		localNinja.ResetHealth();
		networkView.RPC("PositionYourBlackNinja", RPCMode.Others, level.blackNinjaPosition);
	}
	
	[RPC]
	public void PositionYourBlackNinja(Vector3 blackNinjaPosition)
	{
		Debug.Log("Client respawn");
		Debug.Log("WE ARE Positioning A BLACK CLIENT NINJA....");
		localNinja.ResetHealth();
		localNinja.gameObject.transform.position = blackNinjaPosition;
	}
	
	
	private T FlexiInstantiate<T>(Object original, Vector3 position, Quaternion rotation, bool instantiateOnNetwork) where T : Component
	{
		GameObject instance;
		if (instantiateOnNetwork)
		{
			instance = (GameObject) Network.Instantiate(original, position, rotation, 0);
		}
		else
		{
			instance = (GameObject) Instantiate(original, position, rotation);
		}	
		return instance.GetComponent<T>();
	}
}
