import flash.display.BlendMode;

class Shadows
{
	var myShadow : flash.display.Shape;

	public function new()
	{
		myShadow = new flash.display.Shape();
		myShadow.graphics.lineStyle ( 0, 0x000000, 0, false);
		myShadow.blendMode = BlendMode.NORMAL;
		flash.Lib.current.addChild(myShadow);
	}

	public function drawShadows(lights:Array<LightSource>,obstacles:Array<Obstacle>):Void
	{
		myShadow.graphics.clear();
		for(light in lights)
		{
			light.drawShadows(obstacles, myShadow, 0x000000);
		}
	}
}

