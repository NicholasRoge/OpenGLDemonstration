#ifndef UI_ELEMENT_H
#define UI_ELEMENT_H

#include <functional>
#include "../Graphics/Colour.h"
#include "../Graphics/Drawable.h"
#include "../Geometry/Point.h"


namespace UI
{
	class ElementContainer;

	class Element
	{
		/* Member Properties */
		public:
			struct
			{
				Graphics::Colour colour;
				Graphics::Drawable* drawable;
			} background;

			double height;
			ElementContainer* parent;
			std::function<void(double,double)> on_click;
			double width;
			double x;
			double y;

			virtual double inner_height() const;

			virtual double inner_width() const;

		/* Member Methods */
		protected:
			/* Callbacks */
			virtual void onDraw();

		public:
			Element();

			virtual ~Element();

			virtual void click(double x,double y);

			virtual void draw() final;

			Element* getRootElement();

			const Element* getRootElement() const;

			bool isClickable() const;

			virtual void setBackgroundColour(Graphics::Colour colour);

			//virtual void setBackgroundDrawable(Graphics::Drawable* drawable);
	};
}

#endif