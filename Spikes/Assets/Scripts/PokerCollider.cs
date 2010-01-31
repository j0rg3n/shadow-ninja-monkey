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
			if (ninjaBehaviour.AmIMyself())
			{
				networkView.RPC("OnPoked", RPCMode.All);
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
	void OnPoked()
	{
		Debug.Log("The " + ninjaBehaviour.Color + " ninja was Poked!");
		ninjaBehaviour.YouJustGotHit(1);
		Flash();
	}
	
	private void Flash()
	{
		flashEndTime = Time.time + 4;
	}
}
