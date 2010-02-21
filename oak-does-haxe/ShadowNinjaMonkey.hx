import flash.display.LineScaleMode;
import flash.display.CapsStyle;
import flash.display.JointStyle;
import LightSource;

class ShadowNinjaMonkey {

	private static var shadows:Shadows;	
	private static var lightLayer:LightLayer;
	private static var lights:Array<LightSource>;
	private static var obstacles:Array<Obstacle>;

	static function main() {
		shadows = new Shadows();
		lightLayer = new LightLayer();
	
		lights = new Array<LightSource>();
		//lights.push(new LightSource());
		lights.push(new LightSource());
		
		obstacles = new Array<Obstacle>();
		/*
		obstacles.push(new Obstacle(0, 0, flash.Lib.current.stage.stageWidth, 2));
		obstacles.push(new Obstacle(0, flash.Lib.current.stage.stageHeight-2, flash.Lib.current.stage.stageWidth, 2));
		obstacles.push(new Obstacle(0, 0, 2, flash.Lib.current.stage.stageHeight));
		obstacles.push(new Obstacle(flash.Lib.current.stage.stageWidth-2, 0, 2, flash.Lib.current.stage.stageHeight));
		*/
		
		obstacles.push(new Obstacle(140, 250, 135, 20));
		obstacles.push(new Obstacle(340, 250, 142, 20));
		obstacles.push(new Obstacle(240, 320, 150, 45));
		
		/*
		obstacles.push(new Obstacle(220, 150, 20, 20));
		obstacles.push(new Obstacle(250, 200, 12, 6));
		obstacles.push(new Obstacle(440, 150, 42, 4));
		
		var i:Int = 0;
		while(i < 5)
		{
			obstacles.push(new Obstacle(340, 150+i*40, 20, 10));
			i++;
		}
		i = 0;
		while(i < 10)
		{
			obstacles.push(new Obstacle(80+i*35, 450-i*2, 12, 25+i*8));
			i++;
		}
		*/
		
		flash.Lib.current.addEventListener(flash.events.Event.ENTER_FRAME,function(_) ShadowNinjaMonkey.onEnterFrame());
	}
	
	private static var time:Float = 0;
	static function onEnterFrame()
	{
		time += 0.0004;
		
		lights[0].moveTo(
			Math.round(flash.Lib.current.stage.stageWidth/2.0+Math.sin(time)*(flash.Lib.current.stage.stageWidth/2.0-10)),
			Math.round(flash.Lib.current.stage.stageHeight/2.0+Math.cos(time)*(flash.Lib.current.stage.stageHeight/2.0-10))
		);
		/*
		lights[1].moveTo(
			Math.round(flash.Lib.current.stage.stageWidth/2.0+Math.sin(-time*0.4)*(flash.Lib.current.stage.stageWidth/2.0-10)),
			Math.round(flash.Lib.current.stage.stageHeight/2.0+Math.cos(-time*0.4)*(flash.Lib.current.stage.stageHeight/2.0-10))
		);
		*/
		
		//shadows.drawShadows(lights,obstacles);
		lightLayer.drawLights(lights,obstacles);
	}	
}

