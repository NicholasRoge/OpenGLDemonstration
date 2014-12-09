#ifndef UI_LABEL_H
#define UI_LABEL_H

#include "Element.h"
#include "../Graphics/Colour.h"
#include <string>

namespace UI
{
	class Label : public Element
	{
		private:
			std::string font;
			Graphics::Colour font_colour;
			std::string text;

		protected:
			virtual void onDraw();

		public:
			Label();

			virtual void setFont(const char* text);

			virtual void setFont(const std::string& text);

			virtual void setFontColour(Graphics::Colour colour);

			virtual void setText(const char* text);

			virtual void setText(const std::string& text);
	};
}

#endif