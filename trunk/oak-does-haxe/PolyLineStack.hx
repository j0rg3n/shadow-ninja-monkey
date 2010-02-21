
class PolyLineStack
{
	private var light:Point;
	private var lines:Array<SortablePolyLine>;
	
	public function new(light:Point)
	{
		this.light = light;
		this.lines = new Array<SortablePolyLine>();
	}
	
	public function insert(line:SortablePolyLine)
	{
		lines.push(line);
		lines.sort(this.compareDistance);
	}
	
	public function compareDistance(l1:SortablePolyLine,l2:SortablePolyLine):Int
	{
		return light.lengthSquared(l1.left.point)-light.lengthSquared(l2.left.point);
	}
	
	public function exists(line:SortablePolyLine)
	{
		for(l in lines)
		{
			if(l == line)
				return true;
		}
		return false;
	}
	
	public function remove(line:SortablePolyLine)
	{
		lines.remove(line);
	}

	public function getFrontMost()
	{
		return lines[0];
	}	
}
