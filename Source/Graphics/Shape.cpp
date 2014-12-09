#include "../OpenGL.h"
#include "Shape.h"

using Graphics::Colour;
using Graphics::Shape;


/* Type [Graphics::Shape] Definition */
Shape::Shape()
{
	this->fill_colour = Colour::FromRGB(1,1,1);
}

void Shape::draw()
{
	OpenGL::FillPath(this->getPath(),this->fill_colour);
}

Colour Shape::getFill() const
{
	return this->fill_colour;
}

void Shape::setFill(Colour colour)
{
	this->fill_colour = colour;
}