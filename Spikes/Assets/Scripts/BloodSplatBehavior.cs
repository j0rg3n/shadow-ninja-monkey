using UnityEngine;
using System.Collections;

public class BloodSplatBehavior : MonoBehaviour 
{
	void Start () 
	{
		transform.rotation = Quaternion.Euler(0, 360 * Random.value, 0);
		float scale = Random.value * .2f + 1;
		transform.localScale = transform.localScale * scale;		
	}
}
