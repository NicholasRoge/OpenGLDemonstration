#ifndef UI_SLIDER_H
#define UI_SLIDER_H

#include "Element.h"
#include "../Graphics/Colour.h"
#include "../Geometry/Point.h"


namespace UI
{
	class Slider : public Element
	{
		public:
			Graphics::Colour selector_colour;
			Graphics::Colour track_colour;
			float value;

		private:
			void onClick(double x,double y);

		protected:
			virtual void onDraw();

		public:
			Slider();
	};
}

#endif