#include "Shader.h"
#include "PathRenderer.h"
#include "../Window.h"

#define PI 3.1415926535897932384626433832795

using Geometry::Path;
using Geometry::Point;
using OpenGL::FragmentShader;
using OpenGL::GeometryShader;
using OpenGL::VertexShader;


/* Declarations */
struct PathSegmentData
{
	GLfloat colour[4];
	GLfloat width;
	GLfloat position[2];
	GLfloat angle;
};

namespace OpenGL
{
	namespace PathRenderer
	{
		GLfloat GetAngle(Point angle_point,Point previous,Point next);

		void InitBuffers();

		void InitProgram();
	}
}

/* Definitions */
namespace OpenGL
{
	namespace PathRenderer
	{
		GLuint array_handle;
		GLuint buffer_handle;
		GLuint program_handle;
		GLuint total_vertex_count;
		GLuint uniform_translate;
		GLuint uniform_window_height;
		GLuint uniform_window_width;

		FragmentShader* fragment_shader;
		GeometryShader* geometry_shader;
		VertexShader* vertex_shader;

		void DrawPath(Geometry::Path& path,GLfloat width,Graphics::Colour colour)
		{
			PathSegmentData data = {{colour.red,colour.green,colour.blue,colour.alpha},width,{0,0},0};
			unsigned int first_offset = total_vertex_count;
			Point point;
			unsigned int vertex_count = path.size();


			glBindBuffer(GL_ARRAY_BUFFER,buffer_handle);
				/* Take care of the first point */
				point = path.getPoint(0);
				data.position[0] = point.x;
				data.position[1] = point.y;
				data.angle = path.isClosed() ? GetAngle(point,path.getPoint(path.size() - 2),path.getPoint(1)) : PI / 2;
				glBufferSubData(GL_ARRAY_BUFFER,total_vertex_count * sizeof(data),sizeof(data),&data);
				++total_vertex_count;

				/* Take care of all of the points that aren't the first or last point. */
				for(size_t offset = 1;offset < path.size() - 1;++offset)
				{
					point = path.getPoint(offset);
					data.position[0] = point.x;
					data.position[1] = point.y;
					data.angle = GetAngle(point,path.getPoint(offset - 1),path.getPoint(offset + 1));
					glBufferSubData(GL_ARRAY_BUFFER,total_vertex_count * sizeof(data),sizeof(data),&data);
					++total_vertex_count;
				}

				/* Take care of the last point */
				point = path.getPoint(path.size() - 1);
				data.position[0] = point.x;
				data.position[1] = point.y;
				data.angle = path.isClosed() ? GetAngle(point,path.getPoint(path.size() - 2),path.getPoint(1)) : PI / 2;
				glBufferSubData(GL_ARRAY_BUFFER,total_vertex_count * sizeof(data),sizeof(data),&data);
				++total_vertex_count;
			glBindBuffer(GL_ARRAY_BUFFER,0);


			Point translate = OpenGL::GetOrigin();
			OpenGL::AddRenderTask([first_offset,vertex_count,translate](){
				glUseProgram(program_handle);
				glBindVertexArray(array_handle);
				glBindBuffer(GL_ARRAY_BUFFER,buffer_handle);
					glUniform2f(uniform_translate,translate.x,translate.y);
					glDrawArrays(GL_LINE_STRIP,first_offset,vertex_count);
				glBindBuffer(GL_ARRAY_BUFFER,0);
				glBindVertexArray(0);
				glUseProgram(0);

				total_vertex_count -= vertex_count;
			});
		}

		GLfloat GetAngle(Point angle_point,Point previous,Point next)
		{
			GLfloat angle;
			GLfloat sign;


			previous.x -= angle_point.x;
			previous.y -= angle_point.y;
			next.x -= angle_point.x;
			next.y -= angle_point.y;

			angle = acos(((previous.x * next.x) + (previous.y * next.y)) / (sqrt(previous.x * previous.x + previous.y * previous.y) * sqrt(next.x * next.x + next.y * next.y))) / 2;
			sign = ((previous.x * next.x) + (previous.y * next.y)) - ((previous.y * next.x) + (-previous.x * next.y));
			sign = sign / abs(sign);

			return angle * sign;
		}

		void InitBuffers()
		{
			glGenBuffers(1,&buffer_handle);
			glBindBuffer(GL_ARRAY_BUFFER,buffer_handle);
			glBufferData(GL_ARRAY_BUFFER,1024 * sizeof(PathSegmentData),nullptr,GL_STREAM_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER,0);
		}

		void InitProgram()
		{
			GLint attribute;
		

			/* Obtain a new program handle. */
			program_handle = glCreateProgram();
		
			/* Compile the shaders. */
			vertex_shader = new VertexShader();
			vertex_shader->setSourceFile("./Resource/Shader/PathRenderer.vs.glsl",true);
		
			geometry_shader = new GeometryShader();
			geometry_shader->setSourceFile("./Resource/Shader/PathRenderer.gs.glsl",true);

			fragment_shader = new FragmentShader();
			fragment_shader->setSourceFile("./Resource/Shader/PathRenderer.fs.glsl",true);

			/* Attach all the shaders to the program. */
			glAttachShader(program_handle,*vertex_shader);
			glAttachShader(program_handle,*geometry_shader);
			glAttachShader(program_handle,*fragment_shader);
		
			/* Link the shaders into the program. */
			glLinkProgram(program_handle);

			/* Create a new vertex array to store the attribute bindings in. */
			glGenVertexArrays(1,&array_handle);

			/* Obtain a handle to the shader variables. */
			glUseProgram(program_handle);
			glBindVertexArray(array_handle);
			glBindBuffer(GL_ARRAY_BUFFER,buffer_handle);

				uniform_window_width = glGetUniformLocation(program_handle,"window_width");
				uniform_window_height = glGetUniformLocation(program_handle,"window_height");
				uniform_translate = glGetUniformLocation(program_handle,"translate");

				attribute = glGetAttribLocation(program_handle,"colour");
				glVertexAttribPointer(attribute,4,GL_FLOAT,GL_FALSE,sizeof(PathSegmentData),(void*)offsetof(PathSegmentData,colour));
				glEnableVertexAttribArray(attribute);

				attribute = glGetAttribLocation(program_handle,"width");
				glVertexAttribPointer(attribute,1,GL_FLOAT,GL_FALSE,sizeof(PathSegmentData),(void*)offsetof(PathSegmentData,width));
				glEnableVertexAttribArray(attribute);

				attribute = glGetAttribLocation(program_handle,"position");
				glVertexAttribPointer(attribute,2,GL_FLOAT,GL_FALSE,sizeof(PathSegmentData),(void*)offsetof(PathSegmentData,position));
				glEnableVertexAttribArray(attribute);

				attribute = glGetAttribLocation(program_handle,"angle");
				glVertexAttribPointer(attribute,2,GL_FLOAT,GL_FALSE,sizeof(PathSegmentData),(void*)offsetof(PathSegmentData,angle));
				glEnableVertexAttribArray(attribute);

			glBindBuffer(GL_ARRAY_BUFFER,0);
			glBindVertexArray(0);
			glUseProgram(0);
		}

		void Load()
		{
			InitBuffers();
			InitProgram();
		}

		void OnBeforeRender()
		{
			glUseProgram(program_handle);
			glUniform1ui(uniform_window_width,(GLuint)Window::width);
			glUniform1ui(uniform_window_height,(GLuint)Window::height);
			glUseProgram(0);
		}

		void OnAfterRender()
		{

		}
	
		void Unload()
		{
			delete fragment_shader;
			delete geometry_shader;
			delete vertex_shader;
		}
	}
}