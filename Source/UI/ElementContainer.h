#ifndef UI_ELEMENTCONTAINER_H
#define UI_ELEMENTCONTAINER_H

#include "Element.h"
#include <vector>


namespace UI
{
	class ElementContainer : public Element
	{
		private:
			std::vector<Element*> children;

		/* Member Properties */
		public:
			struct
			{
				Graphics::Colour colour;
				double size;
			}border;

			double padding;

			virtual double inner_height() const;

			virtual double inner_width() const;

		/* Member Methods */
		protected:
			virtual void onDraw();

		public:
			ElementContainer();

			virtual void addChild(Element* child);

			virtual void click(double x,double y);
	};
}

#endif