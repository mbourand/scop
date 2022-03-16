#pragma once

#include "EZGL/KeyListener.h"
#include "EZGL/Math/Matrix.h"

namespace scop
{
	class ModelController : public ezgl::KeyListener
	{
	public:
		enum class Direction
		{
			X, Y, Z, None
		};

	protected:
		Direction _direction;
		bool _upPressed, _downPressed;
		float _speed;
		float _pitch, _yaw, _roll;

	public:
		ModelController();

		virtual void onKeyPressed(int key, int scancode) override;
		virtual void onKeyReleased(int key, int scancode) override;
		virtual void onKeyRepeat(int key, int scancode) override;
		virtual void update(float deltaTime) override;

		ezgl::Matrix<float, 4, 4> getRotationMatrix() const;
	};
}