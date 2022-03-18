#include "EZGL/MouseListener.h"
#include <algorithm>

namespace ezgl
{
	std::vector<MouseListener*> MouseListener::_instances;

	MouseListener::MouseListener()
	{
		_instances.push_back(this);
	}

	MouseListener::~MouseListener()
	{
		std::remove_if(_instances.begin(), _instances.end(), [this](MouseListener* a) -> bool { return a == this; });
	}

	void mousePosCallback(GLFWwindow* window, double x, double y)
	{
		for (auto& instance : MouseListener::_instances)
			instance->onMouseMoved(x, y);
	}

	void mouseEnterCallback(GLFWwindow* window, int entered)
	{
		for (auto& instance : MouseListener::_instances)
			instance->onMouseEntered(entered);
	}

	void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		for (auto& instance : MouseListener::_instances)
		{
			switch (action)
			{
			case GLFW_PRESS: 
				instance->onMouseButtonPressed(button, mods);
				break;
			case GLFW_RELEASE:
				instance->onMouseButtonReleased(button, mods);
				break;
			}
		}
	}

	void updateMouseListeners(float deltaTime)
	{
		for (auto& instance : MouseListener::_instances)
			instance->update(deltaTime);
	}
}
