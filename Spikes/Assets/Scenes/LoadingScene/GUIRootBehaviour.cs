using UnityEngine;
using System.Collections;

public class GUIRootBehaviour : MonoBehaviour {

	private NinjaLevel currentLevel;
	public GameObject directConnect;
	private bool hasPlayedTutorial = false;
	
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
		DisposeCurrentLevel();
		const bool instantiateOnNetwork = true;
		if(hasPlayedTutorial)
		{
			currentLevel = GetComponent<LevelLoader>().LoadLevel002(instantiateOnNetwork);
		}
		else
		{
			currentLevel = GetComponent<LevelLoader>().LoadTutorialLevel(instantiateOnNetwork);
			hasPlayedTutorial = true;
		}
	}
	
	public void DisposeCurrentLevel()
	{
		if(currentLevel!=null)
				currentLevel.Dispose();
	}

	void OnGUI()
	{
		if (GUI.Button (new Rect (10,110,150,40), "Singleplayer")) {
			DisposeCurrentLevel();

			const bool instantiateOnNetwork = false;
			currentLevel = GetComponent<LevelLoader>().LoadLevel002(instantiateOnNetwork);
			GetComponent<GlobalGameState>().SetFightingState();
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
