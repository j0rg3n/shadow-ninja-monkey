
class SortablePolyPoint
{
	public var parent:SortablePolyLine;
	public var point:Point;
	public var isLeft:Bool;
	public function new(parent:SortablePolyLine, isLeft:Bool)
	{
		this.parent = parent;
		this.isLeft = isLeft;
	}
}

