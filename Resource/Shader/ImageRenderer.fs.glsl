#version 150 core


in vec2 f_uv;

out vec4 fragment_colour;

uniform sampler2D image;

void main()
{
	fragment_colour = texture(image,vec2(f_uv.x,1 - f_uv.y));
}