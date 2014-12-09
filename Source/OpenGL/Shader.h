#ifndef OPENGL_SHADER_H
#define OPENGL_SHADER_H

#include "../OpenGL.h"
#include <string>

#define GLSL(SOURCE) "#version 150 core\n" #SOURCE


namespace OpenGL
{
	class Shader
	{
		private:
			GLuint handle;

		public:
			Shader(GLenum type);

			virtual ~Shader();

			void compile();

			bool hasInfo() const;

			bool hasSource() const;

			std::string info() const;

			void setSource(const char* source,bool compile = false);

			void setSource(const char* source,size_t length,bool compile = false);

			void setSource(const std::string& source,bool compile = false);

			void setSourceFile(const char* path,bool compile = false);

			void setSourceFile(const std::string& path,bool compile = false);

			std::string source() const;

			GLenum type() const;

			operator GLuint() const;
	};

	class ComputeShader : public Shader
	{
		public:
		ComputeShader();
	};

	class FragmentShader : public Shader
	{
		public:
			FragmentShader();
	};

	class GeometryShader : public Shader
	{
		public:
		GeometryShader();
	};

	class VertexShader : public Shader
	{
		public:
		VertexShader();
	};
}

#endif