#include "Element.h"
#include "ElementContainer.h"
#include "../OpenGL.h"
#include "../Geometry/Path.h"

using Geometry::Point;
using Graphics::Colour;
using Graphics::Drawable;
using UI::Element;


/* Type [UI::Element] Definition */
Element::Element()
{
	this->parent = nullptr;

	this->height = 0;
	this->width = 0;
	this->x = 0;
	this->y = 0;
	this->background.drawable = nullptr;
	this->background.colour = Colour::FromRGB(1,1,1,0);
}

Element::~Element()
{
}

void Element::draw()
{
	OpenGL::TranslateOrigin(this->x,this->y);

	if(this->background.colour.alpha != 0)
	{
		OpenGL::FillRectangle(0,0,this->width,this->height,this->background.colour);
	}
	if(this->background.drawable != nullptr)
	{
		this->background.drawable->draw();
	}
	this->onDraw();

	OpenGL::TranslateOrigin(-this->x,-this->y);
}

double Element::inner_height() const
{
	return this->height;
}

double Element::inner_width() const
{
	return this->width;
}

void Element::setBackgroundColour(Colour colour)
{
	this->background.colour = colour;
}

void Element::onDraw()
{
}