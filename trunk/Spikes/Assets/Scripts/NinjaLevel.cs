using UnityEngine;
using System.Collections;

public class NinjaLevel : MonoBehaviour {

	public Vector3 cameraPosition;
	public Vector3 whiteNinjaPosition;
	public Vector3 blackNinjaPosition;
	
	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
	
	}
	
	public void Dispose() {
		Destroy(gameObject);
	}
}
