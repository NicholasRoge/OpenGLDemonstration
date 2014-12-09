#ifndef GRAPHICS_COLOUR_H
#define GRAPHICS_COLOUR_H

#include <stdint.h>

typedef uint8_t byte;


namespace Graphics
{
	struct Colour
	{
		static Colour FromRGB(byte red,byte green,byte blue,byte alpha = 255);

		byte red;
		byte green;
		byte blue;
		byte alpha;
	};
}

#endif