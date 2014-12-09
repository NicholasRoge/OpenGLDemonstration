#include "Geometry/Point.h"
#include "OpenGL.h"
#include "OpenGL/ImageRenderer.h"
#include "OpenGL/PathRenderer.h"
#include "OpenGL/ShapeRenderer.h"
#include <queue>
#include "Window.h"

using Geometry::Path;
using Geometry::Point;
using Graphics::Image;


/* Declarations */
namespace OpenGL
{
	void PerformRenderTasks();
}

/* Definitions */
namespace OpenGL
{
	HDC device_context;
	Point origin;
	HGLRC render_context;
	std::queue<std::function<void()> > render_tasks;

	Image font_image;

	void AddRenderTask(std::function<void()> task)
	{
		render_tasks.push(task);
	}

	void DrawImage(const Graphics::Image& image)
	{
		ImageRenderer::DrawImage(image);
	}

	void DrawImage(const Graphics::Image& image,double clip_x,double clip_y,double clip_width,double clip_height)
	{
		ImageRenderer::DrawImage(image,clip_x,clip_y,clip_width,clip_height);
	}

	void DrawPath(Geometry::Path& path,GLfloat size,Graphics::Colour colour)
	{
		PathRenderer::DrawPath(path,size,colour);
	}

	void DrawRectangle(double x,double y,double width,double height,GLfloat size,Graphics::Colour colour)
	{
		Path path;


		path
			.addPoint(x + (size / 2),y + (size / 2),0)
			.addPoint(x + width - (size / 2),y + (size / 2),0)
			.addPoint(x + width - (size / 2),y + height - (size / 2),0)
			.addPoint(x + (size / 2),y + height - (size / 2),0)
			.close();

		OpenGL::DrawPath(path,size,colour);
	}

	void DrawText(std::string text)
	{
		for(auto& c : text)
		{
			DrawImage(font_image,(c & 0x0F) * 12,((c & 0xF0) >> 4) * 18,12,18);
			OpenGL::TranslateOrigin(12,0);
		}
		OpenGL::TranslateOrigin(-(double)(12 * text.length()),0);
	}

	void FillPath(Geometry::Path& path,Graphics::Colour colour)
	{
		ShapeRenderer::FillPath(path,colour);
	}

	void FillRectangle(double x,double y,double width,double height,Graphics::Colour colour)
	{
		Path path;


		path
			.addPoint(x,y,0)
			.addPoint(x + width,y,0)
			.addPoint(x + width,y + height,0)
			.addPoint(x,y + height,0)
			.close();

		OpenGL::FillPath(path,colour);
	}

	Point GetOrigin()
	{
		return origin;
	}

	void Load()
	{
		/* Obtain the Device Context */
		OpenGL::device_context = GetDC(Window::handle);

		/* Set the Pixel Format */
		int format_handle;
		PIXELFORMATDESCRIPTOR descriptor;


		memset(&descriptor,0,sizeof(descriptor));
		descriptor.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		descriptor.nVersion = 1;
		descriptor.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_SUPPORT_COMPOSITION;
		descriptor.iPixelType = PFD_TYPE_RGBA;
		descriptor.cColorBits = 32;
		descriptor.cDepthBits = 24;
		descriptor.cRedBits = 8;
		descriptor.cGreenBits = 8;
		descriptor.cBlueBits = 8;
		descriptor.cAlphaBits = 8;
		descriptor.cStencilBits = 8;
		descriptor.iLayerType = PFD_MAIN_PLANE;

		format_handle = ChoosePixelFormat(OpenGL::device_context,&descriptor);
		if(format_handle == 0)
		{
			/* Cleanup Before Exiting */
			ReleaseDC(Window::handle,OpenGL::device_context);

			/* Notify the Developer About the Error and its Cause*/
			LPTSTR error_message;


			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,0,GetLastError(),0,(LPTSTR)&error_message,0,nullptr);
			MessageBox(nullptr,error_message,L"An Error Has Occurred",MB_OK | MB_ICONERROR);
			LocalFree((LPTSTR)&error_message);

			throw GetLastError();
		}

		if(!SetPixelFormat(OpenGL::device_context,format_handle,&descriptor))
		{
			/* Cleanup Before Exiting */
			ReleaseDC(Window::handle,OpenGL::device_context);

			/* Notify the Developer About the Error and its Cause*/
			LPTSTR error_message;


			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,0,GetLastError(),0,(LPTSTR)&error_message,0,nullptr);
			MessageBox(nullptr,error_message,L"An Error Has Occurred",MB_OK | MB_ICONERROR);
			LocalFree((LPTSTR)&error_message);

			throw GetLastError();
		}

		/* Obtain the Rendering Context */
		OpenGL::render_context = wglCreateContext(OpenGL::device_context);
		if(OpenGL::render_context == nullptr)
		{
			/* Cleanup Before Exiting */
			ReleaseDC(Window::handle,OpenGL::device_context);

			/* Notify the Developer About the Error and its Cause*/
			LPTSTR error_message;


			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,0,GetLastError(),0,(LPTSTR)&error_message,0,nullptr);
			MessageBox(nullptr,error_message,L"An Error Has Occurred",MB_OK | MB_ICONERROR);
			LocalFree((LPTSTR)&error_message);

			throw GetLastError();
		}

		/* Make the rendering context we just received active on this thread. */
		if(!wglMakeCurrent(OpenGL::device_context,OpenGL::render_context))
		{
			/* Cleanup Before Exiting */
			wglDeleteContext(OpenGL::render_context);
			ReleaseDC(Window::handle,OpenGL::device_context);

			/* Notify the Developer About the Error and its Cause*/
			LPTSTR error_message;


			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,0,GetLastError(),0,(LPTSTR)&error_message,0,nullptr);
			MessageBox(nullptr,error_message,L"An Error Has Occurred",MB_OK | MB_ICONERROR);
			LocalFree((LPTSTR)&error_message);

			throw GetLastError();
		}

		/* Initialize the GLEW library. */
		GLenum status;


		status = glewInit();
		if(status != GLEW_OK)
		{
			/* Cleanup Before Exiting */
			wglMakeCurrent(nullptr,nullptr);
			wglDeleteContext(OpenGL::render_context);
			ReleaseDC(Window::handle,OpenGL::device_context);

			/* Notify the Developer About the Error and its Cause*/
			LPTSTR error_message;


			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,0,GetLastError(),0,(LPTSTR)&error_message,0,nullptr);
			MessageBox(nullptr,L"Unable to initialize GLEW.",L"An Error Has Occurred",MB_OK | MB_ICONERROR);
			LocalFree((LPTSTR)&error_message);

			throw status;

			/*std::clog
				<< "[Error]" << std::endl
				<< "  Unable to Initialize GLEW." << std::endl
				<< "  Cause:  " << glewGetErrorString(status) << std::endl;

			throw status;*/
		}

		/* Initialize OpenGL itself. */
		glClearColor(0.0f,0.0f,0.0f,0.0f);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glClearDepth(1.0f);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/* Load the individual renderers. */
		PathRenderer::Load();
		ShapeRenderer::Load();
		ImageRenderer::Load();

		/* Load our font image. */
		font_image.setSourceFile("./Resource/Font/CourierNew_18.bmp");
	}

	void PerformRenderTasks()
	{
		while(render_tasks.size() > 0)
		{
			render_tasks.front()();
			render_tasks.pop();
		}
	}

	void Render()
	{
		PathRenderer::OnBeforeRender();
		ShapeRenderer::OnBeforeRender();
		ImageRenderer::OnBeforeRender();

		PerformRenderTasks();
		SwapBuffers(OpenGL::device_context);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImageRenderer::OnAfterRender();
		ShapeRenderer::OnAfterRender();
		PathRenderer::OnAfterRender();
	}

	void SetOrigin(double x,double y)
	{
		origin.x = x;
		origin.y = y;
	}

	std::string TranslateErrorMessage(void* context)
	{
		return TranslateErrorMessage(context,glGetError());
	}

	std::string TranslateErrorMessage(void* context,GLenum error)
	{
		if(error == GL_NO_ERROR)
		{
			return "No error has occured.";
		}

		if(context == glCreateShader)
		{
			switch(error)
			{
				case GL_INVALID_ENUM:
					return "Inavlid shader type specified.";
			}
		}
		
		return "Unknown error.  Please refer to online documentation.";
	}

	void TranslateOrigin(double x,double y)
	{
		origin.x += x;
		origin.y += y;
	}
	
	void Unload()
	{
		ImageRenderer::Unload();
		ShapeRenderer::Unload();
		PathRenderer::Unload();

		wglMakeCurrent(nullptr,nullptr);
		wglDeleteContext(OpenGL::render_context);
		ReleaseDC(Window::handle,OpenGL::device_context);
	}
}