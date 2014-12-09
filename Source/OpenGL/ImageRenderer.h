#ifndef OPENGL_IMAGERENDERER_H
#define OPENGL_IMAGERENDERER_H

#include "../Graphics/Image.h"
#include "../OpenGL.h"


namespace OpenGL
{
	namespace ImageRenderer
	{
		void DrawImage(const Graphics::Image& image);

		void DrawImage(const Graphics::Image& image,double clip_x,double clip_y,double clip_width,double clip_height);

		void Load();

		void OnBeforeRender();

		void OnAfterRender();

		void Unload();
	}
}

#endif