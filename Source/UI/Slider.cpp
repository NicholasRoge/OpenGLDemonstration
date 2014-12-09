#include "Slider.h"
#include "../OpenGL.h"
#include "../Geometry/Path.h"

using Geometry::Path;
using Geometry::Point;
using Graphics::Colour;
using UI::Slider;


/* Type [UI::Element] Definition */
Slider::Slider()
{
	this->height = 16;
	this->width = 100;

	this->value = 0;

	this->selector_colour = Colour::FromRGB(96,96,96);
	this->track_colour = Colour::FromRGB(0,0,0);
}

void Slider::onDraw()
{
	Geometry::Path bar;
	Geometry::Path track;


	this->Element::onDraw();

	track
		.addPoint(2,this->height / 2,0)
		.addPoint(this->width - 2,this->height / 2,0);

	bar
		.addPoint(value * (this->width - 4),0,0)
		.addPoint(value * (this->width - 4) + 4,0,0)
		.addPoint(value * (this->width - 4) + 4,this->height - 2,0)
		.addPoint(value * (this->width - 4) + 2,this->height,0)
		.addPoint(value * (this->width - 4),this->height - 2,0)
		.close();

	OpenGL::DrawPath(track,(3.0/16.0) * this->height,this->track_colour);
	OpenGL::FillPath(bar,this->selector_colour);
}