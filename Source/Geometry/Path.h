#ifndef GEOMETRY_PATH_H
#define GEOMETRY_PATH_H

#include "Point.h"
#include <vector>


namespace Geometry
{
	class Path
	{
		public:
			typedef std::vector<Point>::iterator iterator;
			typedef std::vector<Point>::const_iterator const_iterator;

		private:
			bool closed;
			std::vector<Point> points;

		public:
			Path();

			Path& addPoint(Point& point);

			Path& addPoint(double x,double y,double z);

			iterator begin();

			const_iterator begin() const;

			const_iterator cbegin();

			iterator end();

			const_iterator end() const;

			const_iterator cend();

			Path& close();

			Point getPoint(size_t offset) const;

			bool isClosed() const;

			size_t size() const;
	};
}

#endif