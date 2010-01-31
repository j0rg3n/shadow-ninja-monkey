using UnityEngine;
using System.Collections;

public class GUIRootBehaviour : MonoBehaviour {

	private NinjaLevel currentLevel;
	public GameObject directConnect;
	
	private GameObject currentSubmenu;
	
	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
	
	}

	// Called by ConnectGui once connected.
	public void OnNetworkLoadedLevel ()
	{
		const bool instantiateOnNetwork = true;
		currentLevel = GetComponent<LevelLoader>().LoadLevel002(instantiateOnNetwork);
	}

	void OnGUI()
	{
		if (GUI.Button (new Rect (10,110,150,40), "Singleplayer")) {
			if(currentLevel!=null)
				currentLevel.Dispose();

			const bool instantiateOnNetwork = false;
			currentLevel = GetComponent<LevelLoader>().LoadLevel002(instantiateOnNetwork);			
		}
		
		if (GUI.Button (new Rect (10,160,150,40), "Dualplayer")) 
		{
			if (currentLevel!=null)
			{
				currentLevel.Dispose();
			}

			if(currentSubmenu != null)
				Destroy(currentSubmenu);
			
			currentSubmenu = (GameObject) Instantiate(directConnect);
		}
	}
}
