using UnityEngine;
using System.Collections;

public class PokerCollider : MonoBehaviour {

	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
	
	}
	
	void OnTriggerEnter(Collider other)
	{
		if(other.tag == "Sword")
			Debug.Log(this.name + " was Poked !");
	}
}
