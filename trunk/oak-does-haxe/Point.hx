
class Point
{
	public var x:Int;
	public var y:Int;
	
	public function new(x:Int, y:Int)
	{
		this.x = x;
		this.y = y;	
	}
	
	/**
	 * Is this point left of the line A->B
	 */
	public function leftOf(A:Point,B:Point):Bool
	{
		return this.leftOfRaw(A,B) > 0;
	}
	
	public function leftOfRaw(A:Point,B:Point):Int
	{
		return ( (B.x-A.x)*(this.y-A.y) - (B.y-A.y)*(this.x-A.x) );
	}
	
	public function subtract(p:Point):Vector
	{
		return new Vector(x-p.x, y-p.y);
	}
	
	public function lengthSquared(p:Point):Int
	{
		var dx = x-p.x;
		var dy = y-p.y;
		return dx*dx + dy*dy;
	}

	public function xIntersectionLocal(x:Int, p1:Point, p2:Point)
	{
		this.x = x;
		var dir:Vector = p1.subtract(p2).normalizeLocal();
		var dist:Float = (p1.x - x);
		this.y = Math.floor(p1.y - dir.y*dist);
	}
	
	public function yIntersectionLocal(y:Int, p1:Point, p2:Point)
	{
		this.y = y;
		var dir:Vector = p1.subtract(p2).normalizeLocal();
		var dist:Float = (p1.y - y);
		this.x = Math.floor(p1.x - dir.x*dist);
	}
}
