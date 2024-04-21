#include "movecomponent.h"
#include "Actor.h"
#include "game.h"

minty::MoveComponent::MoveComponent(minty::Actor* actor, std::uint32_t updateOrder) :
	minty::Component(actor, updateOrder){
}

minty::MoveComponent::~MoveComponent() {}

void minty::MoveComponent::Update(float deltatime)
{
	if (std::abs(mAngularSpeed) > glm::epsilon<float>()) {
		float rot = mActor->GetRotation();
		rot += mAngularSpeed * deltatime;
		mActor->SetRotation(rot);
	}

	if (std::abs(mLinerSpeed) > glm::epsilon<float>()) {
		glm::vec2 pos = mActor->GetPos();
		pos += mActor->GetForward() * mLinerSpeed * deltatime;
		if (mWrapScreen) {
			auto ss = mActor->GetGame()->GetScreenSize();
			if (pos.x > ss.x) {
				pos.x = 0;
			}
			else if (pos.x < 0) {
				pos.x = ss.x;
			}

			if (pos.y > ss.y) {
				pos.y = 0;
			}
			else if (pos.y < 0) {
				pos.y = ss.y;
			}
		}
		
		mActor->SetPos(pos);
	}

	
}

minty::InputComponent::InputComponent(Actor* owner, std::uint32_t updateOrder)
: MoveComponent(owner, updateOrder){
}

minty::InputComponent::~InputComponent()
{
}

void minty::InputComponent::Update(float deltatime)
{
	minty::MoveComponent::Update(deltatime);
}

void minty::InputComponent::ProcessInput(const std::uint8_t* state)
{
	float fowardSpeed = 0.0f, angularSpeed = 0.0f, verticalSpeed = 0.0f;
	if (state[mForwardKey]) {
		fowardSpeed += mMaxLinerSpeed;
	}
	if (state[mBackwardKey]) {
		fowardSpeed -= mMaxLinerSpeed;
	}
	mLinerSpeed = fowardSpeed;

	if (state[mClockwiseKey]) {
		angularSpeed += mMaxAngularSpeed;
	}

	if (state[mCounterClockwiseKey]) {
		angularSpeed -= mMaxAngularSpeed;
	}
	mAngularSpeed = angularSpeed;
}

