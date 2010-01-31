using UnityEngine;
using System.Collections;

public class ColliderSceneStartup : MonoBehaviour {

	public GameObject templateNinja;
	
	// Use this for initialization
	void Start () {
		Instantiate(templateNinja, new Vector3(2,5,0), Quaternion.identity);
		Instantiate(templateNinja, new Vector3(-5,5,0), Quaternion.identity);		
	}
	
	// Update is called once per frame
	void Update () {
	
	}
}
