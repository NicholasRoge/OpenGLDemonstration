#include "Path.h"

using Geometry::Path;
using Geometry::Point;


/* Type [Geometry::Path] Definition */
Path::Path()
{
	this->closed = false;
}

Path& Path::addPoint(Point& point)
{
	if(this->isClosed())
	{
		throw "This path has been closed and may no longer be modified.";
	}

	this->points.push_back(point);

	return *this;
}

Path& Path::addPoint(double x,double y,double z)
{
	Point point;


	point.x = x;
	point.y = y;
	point.z = z;
	return this->addPoint(point);
}

Path::iterator Path::begin()
{
	return this->points.begin();
}

Path::const_iterator Path::begin() const
{
	return this->points.begin();
}

Path::const_iterator Path::cbegin()
{
	return this->points.cbegin();
}

Path::iterator Path::end()
{
	return this->points.end();
}

Path::const_iterator Path::end() const
{
	return this->points.end();
}

Path::const_iterator Path::cend()
{
	return this->points.cend();
}

Path& Path::close()
{
	if(this->points.size() < 3 || this->isClosed())
	{
		throw "Invalid Call";
	}

	if(this->points[0] != this->points[this->points.size() - 1])
	{
		this->points.push_back(this->points[0]);
	}

	this->closed = true;

	return *this;
}

Point Path::getPoint(size_t offset) const
{
	return this->points[offset];
}

bool Path::isClosed() const
{
	return this->closed;
}

size_t Path::size() const
{
	return this->points.size();
}