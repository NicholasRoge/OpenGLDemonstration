#include "Label.h"
#include "../OpenGL.h"

using Graphics::Colour;
using UI::Label;


/* Type [UI::Element] Definition */
Label::Label()
{
	this->font = "CourierNew18";
	this->font_colour = Colour::FromRGB(0,0,0);
}

void Label::onDraw()
{
	this->Element::onDraw();

	if(this->text != "")
	{
		OpenGL::DrawText(this->text);
	}
}

void Label::setFont(const char* font)
{
	this->font = font;
}

void Label::setFont(const std::string& font)
{
	this->setFont(font.c_str());
}

void Label::setFontColour(Colour colour)
{
	this->font_colour = colour;
}

void Label::setText(const char* source)
{
	this->text = source;
}

void Label::setText(const std::string& source)
{
	this->setText(source.c_str());
}