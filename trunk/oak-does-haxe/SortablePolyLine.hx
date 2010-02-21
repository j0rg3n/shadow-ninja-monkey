

class SortablePolyLine
{
	private static var idCounter:Int;
	public var id:Int;
	public var left:SortablePolyPoint;
	public var right:SortablePolyPoint;
	
	public function new()
	{
		id = idCounter++;
		left = new SortablePolyPoint(this, true);
		right = new SortablePolyPoint(this, false);
	}
}
