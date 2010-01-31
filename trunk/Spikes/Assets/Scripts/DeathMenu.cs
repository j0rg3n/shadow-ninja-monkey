using UnityEngine;
using System.Collections;

public class DeathMenu : MonoBehaviour {
	
	public Font menuFont;
	public Texture2D whiteBG ;
	public Texture2D blackBG ;
	public bool whiteKilledBlack;
	
	public void OnGUI () {
		int HUDWidth = Screen.width;
		int HUDHeight = 330;
		
		string winner;
		string loser;
		
		GUI.BeginGroup (new Rect ((Screen.width/2)-(HUDWidth/2), (Screen.height/2)-(HUDHeight/2), HUDWidth, HUDHeight));
		
		//results
		GUIStyle resultStyle = new GUIStyle();
		resultStyle.font = menuFont;
		
		Texture2D bgToUse;
		Color fontColor;
		
		int resultWidth = 250;
		int resultPosLeft;
		if (whiteKilledBlack)
		{
			winner = "light";
			loser = "shadow";
		
			bgToUse = whiteBG;
			fontColor = new Color(0,0,0,1);
			
			resultStyle.alignment = TextAnchor.MiddleRight;
			resultPosLeft = (HUDWidth/2)-resultWidth;	
		} else {
			winner = "shadow";
			loser = "light";
			
			bgToUse = blackBG;
			fontColor = new Color(1,1,1,1);
					
			resultStyle.alignment = TextAnchor.MiddleLeft;
			resultPosLeft = (HUDWidth/2);			
		}
		
		GUI.DrawTexture(new Rect (0,0,HUDWidth,177), bgToUse, ScaleMode.StretchToFill, true, 0);

		resultStyle.normal.textColor = new Color(1,0,0,1);
		GUI.Label(new Rect (resultPosLeft, 5, resultWidth, 50), winner, resultStyle);
		GUI.Label(new Rect (resultPosLeft, 65, resultWidth, 50), "killed", resultStyle);	
		resultStyle.normal.textColor = fontColor;
		GUI.Label(new Rect (resultPosLeft, 125, resultWidth, 50), loser, resultStyle);
		
		
		//instructions
		GUI.DrawTexture(new Rect (0,230,HUDWidth,98), bgToUse, ScaleMode.StretchToFill, true, 0);
		resultStyle.alignment = TextAnchor.MiddleCenter;
		
		int instructionsWidth = 500;
		int instructionsPosLeft = (HUDWidth/2)-(instructionsWidth/2);
		
		resultStyle.normal.textColor = fontColor;
		GUI.Label(new Rect (instructionsPosLeft, 230, instructionsWidth, 50), "space to fight", resultStyle);	
		
		GUI.Label(new Rect (instructionsPosLeft, 280, instructionsWidth, 50), "esc to quit", resultStyle);	
		
		
		GUI.EndGroup ();	
	
	}
}
