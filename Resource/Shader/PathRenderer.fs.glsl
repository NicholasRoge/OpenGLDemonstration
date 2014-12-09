#version 150 core


in vec4 f_colour;

out vec4 fragment_colour;

void main()
{
	fragment_colour = f_colour / 255;
}