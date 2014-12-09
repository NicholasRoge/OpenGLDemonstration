#version 150 core


in vec2 uv;
in vec2 position;

out vec2 f_uv;

uniform vec2 translate;
uniform uint window_height;
uniform uint window_width;

void main()
{
	f_uv = uv;
	gl_Position = vec4(position + translate,0,1);
	gl_Position *= mat4(1/float(window_width),0,0,0 , 0,1/float(window_height),0,0 , 0,0,1,0 , 0,0,0,1);
	gl_Position += vec4(-0.5,-0.5,0,0);
	gl_Position *= mat4(2,0,0,0 , 0,-2,0,0 , 0,0,1,0 , 0,0,0,1);
}