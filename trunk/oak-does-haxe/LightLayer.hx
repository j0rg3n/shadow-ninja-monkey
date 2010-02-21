import flash.display.BlendMode;

class LightLayer
{
	var myLight : flash.display.Shape;

	public function new()
	{
		myLight = new flash.display.Shape();
		myLight.graphics.lineStyle ( 0, 0x0f00a0, 0, false);
		myLight.blendMode = BlendMode.NORMAL;
		flash.Lib.current.addChild(myLight);
	}

	public function drawLights(lights:Array<LightSource>,obstacles:Array<Obstacle>):Void
	{
		myLight.graphics.clear();
		for(light in lights)
		{
			light.drawLight(obstacles, myLight, 0xffffff);
		}
	}
}

