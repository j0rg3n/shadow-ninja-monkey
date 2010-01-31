using UnityEngine;
using System.Collections;

public class GlobalGameState : MonoBehaviour {


	public enum State {Fighting, Loading, ShowingDeathScreen};
	
	private State state = State.Loading;
	
	public State GameState
	{
		get {return state;}
		set {this.state = value;}
	}
	
	// Set global game state to showing death screen state
	public void SetShowingDeathScreenState()
	{
		GameState = State.ShowingDeathScreen;
		GetComponent<DeathMenu>().enabled = true;
	}
	
	// Set global game state to loading state (main menu)
	public void SetLoadingState()
	{
		//TODO Dispose level
		GetComponent<GUIRootBehaviour>().DisposeCurrentLevel();
		
		if(GameState == State.Fighting)
		{
			
			
		}
		else if(GameState == State.ShowingDeathScreen)
		{
			GetComponent<DeathMenu>().enabled = false;
			
			
		}
		GameState = State.Loading;
		GetComponent<GUIRootBehaviour>().enabled = true;
	}
	
	// Set global game state to fighting state
	public void SetFightingState()
	{
		if(GameState == State.ShowingDeathScreen)
		{
			GetComponent<DeathMenu>().enabled = false;
			
			//Reset ninja positions:
			if(Network.peerType == NetworkPeerType.Server)
			{
				GetComponent<LevelLoader>().ServerRespawnGame();
			}
			
			//Todo Switch level
			//GetComponent<LevelLoader>().
		}
		else if(GameState == State.Loading)
		{
			GetComponent<GUIRootBehaviour>().enabled = false;
		}
		GameState = State.Fighting;
	}
	
	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
	
	}
}
