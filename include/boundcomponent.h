#pragma once
#include "component.h"

namespace minty {
	class BoundComponent : public Component {
	public:
		BoundComponent(class Actor* actor, std::uint32_t updateOrder = 100);
		virtual ~BoundComponent() {}

		inline constexpr float GetRadius() const { return mRadius; }
		inline constexpr void SetRadius(float radius) { mRadius = radius; }
	private:
		float mRadius = 0.0f;
	};
};