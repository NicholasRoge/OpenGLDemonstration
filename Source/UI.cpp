#include "UI/Element.h"
#include "UI/ElementContainer.h"
#include "UI/Image.h"
#include "UI/Label.h"
#include "UI/Slider.h"
#include "UI.h"
#include "Window.h"

#include "Geometry\Path.h"
#include "OpenGL.h"

using Graphics::Colour;
using UI::Element;

namespace UI
{
	void UpdateColours();
}

namespace UI
{
	Colour theme_colour;

	ElementContainer* content_container;
	Image* close_icon;
	Label* label_red;
	Label* label_green;
	Label* label_blue;
	Image* menu_icon;
	ElementContainer* root;
	Slider* slider_red;
	Slider* slider_green;
	Slider* slider_blue;
	ElementContainer* textbox_red;
	ElementContainer* textbox_green;
	ElementContainer* textbox_blue;
	Label* title;
	ElementContainer* titlebar;

	/* Function Definitions */
	void DelegateClick(double x,double y)
	{
		if(x >= root->x && x <= root->x + root->width)
		{
			root->click(x - root->x,y - root->y);
		}
	}

	void Draw()
	{
		UpdateColours();

		root->draw();
	}

	void Load()
	{
		theme_colour = Colour::FromRGB(255,69,69);


		root = new ElementContainer();
		root->width = 184;
		root->height = 130;
		root->x = (Window::width / 2) - (root->width / 2);
		root->y = (Window::height / 2) - (root->height / 2);
		root->setBackgroundColour(Colour::FromRGB(255,254,254));
		root->border.size = 1;
		root->border.colour = theme_colour;

		titlebar = new ElementContainer();
		titlebar->width = root->inner_width();
		titlebar->height = 30;
		titlebar->x = 0;
		titlebar->y = 0;
		titlebar->setBackgroundColour(theme_colour);
		titlebar->border.size = 0;
		root->addChild(titlebar);

		menu_icon = new Image();
		menu_icon->width = 30;
		menu_icon->height = 30;
		menu_icon->setSourceFile("./Resource/Icon/Menu.bmp");
		titlebar->addChild(menu_icon);

		close_icon = new Image();
		close_icon->width = 30;
		close_icon->height = 30;
		close_icon->x = titlebar->inner_width() - close_icon->width;
		close_icon->y = 0;
		close_icon->setSourceFile("./Resource/Icon/Close.bmp");
		close_icon->on_click = [](double x,double y)
		{
			Window::Close();
		};
		titlebar->addChild(close_icon);

		title = new Label();
		title->width = titlebar->inner_width() - menu_icon->width - close_icon->width;
		title->height = 18;
		title->x = menu_icon->x + menu_icon->width;
		title->y = 6;
		title->setText("GL Window");
		titlebar->addChild(title);

		content_container = new ElementContainer();
		content_container->width = root->inner_width();
		content_container->height = root->inner_height() - titlebar->height;
		content_container->x = 0;
		content_container->y = titlebar->height;
		content_container->padding = 6;
		content_container->border.size = 0;
		root->addChild(content_container);

		slider_red = new Slider();
		slider_red->height = 24;
		slider_red->x = 0;
		slider_red->y = 0;
		slider_red->selector_colour = Colour::FromRGB(theme_colour.red,0,0);
		slider_red->track_colour = Colour::FromRGB(100,100,100);
		slider_red->value = (float)theme_colour.red / 255.0;
		content_container->addChild(slider_red);

		textbox_red = new ElementContainer();
		textbox_red->width = 64;
		textbox_red->height = 24;
		textbox_red->x = slider_red->x + slider_red->width + 6;
		textbox_red->y = slider_red->y;
		textbox_red->padding = 2;
		textbox_red->border.size = 2;
		textbox_red->border.colour = Colour::FromRGB(100,100,100);
		content_container->addChild(textbox_red);

		label_red = new Label();
		label_red->width = textbox_red->inner_width();
		label_red->height = textbox_red->inner_height();
		label_red->setText("255");
		textbox_red->addChild(label_red);

		slider_green = new Slider();
		slider_green->height = 24;
		slider_green->x = 0;
		slider_green->y = slider_red->height + 6;
		slider_green->selector_colour = Colour::FromRGB(0,theme_colour.green,0);
		slider_green->track_colour = Colour::FromRGB(100,100,100);
		slider_green->value = (float)theme_colour.green / 255.0;
		content_container->addChild(slider_green);

		textbox_green = new ElementContainer();
		textbox_green->width = 64;
		textbox_green->height = 24;
		textbox_green->x = slider_green->x + slider_green->width + 6;
		textbox_green->y = slider_green->y;
		textbox_green->padding = 2;
		textbox_green->border.size = 2;
		textbox_green->border.colour = Colour::FromRGB(100,100,100);
		content_container->addChild(textbox_green);

		label_green = new Label();
		label_green->width = textbox_green->inner_width();
		label_green->height = textbox_green->inner_height();
		label_green->setText("69");
		textbox_green->addChild(label_green);

		slider_blue = new Slider();
		slider_blue->height = 24;
		slider_blue->x = 0;
		slider_blue->y = slider_red->height + 6 + slider_green->height + 6;
		slider_blue->selector_colour = Colour::FromRGB(0,0,theme_colour.blue);
		slider_blue->track_colour = Colour::FromRGB(100,100,100);
		slider_blue->value = (float)theme_colour.blue / 255.0;
		content_container->addChild(slider_blue);

		textbox_blue = new ElementContainer();
		textbox_blue->width = 64;
		textbox_blue->height = 24;
		textbox_blue->x = slider_blue->x + slider_blue->width + 6;
		textbox_blue->y = slider_blue->y;
		textbox_blue->padding = 2;
		textbox_blue->border.size = 2;
		textbox_blue->border.colour = Colour::FromRGB(100,100,100);
		content_container->addChild(textbox_blue);

		label_blue = new Label();
		label_blue->width = textbox_blue->inner_width();
		label_blue->height = textbox_blue->inner_height();
		label_blue->setText("69");
		textbox_blue->addChild(label_blue);
	}

	void Unload()
	{
		delete label_blue;
		delete textbox_blue;
		delete slider_blue;
		delete label_green;
		delete textbox_green;
		delete slider_green;
		delete label_red;
		delete textbox_red;
		delete slider_red;
		delete content_container;
		delete close_icon;
		delete menu_icon;
		delete titlebar;
		delete root;
	}

	void UpdateColours()
	{
		static char buffer[4];
		//static byte delta_red = 1;
		//static byte delta_green = 0;
		//static byte delta_blue = 0;

		///* Update the colour itself */
		//if(delta_red == 1)
		//{
		//	theme_colour.red += delta_red;
		//	if(theme_colour.red == 69)
		//	{
		//		delta_red = 0;
		//		delta_green = 1;
		//	}
		//}
		//else if(delta_green == 1)
		//{
		//	theme_colour.green += delta_green;
		//	if(theme_colour.green == 69)
		//	{
		//		delta_green = 0;
		//		delta_blue = 1;
		//	}
		//}
		//else if(delta_blue == 1)
		//{
		//	theme_colour.blue += delta_blue;
		//	if(theme_colour.blue == 69)
		//	{
		//		delta_blue = 0;
		//		delta_red = 1;
		//	}
		//}

		/* Update the colours themselves. */
		theme_colour.red = (byte)(slider_red->value * 255);
		theme_colour.green = (byte)(slider_green->value * 255);
		theme_colour.blue = (byte)(slider_blue->value * 255);

		/* Update the colours in the elements. */
		root->border.colour = theme_colour;

		titlebar->setBackgroundColour(theme_colour);

		slider_red->selector_colour.red = theme_colour.red;
		slider_red->value = (float)theme_colour.red / 255.0;
		label_red->setText(itoa(theme_colour.red,buffer,10));

		slider_green->selector_colour.green = theme_colour.green;
		slider_green->value = (float)theme_colour.green / 255.0;
		label_green->setText(itoa(theme_colour.green,buffer,10));

		slider_blue->selector_colour.blue = theme_colour.blue;
		slider_blue->value = (float)theme_colour.blue / 255.0;
		label_blue->setText(itoa(theme_colour.blue,buffer,10));
	}
}