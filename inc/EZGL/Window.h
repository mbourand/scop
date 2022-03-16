#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "PerspectiveCamera.h"

namespace ezgl
{
	class Window
	{
	protected:
		static GLFWwindow* _window;
		static unsigned int _width;
		static unsigned int _height;
		static PerspectiveCamera* _camera;

	public:
		static void create(unsigned int width, unsigned int height, const std::string& title, PerspectiveCamera& camera);

		static void clear();
		static void pollEvents();
		static void update();
		static void display();
		static bool shouldClose();

		static void showCursor();
		static void hideCursor();
		static void centerCursor();

		static unsigned int getWidth();
		static unsigned int getHeight();
		static void updateCameraProjection();
		static void setWidth(unsigned int width);
		static void setHeight(unsigned int height);
		static GLFWwindow* getPtr();
	};
}