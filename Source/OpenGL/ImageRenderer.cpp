#include "ImageRenderer.h"
#include <map>
#include "Shader.h"
#include "../Window.h"

using Geometry::Point;
using Graphics::Image;
using OpenGL::FragmentShader;
using OpenGL::VertexShader;


/* Definitions */
namespace OpenGL
{
	namespace ImageRenderer
	{
		struct VertexData
		{
			GLfloat uv[2];
			GLfloat position[2];
		};

		GLuint array_handle;
		GLuint buffer_handle;
		GLuint program_handle;
		GLuint total_vertex_count;
		GLuint uniform_translate;
		GLuint uniform_window_height;
		GLuint uniform_window_width;

		FragmentShader* fragment_shader;
		std::map<const Image*,GLuint> texture_handle_from_data;
		VertexShader* vertex_shader;

		void DrawImage(const Image& image)
		{
			DrawImage(image,0,0,image.getWidth(),image.getHeight());
		}

		void DrawImage(const Image& image,double clip_x,double clip_y,double clip_width,double clip_height)
		{
			const unsigned int buffer_offset = total_vertex_count;

			GLuint texture_handle;
			VertexData vertex_data = {{0,0},{0,0}};


			if(texture_handle_from_data.count(&image) > 0)
			{
				texture_handle = texture_handle_from_data[&image];
			}
			else
			{
				glGenTextures(1,&texture_handle);

				glBindTexture(GL_TEXTURE_2D,texture_handle);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
				glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,image.getWidth(),image.getHeight(),0,GL_RGBA,GL_UNSIGNED_BYTE,image.getData());
				glBindTexture(GL_TEXTURE_2D,0);

				texture_handle_from_data[&image] = texture_handle;
			}

			glBindBuffer(GL_ARRAY_BUFFER,buffer_handle);
			
				vertex_data.uv[0] = (float)clip_x / (float)image.getWidth();
				vertex_data.uv[1] = ((float)clip_y / (float)image.getHeight());
				vertex_data.position[0] = 0;
				vertex_data.position[1] = 0;
				glBufferSubData(GL_ARRAY_BUFFER,(buffer_offset + 0) * sizeof(vertex_data),sizeof(vertex_data),&vertex_data);

				vertex_data.uv[0] = ((float)clip_x / (float)image.getWidth()) + ((float)clip_width / (float)image.getWidth());
				vertex_data.uv[1] = ((float)clip_y / (float)image.getHeight());
				vertex_data.position[0] = clip_width;
				vertex_data.position[1] = 0;
				glBufferSubData(GL_ARRAY_BUFFER,(buffer_offset + 1) * sizeof(vertex_data),sizeof(vertex_data),&vertex_data);

				vertex_data.uv[0] = (float)clip_x / (float)image.getWidth();
				vertex_data.uv[1] = ((float)clip_y / (float)image.getHeight()) + ((float)clip_height / (float)image.getHeight());
				vertex_data.position[0] = 0;
				vertex_data.position[1] = clip_height;
				glBufferSubData(GL_ARRAY_BUFFER,(buffer_offset + 2) * sizeof(vertex_data),sizeof(vertex_data),&vertex_data);

				vertex_data.uv[0] = ((float)clip_x / (float)image.getWidth()) + ((float)clip_width / (float)image.getWidth());
				vertex_data.uv[1] = ((float)clip_y / (float)image.getHeight()) + ((float)clip_height / (float)image.getHeight());
				vertex_data.position[0] = clip_width;
				vertex_data.position[1] = clip_height;
				glBufferSubData(GL_ARRAY_BUFFER,(buffer_offset + 3) * sizeof(vertex_data),sizeof(vertex_data),&vertex_data);

			glBindBuffer(GL_ARRAY_BUFFER,0);
			
			total_vertex_count += 4;


			Point translate = OpenGL::GetOrigin();
			OpenGL::AddRenderTask([buffer_offset,translate,texture_handle](){
				glUseProgram(program_handle);
				glBindVertexArray(array_handle);
				glBindBuffer(GL_ARRAY_BUFFER,buffer_handle);
				glBindTexture(GL_TEXTURE_2D,texture_handle);
					
					glUniform2f(uniform_translate,translate.x,translate.y);
					glDrawArrays(GL_TRIANGLE_STRIP,buffer_offset,4);

				glBindTexture(GL_TEXTURE_2D,0);
				glBindBuffer(GL_ARRAY_BUFFER,0);
				glBindVertexArray(0);
				glUseProgram(0);

				total_vertex_count -= 4;
			});
		}

		void InitBuffers()
		{
			glGenBuffers(1,&buffer_handle);
			glBindBuffer(GL_ARRAY_BUFFER,buffer_handle);
			glBufferData(GL_ARRAY_BUFFER,1024 * sizeof(VertexData),nullptr,GL_STREAM_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER,0);
		}

		void InitProgram()
		{
			GLint attribute;


			/* Obtain a new program handle. */
			program_handle = glCreateProgram();

			/* Compile the shaders. */
			vertex_shader = new VertexShader();
			vertex_shader->setSourceFile("./Resource/Shader/ImageRenderer.vs.glsl",true);

			fragment_shader = new FragmentShader();
			fragment_shader->setSourceFile("./Resource/Shader/ImageRenderer.fs.glsl",true);

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

				attribute = glGetAttribLocation(program_handle,"uv");
				glVertexAttribPointer(attribute,4,GL_FLOAT,GL_FALSE,sizeof(VertexData),(void*)offsetof(VertexData,uv));
				glEnableVertexAttribArray(attribute);

				attribute = glGetAttribLocation(program_handle,"position");
				glVertexAttribPointer(attribute,2,GL_FLOAT,GL_FALSE,sizeof(VertexData),(void*)offsetof(VertexData,position));
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