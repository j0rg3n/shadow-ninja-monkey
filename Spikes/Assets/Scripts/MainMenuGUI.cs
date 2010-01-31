using UnityEngine;
using System.Collections;

public class MainMenuGUI : MonoBehaviour {
	
	public Font titleFont;	

	public void OnGUI () {
		int titleWidth = Screen.height;

		GUI.BeginGroup (new Rect ((Screen.width/2) - (titleWidth/2), 0, titleWidth, Screen.height));
	

		GUIStyle titleStyle = new GUIStyle();
		titleStyle.font = titleFont;
		titleStyle.normal.textColor = new Color(1,1,1,1);
		titleStyle.alignment = TextAnchor.MiddleRight;
		
		GUI.Label(new Rect (-15,5,titleWidth,200), "Shadow\nNinja\nMonkey", titleStyle);
		/*
		titleStyle.alignment = TextAnchor.LowerLeft;		
		if(GUI.Button(new Rect (15,(Screen.height-225),150,60), "Start", titleStyle))
		{
			Debug.Log("Start FIGHT!");

		}

		if(GUI.Button(new Rect (15,(Screen.height-145),105,60), "Join", titleStyle))
		{
			Debug.Log("Join FIGHT!");
		}

		if(GUI.Button(new Rect (15,(Screen.height-65),110,60), "Quit", titleStyle))
		{
			Debug.Log("QUIT");
		}
*/

		if(GUI.Button(new Rect (15,(Screen.height-65),110,60), "Quit", titleStyle))
		{
			Debug.Log("QUIT");
		}
		
		GUI.EndGroup ();
	}
}