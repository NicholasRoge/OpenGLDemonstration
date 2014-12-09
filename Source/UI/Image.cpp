#include "Image.h"
#include "../OpenGL.h"

using UI::Image;


/* Type [UI::Element] Definition */
Image::Image()
{
}

void Image::onDraw()
{
	this->Element::onDraw();
	
	if(this->image.isValid())
	{
		OpenGL::DrawImage(this->image);
	}
}

void Image::setSourceFile(const char* source)
{
	this->image.setSourceFile(source);	
}

void Image::setSourceFile(const std::string& source)
{
	this->setSourceFile(source.c_str());
}