using UnityEngine;
using System.Collections;

public class GameHUD : MonoBehaviour {
	
	public int whiteScore = 0;
	public int blackScore = 0;
	public float whiteHealth = 100.0f;
	public float blackHealth = 100.0f;

	public Font scoreFont;
	public Texture2D whiteBarBG ;
	public Texture2D blackBarBG ;

	public void OnGUI () {
	//GUI.skin = ninjaSkin;
	
	int HUDWidth = 446;
	int HUDHeight = 38;
	int maxBarWidth = 168;
	
	// Make a group on the center of the screen
	GUI.BeginGroup (new Rect ((Screen.width/2) - (HUDWidth/2), 0, HUDWidth, HUDHeight));
	// All rectangles are now adjusted to the group. (0,0) is the topleft corner of the group.

	GUI.DrawTexture(new Rect (0,0,HUDWidth,HUDHeight), blackBarBG, ScaleMode.StretchToFill, true, 0);
	GUI.DrawTexture(new Rect (5,5,218,28), whiteBarBG, ScaleMode.StretchToFill, true, 0);
	
	int blackWidth = (int)Mathf.Max(blackHealth/100.0f * maxBarWidth, 0.0f);
	int whiteWidth = (int)Mathf.Max(whiteHealth/100.0f * maxBarWidth, 0.0f);
	GUI.DrawTexture(new Rect (52,8,blackWidth,22), blackBarBG, ScaleMode.StretchToFill, true, 0);
	GUI.DrawTexture(new Rect ((HUDWidth-52)-whiteWidth,8,whiteWidth,22), whiteBarBG, ScaleMode.StretchToFill, true, 0);
	
	GUIStyle scoreStyle = new GUIStyle();
	scoreStyle.padding.left = 6;
	scoreStyle.padding.right = 6;
	scoreStyle.padding.top = 3;
	//scoreStyle.padding.bottom = 5;
	scoreStyle.font = scoreFont;
		
	//whitePlayer
	scoreStyle.normal.textColor = new Color(1,1,1,1);
	scoreStyle.alignment = TextAnchor.MiddleRight;
	GUI.Label(new Rect (0,5,HUDWidth,28), pad(whiteScore,3), scoreStyle);

	//blackPlayer
	scoreStyle.normal.textColor = new Color(0,0,0,1);
	scoreStyle.alignment = TextAnchor.MiddleLeft;	
	GUI.Label(new Rect (0,5,HUDWidth,28), pad(blackScore,3), scoreStyle);

	

	// End the group we started above. This is very important to remember!
	GUI.EndGroup ();
}

public void SetHealth(NinjaBehaviour.NinjaColor color, int health)
{
	switch(color)
	{
		case NinjaBehaviour.NinjaColor.Black:
			blackHealth = health*10;
		break;
		case NinjaBehaviour.NinjaColor.White:
			whiteHealth = health*10;
		break;
		
	}
}

public void RegisterKill(NinjaBehaviour.NinjaColor color)
{
	switch(color)
	{
		case NinjaBehaviour.NinjaColor.Black:
			whiteScore++;
		break;
		case NinjaBehaviour.NinjaColor.White:
			blackScore++;
		break;
		
	}
}

private string pad(int number, int length) {
   
    string str = "" + number;
    while (str.Length < length) {
        str = "0" + str;
    }
   
    return str;

}
}
