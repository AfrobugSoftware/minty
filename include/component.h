#pragma once
#include <cstdint>
#include <SDL2/SDL_render.h>

namespace minty {
	class Component {
	public:
		Component(class Actor* actor, std::uint32_t updateOrder = 100);
		virtual ~Component();
		virtual void Update(float deltatime);
		virtual void Draw(SDL_Renderer* render);
		inline constexpr std::uint32_t GetUpdateOrder() const { return mUpdateOrder; }
		virtual void ProcessInput(const std::uint8_t* keystate) {}
	protected:
		std::uint32_t mUpdateOrder;
		class  Actor* mActor;
	};
}