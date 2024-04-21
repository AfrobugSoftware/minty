#pragma once
#include "component.h"
#include <glm/glm.hpp>
#include <glm/ext/scalar_constants.hpp>

namespace minty {
	class MoveComponent : public Component
	{
	public:
		MoveComponent(class Actor* owner, std::uint32_t updateOrder = 100);
		virtual ~MoveComponent();

		virtual void Update(float deltatime) override;

		//public because setting and getting has no side effects
		float mAngularSpeed = 0.0f;
		float mLinerSpeed = 0.0f;
		bool mWrapScreen = true;
		
	};

	class InputComponent : public MoveComponent
	{
	public:
		InputComponent(class Actor* owner, std::uint32_t updateOrder = 100);
		virtual ~InputComponent();

		virtual void Update(float deltatime) override;
		virtual void ProcessInput(const std::uint8_t* state) override;

		float mMaxLinerSpeed = 0.0f;
		float mMaxAngularSpeed = 0.0f;
		float mMaxVerticalSpeed = 0.0f;

		int mForwardKey = 0;
		int mBackwardKey = 0;
		int mUpKey = 0;
		int mDownKey = 0;
		int mClockwiseKey = 0;
		int mCounterClockwiseKey = 0;
	};
};