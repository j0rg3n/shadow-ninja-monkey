using UnityEngine;
using System.Collections;

public class LevelLoader : MonoBehaviour {

	public Camera mainCamera;
	public GameObject level002;

	public NinjaLevel LoadLevel002(bool instantiateOnNetwork)
	{
		NinjaLevel level= ((GameObject)FlexiInstantiate(level002, new Vector3(0,0,0), Quaternion.identity, instantiateOnNetwork)).GetComponent<NinjaLevel>();
		mainCamera.transform.position = level.cameraPosition;
		return level;
	}
	
	private object FlexiInstantiate(Object original, Vector3 position, Quaternion rotation, bool instantiateOnNetwork)
	{
		if (instantiateOnNetwork)
		{
			return Network.Instantiate(original, position, rotation, 0);
		}
		else
		{
			return Instantiate(original, position, rotation);
		}	
	}
}
