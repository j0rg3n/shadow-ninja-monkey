using UnityEngine;
using System.Collections;

public class LevelLoader : MonoBehaviour {

	public Camera mainCamera;
	public GameObject level002;
	public GameObject ninjaCharacter;
	
	private NinjaLevel level = null;

	public NinjaLevel LoadLevel002(bool instantiateOnNetwork)
	{
		if (instantiateOnNetwork)
		{
			if (Network.peerType == NetworkPeerType.Server)
			{
				level = FlexiInstantiate<NinjaLevel>(level002, new Vector3(0,0,0), Quaternion.identity, instantiateOnNetwork);
				mainCamera.transform.position = level.cameraPosition;
			}
		}
		else
		{
			level = FlexiInstantiate<NinjaLevel>(level002, new Vector3(0,0,0), Quaternion.identity, instantiateOnNetwork);
			mainCamera.transform.position = level.cameraPosition;

			CreateNinja(NinjaBehaviour.NinjaColor.White, level.whiteNinjaPosition, instantiateOnNetwork);
			CreateNinja(NinjaBehaviour.NinjaColor.Black, level.blackNinjaPosition, instantiateOnNetwork);
		}
		
		return level;
	}
	
	[RPC]
	public void CreateYourBlackClientNinja(Vector3 blackNinjaPosition)
	{
		Debug.Log("WE ARE CREATING A BLACK CLIENT NINJA....");
		const bool instantiateOnNetwork = true;
		CreateNinja(NinjaBehaviour.NinjaColor.Black, blackNinjaPosition, instantiateOnNetwork);
	}
	
	public void OnPlayerConnected(NetworkPlayer player)
	{
		const bool instantiateOnNetwork = true;
		CreateNinja(NinjaBehaviour.NinjaColor.White, level.whiteNinjaPosition, instantiateOnNetwork);
		networkView.RPC("CreateYourBlackClientNinja", RPCMode.Others, level.blackNinjaPosition);
	}

	private void CreateNinja(NinjaBehaviour.NinjaColor color, Vector3 position, bool instantiateOnNetwork)
	{
		Debug.Log("Creating " + color + " local");
		NinjaBehaviour ninja = FlexiInstantiate<NinjaBehaviour>(ninjaCharacter, position, Quaternion.identity, instantiateOnNetwork);
		ninja.Initialize(color, instantiateOnNetwork);
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