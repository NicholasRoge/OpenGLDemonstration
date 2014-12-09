#include "Shader.h"
#include "ShapeRenderer.h"
#include "../Window.h"

using Geometry::Path;
using Geometry::Point;
using OpenGL::FragmentShader;
using OpenGL::VertexShader;


/* Declarations */
struct ShapeSegmentData
{
	GLfloat colour[4];
	GLfloat position[2];
};

/* Definitions */
namespace OpenGL
{
	namespace ShapeRenderer
	{
		GLuint array_handle;
		GLuint buffer_handle;
		GLuint program_handle;
		GLuint total_vertex_count;
		GLuint uniform_translate;
		GLuint uniform_window_height;
		GLuint uniform_window_width;

		FragmentShader* fragment_shader;
		VertexShader* vertex_shader;

		void FillPath(Geometry::Path& path,Graphics::Colour colour)
		{
			const unsigned int first_offset = total_vertex_count;
			const unsigned int vertex_count = path.size() + 1;

			ShapeSegmentData data = {{colour.red,colour.green,colour.blue,colour.alpha},{0,0}};
			double x_max;
			double x_min;
			double y_max;
			double y_min;


			if(!path.isClosed())
			{
				throw std::runtime_error("Path must be closed to use draw a shape.");
			}

			x_max = std::numeric_limits<double>::lowest();
			x_min = -x_max;
			y_max = std::numeric_limits<double>::lowest();
			y_min = -y_max;
			for(Point& point : path)
			{
				if(point.x < x_min)
				{
					x_min = point.x;
				}
				if(point.x > x_max)
				{
					x_max = point.x;
				}
				if(point.y < y_min)
				{
					y_min = point.y;
				}
				if(point.y > y_max)
				{
					y_max = point.y;
				}
			}

			glBindBuffer(GL_ARRAY_BUFFER,buffer_handle);
			data.position[0] = (x_min + x_max) / 2;
			data.position[1] = (y_min + y_max) / 2;
			glBufferSubData(GL_ARRAY_BUFFER,total_vertex_count * sizeof(data),sizeof(data),&data);
			++total_vertex_count;

			for(Point& point : path)
			{
				data.position[0] = point.x;
				data.position[1] = point.y;
				glBufferSubData(GL_ARRAY_BUFFER,total_vertex_count * sizeof(data),sizeof(data),&data);
				++total_vertex_count;
			}
			glBindBuffer(GL_ARRAY_BUFFER,0);


			Point translate = OpenGL::GetOrigin();
			OpenGL::AddRenderTask([first_offset,vertex_count,translate](){
				glUseProgram(program_handle);
				glBindVertexArray(array_handle);
				glBindBuffer(GL_ARRAY_BUFFER,buffer_handle);

					glUniform2f(uniform_translate,translate.x,translate.y);
					glDrawArrays(GL_TRIANGLE_FAN,first_offset,vertex_count);

				glBindBuffer(GL_ARRAY_BUFFER,0);
				glBindVertexArray(0);
				glUseProgram(0);

				total_vertex_count -= vertex_count;
			});
		}

		void InitBuffers()
		{
			glGenBuffers(1,&buffer_handle);
			glBindBuffer(GL_ARRAY_BUFFER,buffer_handle);
			glBufferData(GL_ARRAY_BUFFER,1024 * sizeof(ShapeSegmentData),nullptr,GL_STREAM_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER,0);
		}

		void InitProgram()
		{
			GLint attribute;


			/* Obtain a new program handle. */
			program_handle = glCreateProgram();

			/* Compile the shaders. */
			vertex_shader = new VertexShader();
			vertex_shader->setSourceFile("./Resource/Shader/ShapeRenderer.vs.glsl",true);

			fragment_shader = new FragmentShader();
			fragment_shader->setSourceFile("./Resource/Shader/ShapeRenderer.fs.glsl",true);

			/* Attach all the shaders to the program. */
			glAttachShader(program_handle,*vertex_shader);
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
			glVertexAttribPointer(attribute,4,GL_FLOAT,GL_FALSE,sizeof(ShapeSegmentData),(void*)offsetof(ShapeSegmentData,colour));
			glEnableVertexAttribArray(attribute);

			attribute = glGetAttribLocation(program_handle,"position");
			glVertexAttribPointer(attribute,2,GL_FLOAT,GL_FALSE,sizeof(ShapeSegmentData),(void*)offsetof(ShapeSegmentData,position));
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
			delete vertex_shader;
		}
	}
}