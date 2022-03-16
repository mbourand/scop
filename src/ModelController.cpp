#include "../inc/ModelController.h"
#include "../inc/EZGL/Math/Vector.h"

namespace scop
{
	ModelController::ModelController()
	{
		this->_direction = Direction::None;
		this->_upPressed = false;
		this->_downPressed = false;
		this->_speed = 1.0f;
		this->_yaw = 0;
		this->_pitch = 0;
		this->_roll = 0;
	}

	void ModelController::onKeyPressed(int key, int scancode)
	{
		if (key == GLFW_KEY_X)
			this->_direction = (this->_direction == Direction::X ? Direction::None : Direction::X);
		else if (key == GLFW_KEY_Y)
			this->_direction = (this->_direction == Direction::Y ? Direction::None : Direction::Y);
		else if (key == GLFW_KEY_Z)
			this->_direction = (this->_direction == Direction::Z ? Direction::None : Direction::Z);

		if (key == GLFW_KEY_UP)
			this->_upPressed = true;
		if (key == GLFW_KEY_DOWN)
			this->_downPressed = true;
	}

	void ModelController::onKeyReleased(int key, int scancode)
	{
		if (key == GLFW_KEY_UP)
			this->_upPressed = false;
		if (key == GLFW_KEY_DOWN)
			this->_downPressed = false;
	}

	void ModelController::onKeyRepeat(int key, int scancode)
	{
	}

	void ModelController::update(float deltaTime)
	{
		int dir = this->_upPressed - this->_downPressed;
		float speed = this->_speed * dir * deltaTime;

		switch (this->_direction)
		{
		case Direction::X:
			this->_yaw += speed;
			break;
		case Direction::Y:
			this->_pitch += speed;
			break;
		case Direction::Z:
			this->_roll += speed;
			break;
		}
	}

	ezgl::Matrix<float, 4, 4> ModelController::getRotationMatrix() const
	{
		ezgl::Matrix<float, 4, 4> rotationMatrix;

		std::array<ezgl::Vector3<float>, 3> axis = {
			ezgl::Vector3<float>(1, 0, 0),
			ezgl::Vector3<float>(0, 1, 0),
			ezgl::Vector3<float>(0, 0, 1),
		};

		std::array<float, 3> values = {
			this->_yaw, this->_pitch, this->_roll
		};

		std::array<ezgl::Matrix<float, 4, 4>, 3> matrixes;

		for (size_t i = 0; i < 3; i++)
		{
			float c = std::cos(values[i]);
			float s = std::sin(values[i]);

			float x = axis[i].x;
			float y = axis[i].y;
			float z = axis[i].z;

			matrixes[i].at(0, 0) = (1 - c) * x * x + c;
			matrixes[i].at(1, 0) = (1 - c) * x * y - s * z;
			matrixes[i].at(2, 0) = (1 - c) * x * z + s * y;
			matrixes[i].at(3, 0) = 0;
			matrixes[i].at(0, 1) = (1 - c) * x * y + s * z;
			matrixes[i].at(1, 1) = (1 - c) * y * y + c;
			matrixes[i].at(2, 1) = (1 - c) * y * z - s * x;
			matrixes[i].at(3, 1) = 0;
			matrixes[i].at(0, 2) = (1 - c) * x * z - s * y;
			matrixes[i].at(1, 2) = (1 - c) * y * z + s * x;
			matrixes[i].at(2, 2) = (1 - c) * z * z + c;
			matrixes[i].at(3, 2) = 0;
			matrixes[i].at(0, 3) = 0;
			matrixes[i].at(1, 3) = 0;
			matrixes[i].at(2, 3) = 0;
			matrixes[i].at(3, 3) = 1;
		}

		return matrixes[0] * matrixes[1] * matrixes[2];
	}
}