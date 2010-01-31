var whiteScore = 0;
var blackScore = 0;
var whiteHealth = 100.0;
var blackHealth = 100.0;

var scoreFont : Font;
var whiteBarBG : Texture2D;
var blackBarBG : Texture2D;

function OnGUI () {
	//GUI.skin = ninjaSkin;
	
	var HUDWidth = 446;
	var HUDHeight = 38;
	var maxBarWidth = 168;
	
	// Make a group on the center of the screen
	GUI.BeginGroup (Rect ((Screen.width/2) - (HUDWidth/2), 0, HUDWidth, HUDHeight));
	// All rectangles are now adjusted to the group. (0,0) is the topleft corner of the group.

	GUI.DrawTexture(Rect (0,0,HUDWidth,HUDHeight), blackBarBG, ScaleMode.StretchToFill, true, 0);
	GUI.DrawTexture(Rect (5,5,218,28), whiteBarBG, ScaleMode.StretchToFill, true, 0);
	
	var blackWidth = Mathf.Max(blackHealth/100 * maxBarWidth, 0.0);
	var whiteWidth = Mathf.Max(whiteHealth/100 * maxBarWidth, 0.0);
	Debug.Log(blackWidth);
	GUI.DrawTexture(Rect (52,8,blackWidth,22), blackBarBG, ScaleMode.StretchToFill, true, 0);
	GUI.DrawTexture(Rect ((HUDWidth-52)-whiteWidth,8,whiteWidth,22), whiteBarBG, ScaleMode.StretchToFill, true, 0);
	
	var scoreStyle = new GUIStyle();
	scoreStyle.padding.left = 6;
	scoreStyle.padding.right = 6;
	scoreStyle.padding.top = 3;
	//scoreStyle.padding.bottom = 5;
	scoreStyle.font = scoreFont;
		
	//whitePlayer
	scoreStyle.normal.textColor = new Color(1,1,1,1);
	scoreStyle.alignment = TextAnchor.MiddleRight;
	GUI.Label(Rect (0,5,HUDWidth,28), pad(whiteScore,3), scoreStyle);

	//blackPlayer
	scoreStyle.normal.textColor = new Color(0,0,0,1);
	scoreStyle.alignment = TextAnchor.MiddleLeft;	
	GUI.Label(Rect (0,5,HUDWidth,28), pad(blackScore,3), scoreStyle);

	

	// End the group we started above. This is very important to remember!
	GUI.EndGroup ();
}

function pad(number, length) {
   
    var str = "" + number;
    while (str.length < length) {
        str = "0" + str;
    }
   
    return str;

}