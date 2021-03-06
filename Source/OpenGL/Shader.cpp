#include "Shader.h"

#include <fstream>

using OpenGL::Shader;
using OpenGL::ComputeShader;
using OpenGL::FragmentShader;
using OpenGL::GeometryShader;
using OpenGL::VertexShader;


/* Type [OpenGL::Shader] Definition */
Shader::Shader(GLenum type)
{
	GLenum error;


	this->handle = glCreateShader(type);
	if(this->handle == 0)
	{
		error = glGetError();
		switch(error)
		{
			case GL_INVALID_ENUM:
				throw std::runtime_error("Call to 'glCreateShader' failed.  Cause:  Inavlid shader type specified.");

			default:
				throw std::runtime_error("Call to 'glCreateShader' failed.  Cause:  Unknown error.  Please refer to online documentation.");
		}
	}
}

Shader::~Shader()
{
	glDeleteShader(this->handle);
}

void Shader::compile()
{
	GLint compile_status;
	GLenum error;


	glCompileShader(this->handle);
	glGetShaderiv(this->handle,GL_COMPILE_STATUS,&compile_status);
	if(compile_status == GL_FALSE)
	{
		error = glGetError();
		switch(error)
		{
			case GL_INVALID_OPERATION:
				if(glIsShader(this->handle))
				{
					throw std::runtime_error("Call to 'glCompileShader' failed.  Cause:  shader compiler is not supported.");
				}
				else
				{
					throw std::runtime_error("Call to 'glCompileShader' failed.  Cause:  supplied handle does not represent a shader object.");
				}

			case GL_INVALID_VALUE:
				throw std::runtime_error("Call to 'glCompileShader' failed.  Cause:  supplied shader handle was not generated by OpenGL.");

			default:
				throw std::runtime_error("Call to 'glCompileShader' failed.  See 'Shader#info' for additional information.");
		}
	}
}

bool Shader::hasInfo() const
{
	GLint length;


	glGetShaderiv(this->handle,GL_INFO_LOG_LENGTH,&length);
	return length != 0;
}

bool Shader::hasSource() const
{
	GLint length; 


	glGetShaderiv(this->handle,GL_INFO_LOG_LENGTH,&length);
	return length != 0;
}

std::string Shader::info() const
{
	char* buffer;
	GLint buffer_length;


	glGetShaderiv(this->handle,GL_INFO_LOG_LENGTH,&buffer_length);
	if(buffer_length == 0)
	{
		return std::string();
	}
	else
	{
		std::string info;


		buffer = new char[buffer_length];
		glGetShaderInfoLog(this->handle,buffer_length,nullptr,buffer);
		info.assign(buffer,buffer_length);
		delete[] buffer;

		return info;
	}
}

void Shader::setSource(const char* source,bool compile)
{
	this->setSource(source,strlen(source),compile);
}

void Shader::setSource(const char* source,size_t length,bool compile)
{
	GLenum error;


	glShaderSource(this->handle,1,&source,nullptr);
	error = glGetError();
	if(error != GL_NO_ERROR)
	{
		switch(error)
		{
			case GL_INVALID_VALUE:
				if(glIsShader(this->handle))
				{
					throw std::runtime_error("Call to 'glShaderSource' failed.  Cause:  argument 'count' is less than zero.");
				}
				else
				{
					throw std::runtime_error("Call to 'glShaderSource' failed.  Cause:  supplied shader handle was not generated by OpenGL.");
				}

			case GL_INVALID_OPERATION:
				throw std::runtime_error("Call to 'glShaderSource' failed.  Cause:  supplied handle does not represent a shader object.");

			default:
				throw std::runtime_error("Call to 'glShaderSource' failed.  Cause:  Unknown error.  Please refer to online documentation.");
		}
	}

	if(compile)
	{
		this->compile();
	}
}

void Shader::setSource(const std::string& source,bool compile)
{
	this->setSource(source.c_str(),source.length(),compile);
}

void Shader::setSourceFile(const char* path,bool compile)
{
	std::string line;
	std::string source;
	std::ifstream stream;


	stream.open(path);
	if(!stream.is_open())
	{
		throw std::runtime_error(std::string("Attempt to read shader source file '").append(path).append("' failed.  Cause:  File could not be opened."));
	}

	while(std::getline(stream,line))
	{
		source.append(line).append("\n");
	}
	stream.close();

	this->setSource(source.c_str(),source.length(),compile);
}

void Shader::setSourceFile(const std::string& path,bool compile)
{
	this->setSourceFile(path.c_str(),compile);
}

std::string Shader::source() const
{
	char* buffer;
	GLint buffer_length;


	glGetShaderiv(this->handle,GL_SHADER_SOURCE_LENGTH,&buffer_length);
	if(buffer_length == 0)
	{
		return std::string();
	}
	else
	{
		std::string source;


		buffer = new char[buffer_length];
		glGetShaderSource(this->handle,buffer_length,nullptr,buffer);
		source.assign(buffer,buffer_length);
		delete[] buffer;

		return source;
	}
}

GLenum Shader::type() const
{
	GLint type;


	glGetShaderiv(this->handle,GL_SHADER_TYPE,&type);
	return type;
}

Shader::operator GLuint() const
{
	return this->handle;
}

/* Type [OpenGL::ComputationShader] Definition */
ComputeShader::ComputeShader()
: Shader(GL_COMPUTE_SHADER)
{
}

/* Type [OpenGL::ComputationShader] Definition */
FragmentShader::FragmentShader()
: Shader(GL_FRAGMENT_SHADER)
{
}

/* Type [OpenGL::ComputationShader] Definition */
GeometryShader::GeometryShader()
: Shader(GL_GEOMETRY_SHADER)
{
}

/* Type [OpenGL::ComputationShader] Definition */
VertexShader::VertexShader()
: Shader(GL_VERTEX_SHADER)
{
}