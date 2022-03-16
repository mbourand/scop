#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <stdexcept>
#include "../../inc/EZGL/Window.h"
#include "../../inc/EZGL/KeyListener.h"
#include "../../inc/EZGL/MouseListener.h"
#include <chrono>

namespace ezgl
{
	GLFWwindow* Window::_window = nullptr;
	unsigned int Window::_width = 0;
	unsigned int Window::_height = 0;
	PerspectiveCamera* Window::_camera = nullptr;

	void Window::create(unsigned int width, unsigned int height, const std::string& title, PerspectiveCamera& camera)
	{
		_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
		_width = width;
		_height = height;
		_camera = &camera;
		if (!_window)
		{
			glfwTerminate();
			throw std::runtime_error("Failed to create the window");
		}

		glfwMakeContextCurrent(_window);

		if (glewInit() != GLEW_OK)
		{
			glfwTerminate();
			throw std::runtime_error("Failed to create the window");
		}

		glfwSetKeyCallback(_window, ezgl::keyCallback);
		glfwSetInputMode(_window, GLFW_STICKY_KEYS, 1);

		glfwSetCursorPosCallback(_window, ezgl::mousePosCallback);
		glfwSetCursorEnterCallback(_window, ezgl::mouseEnterCallback);
		glfwSetMouseButtonCallback(_window, ezgl::mouseButtonCallback);
	}

	void Window::clear()
	{
		glViewport(0, 0, _width, _height);
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);
	}

	void Window::update()
	{
		using namespace std::chrono;
		static auto lastUpdate = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
		auto now = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
		auto deltaTime = now - lastUpdate;
		if (deltaTime > 0)
			ezgl::updateKeyListeners(deltaTime / 1000.0f);

		lastUpdate = now;
	}

	void Window::pollEvents()
	{
		glfwPollEvents();
	}

	void Window::display()
	{
		glfwSwapBuffers(_window);
	}

	bool Window::shouldClose()
	{
		return glfwWindowShouldClose(_window);
	}

	void Window::showCursor()
	{
		glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	void Window::hideCursor()
	{
		glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}

	void Window::centerCursor()
	{
		glfwSetCursorPos(_window, _width / 2, _height / 2);
	}

	unsigned int Window::getWidth() { return _width; }
	unsigned int Window::getHeight() { return _height; }
	void Window::updateCameraProjection() { _camera->updateProjection(); }
	void Window::setWidth(unsigned int width) { _width = width; }
	void Window::setHeight(unsigned int height) { _height = height; };
	GLFWwindow* Window::getPtr() { return _window; }
}