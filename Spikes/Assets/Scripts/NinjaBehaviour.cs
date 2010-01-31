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
	
	public static int maxHealth = 10;
	
	// True if player is this ninja
	public bool AmIMyself()
	{
		NetworkView networkView = GetComponent<NetworkView>();
		return (noNetwork || networkView == null ||  networkView.isMine);
	}
	
	void Start()
	{
		bool useNetwork = Network.peerType != NetworkPeerType.Disconnected;
		NinjaBehaviour.NinjaColor ninjaColor;
		if (Network.peerType == NetworkPeerType.Server)
		{
			ninjaColor = NinjaBehaviour.NinjaColor.White;
		}
		else
		{
			ninjaColor = NinjaBehaviour.NinjaColor.Black;
		}
		
		this.noNetwork = !useNetwork;
		this.color = ninjaColor;
		this.health = maxHealth;
		this.score = 0;

		UpdateMaterial();
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
		health-=dam;
		GameObject.Find("GUIRoot").GetComponent<GameHUD>().SetHealth(color,health);
		if(health <= 0)
		{
			GameObject.Find("GUIRoot").GetComponent<GameHUD>().RegisterKill(color);
		}
		
		
		
		//TODO Blood splatter sounds.
	}
}
