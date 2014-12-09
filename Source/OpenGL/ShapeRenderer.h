#ifndef OPENGL_SHAPERENDERER_H
#define OPENGL_SHAPERENDERER_H

#include "../OpenGL.h"
#include "../Geometry/Path.h"


namespace OpenGL
{
	namespace ShapeRenderer
	{
		void FillPath(Geometry::Path& path,Graphics::Colour colour);

		void Load();

		void OnBeforeRender();

		void OnAfterRender();

		void Unload();
	}
}

#endif