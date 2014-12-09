#version 150 core

#define PI 3.1415926535897932384626433832795


layout(lines) in;
layout(triangle_strip,max_vertices = 5) out;
	
in float v_angle[];
in vec4 v_colour[];
in float v_width[];

out vec4 f_colour;

uniform uint window_height;
uniform uint window_width;
uniform vec2 translate;

/* Function Declarations */
vec4 Rotate(vec4 v,float radians);

vec4 Scale(vec4 v,float amount);

/* Function Definitions */
vec4 Rotate(vec4 v,float radians)
{
	return v * mat4(
		cos(radians),-sin(radians),0,0,
		sin(radians),cos(radians),0,0,
		0,0,1,0,
		0,0,0,1
	);
}

vec4 Scale(vec4 v,float amount)
{
	return v * mat4(
		amount,0,0,0,
		0,amount,0,0,
		0,0,1,0,
		0,0,0,1
	);
}

/* Main */
void main()
{
	vec4 ab_normal;
	float angle[2];
	vec4 point[4];


	angle[0] = v_angle[0];
	angle[1] = v_angle[1];

	ab_normal = vec4(gl_in[1].gl_Position.xy - gl_in[0].gl_Position.xy,0,1);
	ab_normal /= length(ab_normal);
			
	if(angle[0] < 0)
	{
		angle[0] = PI + -angle[0];
	}
	point[0] = Scale(Rotate(ab_normal,angle[0]),(v_width[0] / 2) / sin(angle[0]));
	point[1] = -point[0];

	if(angle[1] < 0)
	{
		angle[1] = PI + -angle[1];
	}
	point[2] = Scale(Rotate(ab_normal,-angle[1]),(v_width[1] / 2) / sin(-angle[1]));
	point[3] = -point[2];

	f_colour = v_colour[0];
	gl_Position = vec4(gl_in[0].gl_Position.xy + point[0].xy,0,1);
	gl_Position += vec4(translate,0,0);
	gl_Position *= mat4(1/float(window_width),0,0,0 , 0,1/float(window_height),0,0 , 0,0,1,0 , 0,0,0,1);
	gl_Position += vec4(-0.5,-0.5,0,0);
	gl_Position *= mat4(2,0,0,0 , 0,-2,0,0 , 0,0,1,0 , 0,0,0,1);
	EmitVertex();

	f_colour = v_colour[1];
	gl_Position = vec4(gl_in[1].gl_Position.xy + point[3].xy,0,1);
	gl_Position += vec4(translate,0,0);
	gl_Position *= mat4(1 / float(window_width),0,0,0,0,1 / float(window_height),0,0,0,0,1,0,0,0,0,1);
	gl_Position += vec4(-0.5,-0.5,0,0);
	gl_Position *= mat4(2,0,0,0,0,-2,0,0,0,0,1,0,0,0,0,1);
	EmitVertex();

	f_colour = v_colour[1];
	gl_Position = vec4(gl_in[1].gl_Position.xy + point[2].xy,0,1);
	gl_Position += vec4(translate,0,0);
	gl_Position *= mat4(1 / float(window_width),0,0,0,0,1 / float(window_height),0,0,0,0,1,0,0,0,0,1);
	gl_Position += vec4(-0.5,-0.5,0,0);
	gl_Position *= mat4(2,0,0,0,0,-2,0,0,0,0,1,0,0,0,0,1);
	EmitVertex();

	f_colour = v_colour[0];
	gl_Position = vec4(gl_in[0].gl_Position.xy + point[1].xy,0,1);
	gl_Position += vec4(translate,0,0);
	gl_Position *= mat4(1/float(window_width),0,0,0 , 0,1/float(window_height),0,0 , 0,0,1,0 , 0,0,0,1);
	gl_Position += vec4(-0.5,-0.5,0,0);
	gl_Position *= mat4(2,0,0,0 , 0,-2,0,0 , 0,0,1,0 , 0,0,0,1);
	EmitVertex();

	f_colour = v_colour[0];
	gl_Position = vec4(gl_in[0].gl_Position.xy + point[0].xy,0,1);
	gl_Position += vec4(translate,0,0);
	gl_Position *= mat4(1 / float(window_width),0,0,0,0,1 / float(window_height),0,0,0,0,1,0,0,0,0,1);
	gl_Position += vec4(-0.5,-0.5,0,0);
	gl_Position *= mat4(2,0,0,0,0,-2,0,0,0,0,1,0,0,0,0,1);
	EmitVertex();

	EndPrimitive();
}