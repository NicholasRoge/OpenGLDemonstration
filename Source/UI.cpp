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
		root->height = 106;
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
		slider_red->x = 0;
		slider_red->y = 0;
		slider_red->selector_colour = Colour::FromRGB(theme_colour.red,0,0);
		slider_red->track_colour = Colour::FromRGB(100,100,100);
		slider_red->value = (float)theme_colour.red / 255.0;
		content_container->addChild(slider_red);

		textbox_red = new ElementContainer();
		textbox_red->width = 64;
		textbox_red->height = 16;
		textbox_red->x = slider_red->x + slider_red->width + 6;
		textbox_red->y = slider_red->y;
		textbox_red->border.size = 2;
		textbox_red->border.colour = Colour::FromRGB(100,100,100);
		content_container->addChild(textbox_red);

		slider_green = new Slider();
		slider_green->x = 0;
		slider_green->y = slider_red->height + 6;
		slider_green->selector_colour = Colour::FromRGB(0,theme_colour.green,0);
		slider_green->track_colour = Colour::FromRGB(100,100,100);
		slider_green->value = (float)theme_colour.green / 255.0;
		content_container->addChild(slider_green);

		textbox_green = new ElementContainer();
		textbox_green->width = 64;
		textbox_green->height = 16;
		textbox_green->x = slider_green->x + slider_green->width + 6;
		textbox_green->y = slider_green->y;
		textbox_green->border.size = 2;
		textbox_green->border.colour = Colour::FromRGB(100,100,100);
		content_container->addChild(textbox_green);

		slider_blue = new Slider();
		slider_blue->x = 0;
		slider_blue->y = slider_red->height + 6 + slider_green->height + 6;
		slider_blue->selector_colour = Colour::FromRGB(0,0,theme_colour.blue);
		slider_blue->track_colour = Colour::FromRGB(100,100,100);
		slider_blue->value = (float)theme_colour.blue / 255.0;
		content_container->addChild(slider_blue);

		textbox_blue = new ElementContainer();
		textbox_blue->width = 64;
		textbox_blue->height = 16;
		textbox_blue->x = slider_blue->x + slider_blue->width + 6;
		textbox_blue->y = slider_blue->y;
		textbox_blue->border.size = 2;
		textbox_blue->border.colour = Colour::FromRGB(100,100,100);
		content_container->addChild(textbox_blue);
	}

	void Unload()
	{
		delete textbox_blue;
		delete slider_blue;
		delete textbox_green;
		delete slider_green;
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
		root->border.colour = theme_colour;

		titlebar->setBackgroundColour(theme_colour);

		slider_red->selector_colour.red = theme_colour.red;
		slider_red->value = (float)theme_colour.red / 255.0;

		slider_green->selector_colour.green = theme_colour.green;
		slider_green->value = (float)theme_colour.green / 255.0;

		slider_blue->selector_colour.blue = theme_colour.blue;
		slider_blue->value = (float)theme_colour.blue / 255.0;
	}
}