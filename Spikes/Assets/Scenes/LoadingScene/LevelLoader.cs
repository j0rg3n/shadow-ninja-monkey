using UnityEngine;
using System.Collections;

public class LevelLoader : MonoBehaviour {

	public Camera mainCamera;
	public GameObject level002;
	
	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
	
	}
	
	public NinjaLevel LoadLevel002()
	{
		NinjaLevel level= ((GameObject )Instantiate(level002, new Vector3(0,0,0), Quaternion.identity)).GetComponent<NinjaLevel>();
		mainCamera.transform.position = level.cameraPosition;
		return level;
	}
	
	
}
