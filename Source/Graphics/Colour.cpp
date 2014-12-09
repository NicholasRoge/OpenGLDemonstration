#include "Colour.h"

using Graphics::Colour;


/* Type [Graphics::Colour] Definition */
Colour Colour::FromRGB(byte red,byte green,byte blue,byte alpha)
{
	Colour colour;


	colour.red = red;
	colour.green = green;
	colour.blue = blue;
	colour.alpha = alpha;

	return colour;
}