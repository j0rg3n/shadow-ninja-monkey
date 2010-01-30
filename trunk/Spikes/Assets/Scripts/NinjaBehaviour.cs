using UnityEngine;
using System.Collections;

public class NinjaBehaviour : MonoBehaviour {

	
	public enum NinjaColor {MyBlack, MyWhite, HisBlack, HisWhite};
	
	private static Color MyWhiteColor = new Color(0.8f,0.8f,0.8f);
	private static Color MyBlackColor = new Color(0.5f,0.5f,0.5f);
	private static Color HisWhiteColor = new Color(1,1,1);
	private static Color HisBlackColor = new Color(0,0,0);
	
	private NinjaColor color;
	
	private int score;
	
	private int health;
	
	public static int maxHealth = 10;
	
	/* Dubeg
	public void Start()
	{
		Initialize(NinjaColor.MyBlack);
	}
	*/
	
	public void Initialize(NinjaColor color)
	{
		this.color = color;
		setMaterial();
		this.health = maxHealth;
		this.score = 0;
	}
	
	private void setMaterial()
	{
		switch(color)
		{
			case NinjaColor.MyWhite:
				setColorOnAllChildren(MyWhiteColor);
				break;
			case NinjaColor.MyBlack:
				setColorOnAllChildren(MyBlackColor);
				break;
			case NinjaColor.HisWhite:
				setColorOnAllChildren(HisWhiteColor);
				break;
			case NinjaColor.HisBlack:
				setColorOnAllChildren(HisBlackColor);
				break;
		}
	}
	
	private void setColorOnAllChildren(Color color)
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
}
