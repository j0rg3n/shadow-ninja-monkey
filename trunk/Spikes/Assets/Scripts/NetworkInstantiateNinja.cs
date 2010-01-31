using UnityEngine;
using System.Collections;

public class NetworkInstantiateNinja : MonoBehaviour {

	public Transform playerPrefab;

	public void OnNetworkLoadedLevel ()
	{
		Network.Instantiate(playerPrefab, transform.position, transform.rotation, 0);
	}

	void OnPlayerDisconnected (NetworkPlayer player)
	{
		Debug.Log("Server destroying player");
		Network.RemoveRPCs(player, 0);
		Network.DestroyPlayerObjects(player);
	}
}
