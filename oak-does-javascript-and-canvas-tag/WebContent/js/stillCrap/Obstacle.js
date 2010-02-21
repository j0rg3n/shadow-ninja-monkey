
class Obstacle
{
	//private var x:Int;
	//private var y:Int;
	var mySprite : flash.display.Sprite;
	var ulCorner : Point;
	var urCorner : Point;
	var llCorner : Point;
	var lrCorner : Point;

	public function new(x:Int, y:Int, w:Int, h:Int)
	{
		mySprite = new flash.display.Sprite();
		mySprite.graphics.beginFill ( 0xff5555 );  // the color of the rectangle
		mySprite.graphics.lineStyle ( 1, 0x000000, 1, false, flash.display.LineScaleMode.NONE ); // the border style
		mySprite.graphics.drawRect ( 0, 0, w, h);
		mySprite.graphics.endFill();
		mySprite.x = x;
		mySprite.y = y;
		flash.Lib.current.addChild(mySprite);
		
		ulCorner = new Point(x,y);
		urCorner = new Point(x+w,y);
		llCorner = new Point(x,y+h);
		lrCorner = new Point(x+w,y+h);
	}

	// Calculate the most 'left' corner that casts shadow
	public function getLeftCorner(light:Point):Point
	{
		var p:Point = ulCorner;
		if(urCorner.leftOf(light, p))
			p = urCorner;
		if(llCorner.leftOf(light, p))
			p = llCorner;
		if(lrCorner.leftOf(light, p))
			p = lrCorner;
		return p;
	}
	// Calculate the most 'left' corner that casts shadow
	public function getRightCorner(light:Point):Point
	{
		var p:Point = ulCorner;
		if(!urCorner.leftOf(light, p))
			p = urCorner;
		if(!llCorner.leftOf(light, p))
			p = llCorner;
		if(!lrCorner.leftOf(light, p))
			p = lrCorner;
		return p;
	}
}

