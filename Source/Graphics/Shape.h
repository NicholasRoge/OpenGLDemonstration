#ifndef GRAPHICS_SHAPE_H
#define GRAPHICS_SHAPE_H

#include "Colour.h"
#include "Drawable.h"
#include "../Geometry/Path.h"


namespace Graphics
{
	class Shape : public Drawable
	{
		private:
			Colour fill_colour;

		public:
			Shape();

			virtual void draw();

			virtual Colour getFill() const;

			virtual Geometry::Path getPath() const = 0;

			virtual void setFill(Colour colour);
	};
}

#endif