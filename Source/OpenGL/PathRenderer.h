#ifndef OPENGL_PATHRENDERER_H
#define OPENGL_PATHRENDERER_H

#include "../OpenGL.h"
#include "../Geometry/Path.h"


namespace OpenGL
{
	namespace PathRenderer
	{
		void DrawPath(Geometry::Path& path,GLfloat width,Graphics::Colour colour);

		void Load();

		void OnBeforeRender();

		void OnAfterRender();

		void Unload();
	}
}

#endif