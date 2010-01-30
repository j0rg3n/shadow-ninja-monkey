using UnityEngine;
using System.Collections;

public class RotateUsingInputAxes : MonoBehaviour {

	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
		transform.Translate(Input.GetAxis("Horizontal"), Input.GetAxis("Vertical"), 0);
	}
}
