#version 150 core


in float angle;
in vec4 colour;
in float width;
in vec2 position;

out float v_angle;
out vec4 v_colour;
out float v_width;

void main()
{
	v_angle = angle;
	v_colour = colour;
	v_width = width;
	gl_Position = vec4(position,0,1);
}