
class Vector
{
	public var x:Float;
	public var y:Float;
	
	public function new(x:Float, y:Float)
	{
		this.x = x;
		this.y = y;	
	}
	
	public function normalizeLocal():Vector
	{
		var l:Float = Math.sqrt(x*x + y*y);
		x = x / l;
		y = y / l;
		return this;
	}
	
	public function multiplyLocal(s:Float):Vector
	{
		x = x*s;
		y = y*s;
		return this;
	}
}
