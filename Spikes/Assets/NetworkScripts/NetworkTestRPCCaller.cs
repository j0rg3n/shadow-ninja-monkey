using UnityEngine;
using System.Collections;

public class NetworkTestRPCCaller : MonoBehaviour 
{
	public Material activatedMaterial;
	private Material defaultMaterial;
	
	private MeshRenderer meshRenderer;
	private float resetTime = 0;
	
	void Start () 
	{
		meshRenderer = GetComponent<MeshRenderer>();
		if (meshRenderer == null)
		{
			return;
		}

		defaultMaterial = meshRenderer.material;
	}
	
	void Update () 
	{
		if (meshRenderer == null)
		{
			return;
		}
		
		if (networkView != null && networkView.isMine)
		{
			if (Input.GetButton("Fire1"))
			{
				networkView.RPC("Activate", RPCMode.All);
			}
		}
		
		float time = Time.time;
		if (meshRenderer.material != defaultMaterial)
		{
			if (time > resetTime)
			{
				meshRenderer.material = defaultMaterial;
			}
		}
	}
	
	[RPC]
	private void Activate()
	{
		meshRenderer.material = activatedMaterial;
		resetTime = Time.time + 1;
	}
}
