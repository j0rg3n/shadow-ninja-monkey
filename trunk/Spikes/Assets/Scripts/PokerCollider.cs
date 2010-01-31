using UnityEngine;
using System.Collections;

[RequireComponent(typeof(NetworkView))]
public class PokerCollider : MonoBehaviour 
{
	public Material pokingMaterial;
	public float flashRate = 7;
	
	private Material defaultMaterial;

	private MeshRenderer meshRenderer;
	private float flashEndTime = 0;
	private NinjaBehaviour ninjaBehaviour;

	void Start()
	{
		ninjaBehaviour = GetComponent<NinjaBehaviour>();
		
		meshRenderer = GetComponentInChildren<MeshRenderer>();
		if (meshRenderer != null)
		{
			defaultMaterial = meshRenderer.material;
		}
	}
	
	void OnTriggerEnter(Collider other)
	{
		if (other.tag == "Sword")
		{
			Debug.Log("I, the " + ninjaBehaviour.Color + " ninja, was Poked !");

			if (ninjaBehaviour.AmIMyself())
			{
				// Tell the other player that he poked us.
				// We have only two players, so RPCMode.Others is in effect the other player.
				networkView.RPC("OnPokedOpponent", RPCMode.Others);
				ninjaBehaviour.YouJustGotHit(1);
			}
		}
	}

	void Update()
	{
		if (meshRenderer != null)
		{
			float time = Time.time;
			bool flashing = time < flashEndTime;
			if (flashing)
			{
				bool on = ((int)(time * flashRate) & 1) == 1;
				meshRenderer.material = on ? pokingMaterial : defaultMaterial;	
			}
			else if (meshRenderer.material != defaultMaterial)
			{	
				meshRenderer.material = defaultMaterial;
			}
		}
	}
	
	[RPC]
	void OnPokedOpponent()
	{
		Debug.Log("I, the " + ninjaBehaviour.Color + " ninja, poked the opponent righteously!");
		Flash();
	}
	
	private void Flash()
	{
		flashEndTime = Time.time + 4;
	}
}
