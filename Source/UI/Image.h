#ifndef UI_IMAGE_H
#define UI_IMAGE_H

#include "Element.h"
#include "../Graphics/Image.h"
#include <string>

namespace UI
{
	class Image : public Element
	{
		private:
			Graphics::Image image;

		protected:
			virtual void onDraw();

		public:
			Image();

			virtual void setSourceFile(const char* path);

			virtual void setSourceFile(const std::string& path);
	};
}

#endif