using UnityEngine;
using System.Collections;

public class NinjaAnimation : MonoBehaviour 
{
	enum State { Stand, Poke, Block }
	
	public float pokeDelay = 2.0f;
	public float blockDelay = 1.0f;

	private State state = State.Stand;
	private float stateResetTime = 0.0f;
	private NinjaPokerAnimation pokerAnimation;
	
	// Use this for initialization
	void Start () 
	{
		pokerAnimation = GetComponentInChildren<NinjaPokerAnimation>();
	}
	
	// Update is called once per frame
	void Update () 
	{
		float time = Time.time;
		if (state != State.Stand)
		{
			if (time < stateResetTime)
			{
				// Do nothing.
			}
			else
			{
				state = State.Stand;
				pokerAnimation.Stand();
			}
		}
		else
		{			
			if (Input.GetButton("Fire1"))
			{
				pokerAnimation.Poke();
				state = State.Poke;
				stateResetTime = time + pokeDelay;
			}
			else if (Input.GetButton("Fire2"))
			{
				pokerAnimation.Block();
				state = State.Block;
				stateResetTime = time + blockDelay;
			}
		}
	}
}
