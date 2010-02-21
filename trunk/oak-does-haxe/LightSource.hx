
class LightSource
{
	//private var x:Int;
	//private var y:Int;
	var mySprite : flash.display.Sprite;
	var myPoint : Point;

	public function new()
	{
		myPoint = new Point(0, 0);
		mySprite = new flash.display.Sprite();
		mySprite.graphics.lineStyle ( 1, 0x000000, 1, false, flash.display.LineScaleMode.NONE ); // the border style
		mySprite.graphics.drawRect ( -1, -1, 2, 2);
		flash.Lib.current.addChild(mySprite);
	}

	public function move(x:Int, y:Int):Void
	{
		myPoint.x += x;
		myPoint.y += y;
		updateSprite();
	}

	public function moveTo(x:Int, y:Int):Void
	{
		myPoint.x = x;
		myPoint.y = y;
		updateSprite();
	}
	
	private function updateSprite()
	{
		mySprite.x = myPoint.x;
		mySprite.y = myPoint.y;
	}
	
	
	public function drawShadows(obstacles:Array<Obstacle>, myShadow:flash.display.Shape,color:Int):Void
	{
		for(obstacle in obstacles)
		{
			//var obstacle:Obstacle = obstacles[i];
			var left:Point = obstacle.getLeftCorner(myPoint);
			var right:Point = obstacle.getRightCorner(myPoint);
		
			var dir1:Vector = left.subtract(myPoint).normalizeLocal().multiplyLocal(1000);
			var dir2:Vector = right.subtract(myPoint).normalizeLocal().multiplyLocal(1000);
		
			myShadow.graphics.beginFill(color);
			myShadow.graphics.moveTo(left.x,left.y);
			myShadow.graphics.lineTo(left.x+dir1.x,left.y+dir1.y);
			myShadow.graphics.lineTo(right.x+dir2.x,right.y+dir2.y);
			myShadow.graphics.lineTo(right.x,right.y);
			myShadow.graphics.endFill();
		}
	}

	public function drawLight(obstacles:Array<Obstacle>, lightLayer:flash.display.Shape, color:Int):Void
	{
		var sortedObstacles:Array<SortablePolyPoint> = new Array<SortablePolyPoint>();
		
		for(obstacle in obstacles)
		{
			//var obstacle:Obstacle = obstacles[i];
			var poly:SortablePolyLine = new SortablePolyLine();
			poly.left.point = obstacle.getLeftCorner(myPoint);
			poly.right.point = obstacle.getRightCorner(myPoint);
			if(poly.left.point.x != poly.right.point.x && poly.left.point.y != poly.right.point.y)
			{
				var poly2:SortablePolyLine = new SortablePolyLine();
				if(poly.left.point.y < poly.right.point.y)
				{
					poly2.left.point = new Point(poly.left.point.x, poly.right.point.y);
				}
				else
				{
					poly2.left.point = new Point(poly.right.point.x, poly.left.point.y);
				}
				poly2.right.point = poly.right.point;
				poly.right.point = poly2.left.point;

				sortedObstacles.push(poly2.left);
				sortedObstacles.push(poly2.right);
			}
			sortedObstacles.push(poly.left);
			sortedObstacles.push(poly.right);
		}

		// Sort and initialize the stack
		sortedObstacles.sort(this.compareObstacles);
		var openLines:PolyLineStack = new PolyLineStack(myPoint);
		var tmpCalc:Point = new Point(0,0);
		var front:SortablePolyLine;
				
		// Draw the light.
		lightLayer.graphics.lineStyle(3, 0x0000ff, 3);
		lightLayer.graphics.moveTo(myPoint.x, myPoint.y);
		//lightLayer.graphics.moveTo(sortedObstacles[0].point.x, sortedObstacles[0].point.y);
		lightLayer.graphics.beginFill(0xaa77aa);
		for(point in sortedObstacles)
		{
			front = openLines.getFrontMost();
			if(point.isLeft) // the begining of a line
			{
				if(front != null)
				{
					if(front.left.point.lengthSquared(myPoint) > 
						point.point.lengthSquared(myPoint))
					{
						// The new point is in front, trace to previous
						if(front.left.point.x == front.right.point.x)
						{
							tmpCalc.xIntersectionLocal(front.left.point.x, myPoint, point.point);
						}
						else
						{
							tmpCalc.yIntersectionLocal(front.left.point.y, myPoint, point.point);
						}
						lightLayer.graphics.lineTo(tmpCalc.x, tmpCalc.y);
						lightLayer.graphics.lineTo(point.point.x, point.point.y);
					}
					else
					{
						// The new point is behind
					}
				}
				else
				{
					// Trace to the edge and 
					var dir2:Vector = point.point.subtract(myPoint).normalizeLocal();
					if(dir2.x < 0)
					{
						tmpCalc.xIntersectionLocal(0, myPoint, point.point);
					}
					else
					{
						tmpCalc.xIntersectionLocal(flash.Lib.current.stage.stageWidth, myPoint, point.point);
					}
					lightLayer.graphics.lineTo(tmpCalc.x, tmpCalc.y);
					lightLayer.graphics.lineTo(point.point.x, point.point.y);
				}
				openLines.insert(point.parent);
			}
			else // a line is ending
			{
				if(front == point.parent)
				{
					lightLayer.graphics.lineTo(point.point.x, point.point.y);
					openLines.remove(point.parent);
					
					// Trace to the next
					front = openLines.getFrontMost();
					if(front != null)
					{
						if(front.left.point.x == front.right.point.x)
						{
							tmpCalc.xIntersectionLocal(front.left.point.x, myPoint, point.point);
						}
						else
						{
							tmpCalc.yIntersectionLocal(front.left.point.y, myPoint, point.point);
						}
						lightLayer.graphics.lineTo(tmpCalc.x, tmpCalc.y);
					}
					else
					{
						// Trace to the edge and 
						var dir2:Vector = point.point.subtract(myPoint).normalizeLocal().multiplyLocal(1000);
						lightLayer.graphics.lineTo(myPoint.x+dir2.x, myPoint.y+dir2.y);
					}

				}
				else
				{
					// The poly-line must have been behind something
				}
				
			}
		}
		lightLayer.graphics.endFill();
		
		
		/*
			var dir1:Vector = left.subtract(myPoint).normalizeLocal().multiplyLocal(1000);
			var dir2:Vector = right.subtract(myPoint).normalizeLocal().multiplyLocal(1000);
		
			myShadow.graphics.beginFill(color);
			myShadow.graphics.moveTo(left.x,left.y);
			myShadow.graphics.lineTo(left.x+dir1.x,left.y+dir1.y);
			myShadow.graphics.lineTo(right.x+dir2.x,right.y+dir2.y);
			myShadow.graphics.lineTo(right.x,right.y);
			myShadow.graphics.endFill();
		*/
	}
	
	public function compareObstacles(o1:SortablePolyPoint, o2:SortablePolyPoint):Int
	{
		// The division is just above the light
		/*
		if(o1.point.y > myPoint.y && o1.point.y > myPoint.y &&
			((o1.point.x > myPoint.x) != (o1.point.x > myPoint.x))
		{
			return o1.point.x - o2.point.x;
		}
		*/
		return o2.point.leftOfRaw(myPoint, o1.point);
	}
}

