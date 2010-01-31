using UnityEngine;
using System.Collections;

public class NinjaBehaviour : MonoBehaviour {
	
	public enum NinjaColor {Black, White};
	
	private static Color MyWhiteColor = new Color(1,0,0);
	private static Color MyBlackColor = new Color(0,0,1);
	private static Color HisWhiteColor = new Color(1,1,1);
	private static Color HisBlackColor = new Color(0,0,0);
	
	
	private bool noNetwork = true;
	
	private NinjaColor color;
	
	private int score;	
	private int health;
	
	public const int maxHealth = 10;
	
	// True if player is this ninja
	public bool AmIMyself()
	{
		if (Network.peerType == NetworkPeerType.Client)
		{
			return color == NinjaColor.Black;
		}
		else
		{
			return color == NinjaColor.White;
		}
	}
	
	public void Initialize(NinjaBehaviour.NinjaColor ninjaColor, bool useNetwork)
	{
		this.noNetwork = !useNetwork;
		this.color = ninjaColor;
		this.health = maxHealth;
		this.score = 0;
	}
	
	void Start()
	{
		if (AmIMyself())
		{
			gameObject.AddComponent<AudioListener>();
		}
		
		UpdateMaterial();
		UpdateHUD();
	}
	
	private void UpdateMaterial()
	{
		switch(color)
		{
			case NinjaColor.White:
				SetColorOnAllChildren(AmIMyself() ? MyWhiteColor : HisWhiteColor);
				break;
			
			case NinjaColor.Black:
				SetColorOnAllChildren(AmIMyself() ? MyBlackColor : HisBlackColor);
				break;
		}
	}
	
	private void SetColorOnAllChildren(Color color)
	{
		MeshRenderer[] children = gameObject.GetComponentsInChildren<MeshRenderer>();
		
		foreach(MeshRenderer m in children) {
			m.material.color = color;
		}
		
	}
	
	public int Health
	{
		get { return health; }
	}
	
	public int Score
	{
		get { return score; }
	}
	
	public NinjaColor Color
	{
		get { return color; }
	}
	
	public void YouJustGotHit(int dam)
	{
		if (AmIMyself())
		{
			health-=dam;
			if(health <= 0)
			{
				// Tell all servers we were killed.
				networkView.RPC("OnKilled", RPCMode.All);
			}
			UpdateHUD();
		}
		else
		{
			// WHO AM I!!??!
		}
		
		//TODO Blood splatter sounds.
	}
	
	[RPC]
	void OnKilled()
	{
		Debug.Log("I am killed, says the " + color + " ninja.");

		if (AmIMyself())
		{
			++score;
			
			// TODO: End game here.
			health = maxHealth;
			
			UpdateHUD();
		}
	}

	// Keep HUD updated with our current health and score.
	public void UpdateHUD()
	{
		// TODO: Let HUD reference the actual NinjaBehaviour.
		GameObject.Find("GUIRoot").GetComponent<GameHUD>().SetHealth(color, health);
		GameObject.Find("GUIRoot").GetComponent<GameHUD>().SetScore(color, score);
	}
	
	void OnSerializeNetworkView(BitStream stream, NetworkMessageInfo info)
	{
		int n = (int) color;
		stream.Serialize(ref n);
		color = (NinjaColor) n;
		
		stream.Serialize(ref health);
		stream.Serialize(ref score);

		if (!stream.isWriting)
		{
			UpdateHUD();
		}
	}
}
