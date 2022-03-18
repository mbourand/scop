#pragma once

#include "EZGL/KeyListener.h"
#include "EZGL/MouseListener.h"
#include "EZGL/Math/Matrix.h"
#include "EZGL/Math/Vector.h"

namespace scop
{
	class ModelController : public ezgl::KeyListener, public ezgl::MouseListener
	{
	public:
		enum class Direction
		{
			X, Y, Z, None
		};

		enum class Mode
		{
			Rotate, Move
		};

	protected:
		Mode _mode;
		Direction _direction;
		ezgl::Vector3<float> _pos;
		bool _upPressed, _downPressed;
		float _rotateSpeed, _moveSpeed;
		float _pitch, _yaw, _roll;
		float _transitionValue;
		bool _transitionState;

	public:
		ModelController();

		virtual void onKeyPressed(int key, int scancode) override;
		virtual void onKeyReleased(int key, int scancode) override;
		virtual void onKeyRepeat(int key, int scancode) override;

		virtual void onMouseScroll(double xoffset, double yoffset) override;
		virtual void onMouseMoved(double x, double y) override;
		virtual void onMouseEntered(int entered) override;
		virtual void onMouseButtonPressed(int button, int mods) override;
		virtual void onMouseButtonReleased(int button, int mods) override;

		virtual void update(float deltaTime) override;


		float getTransitionValue() const;
		ezgl::Matrix<float, 4, 4> getRotationMatrix() const;
		ezgl::Matrix<float, 4, 4> ModelController::getTranslationMatrix() const;
	};
}
