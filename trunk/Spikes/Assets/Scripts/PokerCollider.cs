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
	
	void Start()
	{
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
			Debug.Log(name + " was Poked !");

			if (networkView != null && networkView.isMine)
			{
				// We have only two players, so RPCMode.Others is in effect the other player.
				networkView.RPC("OnPokedOpponent", RPCMode.Others);
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
		Flash();
	}
	
	private void Flash()
	{
		flashEndTime = Time.time + 4;
	}
}
