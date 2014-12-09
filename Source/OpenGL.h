#ifndef OpenGL_H
#define OpenGL_H

#define GLEW_STATIC

#include <GL/glew.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <Windows.h>

#include <functional>
#include "Geometry/Path.h"
#include "Graphics/Colour.h"
#include "Graphics/Image.h"


namespace OpenGL
{
	void AddRenderTask(std::function<void()> task);

	void DrawImage(const Graphics::Image& image);

	void DrawImage(const Graphics::Image& image,double clip_x,double clip_y,double clip_width,double clip_height);

	void DrawPath(Geometry::Path& path,GLfloat size,Graphics::Colour colour);

	void DrawRectangle(double x,double y,double width,double height,GLfloat size,Graphics::Colour colour);

	void DrawText(std::string text);

	void FillPath(Geometry::Path& path,Graphics::Colour colour);

	void FillRectangle(double x,double y,double width,double height,Graphics::Colour colour);

	Geometry::Point GetOrigin();

	void Load();

	void Render();

	void SetOrigin(double x,double y);

	std::string TranslateErrorMessage(void* context);

	std::string TranslateErrorMessage(void* context,GLenum error);

	void TranslateOrigin(double x,double y);

	void Unload();
}

#endif