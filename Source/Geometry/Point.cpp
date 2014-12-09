#include "Point.h"

bool operator ==(const Geometry::Point& a,const Geometry::Point& b)
{
	return a.x == b.x && a.y == b.y && a.z == b.z;
}

bool operator !=(const Geometry::Point& a,const Geometry::Point& b)
{
	return !(a == b);
}