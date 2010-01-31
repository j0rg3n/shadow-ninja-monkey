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
	
	public void SetShowingDeathScreenState()
	{
		GameState = State.ShowingDeathScreen;
		GetComponent<DeathMenu>().enabled = true;
	}
	
	public void SetLoadingState()
	{
		if(GameState == State.Fighting)
		{
			//TODO Dispose level
			
		}
		else if(GameState == State.ShowingDeathScreen)
		{
			GetComponent<GUIRootBehaviour>().enabled = false;
		}
		GameState = State.Loading;
		GetComponent<GUIRootBehaviour>().enabled = true;
	}
	
	public void SetFightingState()
	{
		if(GameState == State.ShowingDeathScreen)
		{
			GetComponent<DeathMenu>().enabled = false;
		}
		else if(GameState == State.ShowingDeathScreen)
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
