/**
 * ...
 * @author jorgen@fabeljet.com
 */

//package ;

import flash.events.Event;

class Test 
{
	public static function main() 
	{
        trace("Hello World !");		
		
		var mc:flash.display.MovieClip = flash.Lib.current;
		mc.addEventListener(Event.ENTER_FRAME, onFrame, false, 0, true);
	}
	
	private static function onFrame(param : Dynamic)
	{
		++i;
		
		var mc:flash.display.MovieClip = flash.Lib.current;
        mc.graphics.beginFill(0xFF0000);
        mc.graphics.moveTo(50,50 + i);
        mc.graphics.lineTo(100,50 + i);
        mc.graphics.lineTo(100,100 + i);
        mc.graphics.lineTo(50,100 + i);
        mc.graphics.endFill();
	}
	
	static var i : Int = 0;
}