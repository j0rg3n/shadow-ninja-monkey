using UnityEngine;
using System.Collections;

public class NinjaPokerAnimation : MonoBehaviour 
{
	public float speed = 5.0f;
	public Quaternion blockRotation = Quaternion.Euler(90, 0, 0);
	public Quaternion pokeRotation = Quaternion.Euler(0, 0, 90);
	public Quaternion standRotation = Quaternion.Euler(0, 0, 0);
	
	// Store separately, as Transform is handled by reference, not by value.
	private Quaternion defaultRotation;
	private Vector3 defaultPosition;

	// Interpolation state
	private Quaternion targetRotation;
	private Quaternion currentRotation;
	private float moveStartTime = -1000.0f;

	public void Poke()
	{
		SetTargetRotation(pokeRotation);
	}
	
	public void Block()
	{
		SetTargetRotation(blockRotation);
	}
	
	public void Stand()
	{
		SetTargetRotation(defaultRotation);
	}
	
	// Use this for initialization
	void Start () 
	{
		defaultPosition = transform.localPosition;
		defaultRotation = transform.localRotation;
		targetRotation = defaultRotation;
	}
	
	// Update is called once per frame
	void Update () 
	{
		float t = (Time.time - moveStartTime) * speed;
		
		Quaternion r;
		if (t <= 0)
		{
			r = currentRotation;
		}
		else if (t < 1)
		{
			r = Quaternion.Slerp(currentRotation, targetRotation, t);
		}
		else
		{
			r = targetRotation;
		}
		
		transform.localRotation = r * defaultRotation;
		transform.localPosition = r * defaultPosition;
	}

	private void SetTargetRotation(Quaternion newTargetRotation)
	{
		currentRotation = targetRotation;
		moveStartTime = Time.time;
		targetRotation = newTargetRotation;
	}
	
	void OnSerializeNetworkView(BitStream stream, NetworkMessageInfo info)
	{
		stream.Serialize(ref currentRotation);
		stream.Serialize(ref targetRotation);
		stream.Serialize(ref moveStartTime);
	}
}
