using UnityEngine;
using System.Collections;

public class ColliderSceneStartup : MonoBehaviour {

	public GameObject templateNinja;
	
	// Use this for initialization
	void Start () {
		GameObject whiteNinja = (GameObject )Instantiate(templateNinja, new Vector3(2,5,0), Quaternion.identity);
		GameObject blackNinja = (GameObject )Instantiate(templateNinja, new Vector3(-5,5,0), Quaternion.identity);
		whiteNinja.GetComponent<NinjaBehaviour>().Initialize(NinjaBehaviour.NinjaColor.White,false);
		blackNinja.GetComponent<NinjaBehaviour>().Initialize(NinjaBehaviour.NinjaColor.Black,false);
		
	}
	
	// Update is called once per frame
	void Update () {
	
	}
}
