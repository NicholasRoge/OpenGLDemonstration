#ifndef GEOMETRY_POINT_H
#define GEOMETRY_POINT_H


namespace Geometry
{
	struct Point
	{
		double x = 0;
		double y = 0;
		double z = 0;
	};
}

bool operator ==(const Geometry::Point& a,const Geometry::Point& b);

bool operator !=(const Geometry::Point& a,const Geometry::Point& b);

#endif