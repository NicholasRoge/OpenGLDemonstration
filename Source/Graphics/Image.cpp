#include <fstream>
#include "Image.h"
#include "../OpenGL.h"

using Graphics::Image;


/* Function Declarations */
bool IsSystemBigEndian();

template<typename Type>
void SwapByteOrder(Type& value);

/* Function Definitions */
bool IsSystemBigEndian()
{
	uint32_t test = 0x000000FF;


	return ((unsigned char*)&test)[0] == 0;
}

template<typename Type>
void SwapByteOrder(Type& value)
{
	char bytes[sizeof(Type)];


	memcpy(bytes,&value,sizeof(Type));
	for(size_t offset = 0;offset < sizeof(Type);++offset)
	{
		((char*)&value)[offset] = bytes[sizeof(Type) - 1 - offset];
	}
}

/* Type [Graphics::Image] Definition */
Image::Image()
{
	this->data = nullptr;
	this->data_length = 0;
	this->height = -1;
	this->width = -1;
}

Image::~Image()
{
	if(this->data != nullptr)
	{
		delete[] this->data;
	}
}

const Graphics::Colour* Image::getData() const
{
	return this->data;
}

int32_t Image::getHeight() const
{
	return this->height;
}

int32_t Image::getWidth() const
{
	return this->width;
}

bool Image::isValid() const
{
	return this->data != nullptr;
}

void Image::setSourceFile(const char* path)
{
	char buffer[4];
	unsigned char component_alpha;
	unsigned char component_blue;
	unsigned char component_green;
	unsigned char component_red;
	uint32_t data_offset;
	std::ifstream imagestream;


	/* Open the file. */
	imagestream.open(path,std::ios::binary | std::ios::in);
	if(!imagestream.is_open())
	{
		throw std::runtime_error(std::string("Attempt to read image file '").append(path).append("' failed.  Cause:  File could not be opened."));
	}

	/* Read the bitmap's file header and validate it. */
	imagestream.read(buffer,2);
	if(strncmp(buffer,"BM",2) != 0)
	{
		throw std::runtime_error(std::string("Attempt to read image file '").append(path).append("' failed.  Cause:  File does not appear to have a valid bitmap file format."));
	}

	imagestream.seekg(10);
	imagestream.read((char*)&data_offset,4);
	if(IsSystemBigEndian())
	{
		SwapByteOrder(data_offset);
	}

	/* Read the bitmap's info header. */
	imagestream.seekg(18);
	imagestream.read((char*)&this->width,4);
	imagestream.read((char*)&this->height,4);
	if(IsSystemBigEndian())
	{
		SwapByteOrder(this->width);
		SwapByteOrder(this->height);
	}

	/* Get information about how the pixels are stored in the file. */
	uint32_t bitfields;
	uint32_t channel_alpha_mask;
	uint32_t channel_blue_mask;
	uint32_t channel_green_mask;
	uint32_t channel_red_mask;
	uint32_t colour;

	imagestream.seekg(0x1E);
	imagestream.read((char*)&bitfields,4);

	if(bitfields != 0)
	{
		imagestream.seekg(0x36);

		imagestream.read((char*)&channel_red_mask,4);
		imagestream.read((char*)&channel_green_mask,4);
		imagestream.read((char*)&channel_blue_mask,4);
		imagestream.read((char*)&channel_alpha_mask,4);
	}

	/* Read the image data in. */
	imagestream.seekg(data_offset);

	this->data_length = this->width * this->height;
	this->data = new Colour[this->data_length];
	for(size_t offset_y = 0;offset_y < this->height;++offset_y)
	{
		for(size_t offset_x = 0;offset_x < this->width;++offset_x)
		{
			imagestream.read((char*)&component_blue,1);
			imagestream.read((char*)&component_green,1);
			imagestream.read((char*)&component_red,1);
			imagestream.read((char*)&component_alpha,1);

			this->data[(offset_y * this->width) + offset_x] = Colour::FromRGB(component_red,component_green,component_blue,component_alpha);
		}
	}

	/* Clean up. */
	imagestream.close();
}

void Image::setSourceFile(const std::string& path)
{
	this->setSourceFile(path.c_str());
}