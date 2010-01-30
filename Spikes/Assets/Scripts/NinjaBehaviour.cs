using UnityEngine;
using System.Collections;

public class NinjaBehaviour : MonoBehaviour {

	
	public enum NinjaColor {Black, White};
	
	private static Color MyWhiteColor = new Color(0.8f,0.8f,0.8f);
	private static Color MyBlackColor = new Color(0.5f,0.5f,0.5f);
	private static Color HisWhiteColor = new Color(1,1,1);
	private static Color HisBlackColor = new Color(0,0,0);
	
	
	private bool noNetwork = true;
	
	private NinjaColor color;
	
	private int score;
	
	private int health;
	
	public static int maxHealth = 10;
	
	/*
	public void Start()
	{
		Initialize(NinjaColor.MyBlack);
	}
	*/
	
	
	public void Initialize(NinjaColor color, bool useNetwork)
	{
		this.noNetwork = !useNetwork;
		this.color = color;
		UpdateMaterial();
		this.health = maxHealth;
		this.score = 0;
	}
	
	// True if player is this ninja
	public bool AmIMyself()
	{
		NetworkView networkView = GetComponent<NetworkView>();
		return (noNetwork || networkView == null ||  networkView.isMine);
	}
	
	private void UpdateMaterial()
	{
		switch(color)
		{
			case NinjaColor.White:
				SetColorOnAllChildren(AmIMyself()? MyWhiteColor : HisWhiteColor);
				break;
			case NinjaColor.Black:
				SetColorOnAllChildren(AmIMyself()? MyBlackColor : HisBlackColor);
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
		if(health <= 0)
		{
			//TODO Die
		}
		
		//TODO Blood splatter sounds.
	}
}
