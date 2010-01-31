using UnityEngine;
using System.Collections;

public class LevelLoader : MonoBehaviour {

	public Camera mainCamera;
	public GameObject level002;
	public GameObject ninjaCharacter;

	public NinjaLevel LoadLevel002(bool instantiateOnNetwork)
	{
		NinjaLevel level= FlexiInstantiate<NinjaLevel>(level002, new Vector3(0,0,0), Quaternion.identity, instantiateOnNetwork);
		mainCamera.transform.position = level.cameraPosition;
		
		NinjaBehaviour blackNinja = FlexiInstantiate<NinjaBehaviour>(ninjaCharacter, level.blackNinjaPosition, Quaternion.identity, instantiateOnNetwork);
		blackNinja.Initialize(NinjaBehaviour.NinjaColor.Black, instantiateOnNetwork);
		
		NinjaBehaviour whiteNinja = FlexiInstantiate<NinjaBehaviour>(ninjaCharacter, level.whiteNinjaPosition, Quaternion.identity, instantiateOnNetwork);
		whiteNinja.Initialize(NinjaBehaviour.NinjaColor.White, instantiateOnNetwork);
		
		return level;
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
