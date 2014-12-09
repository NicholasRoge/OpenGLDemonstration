#ifndef GRAPHICS_IMAGE_H
#define GRAPHICS_IMAGE_H

#include "Colour.h"
#include "Drawable.h"
#include <stdint.h>
#include <string>


namespace Graphics
{
	class Image
	{
		private:
			Colour* data;
			size_t data_length;
			int32_t height;
			int32_t width;

		public:
			Image();

			~Image();

			const Colour* getData() const;

			int32_t getHeight() const;
			
			int32_t getWidth() const;

			bool isValid() const;

			void setSourceFile(const char* path);

			void setSourceFile(const std::string& path);
	};
}

#endif