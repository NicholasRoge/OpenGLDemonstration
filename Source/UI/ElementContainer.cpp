#include "ElementContainer.h"
#include "../OpenGL.h"

using Geometry::Path;
using Graphics::Colour;
using UI::ElementContainer;


/* Type [UI::ElementContainer] Definition */
ElementContainer::ElementContainer()
{
	this->padding = 0;

	this->border.size = 0;
	this->border.colour = Colour::FromRGB(0,0,0);
}

void ElementContainer::addChild(Element* child)
{
	this->children.push_back(child);
	child->parent = this;
}

void ElementContainer::click(double x,double y)
{
	for(auto& child : this->children)
	{
		if(x >= child->x && x <= child->x + child->width && y >= child->y && y <= child->y + child->width)
		{
			child->click(x - ((this->width - this->inner_width()) / 2) - child->x,y - ((this->height - this->inner_height()) / 2) - child->y);
		}
	}
}

void ElementContainer::onDraw()
{
	this->Element::onDraw();
	
	if(border.size > 0)
	{
		OpenGL::DrawRectangle(0,0,this->width,this->height,this->border.size,this->border.colour);
	}

	OpenGL::TranslateOrigin(this->border.size + this->padding,this->border.size + this->padding);
	for(Element* child : this->children)
	{
		child->draw();
	}
	OpenGL::TranslateOrigin(-(this->border.size + this->padding),-(this->border.size + this->padding));
}

double ElementContainer::inner_height() const
{
	return this->Element::inner_height() - (this->border.size * 2) - (this->padding * 2);
}

double ElementContainer::inner_width() const
{
	return this->Element::inner_width() - (this->border.size * 2) - (this->padding * 2);
}