#pragma once
#include"glm/glm.hpp"
#include <SDL2/SDL_render.h>
#include <vector>
#include <ranges>

namespace minty
{
	//base class for bound volume
	class BoundVolume {
	public:
		BoundVolume();
		virtual ~BoundVolume();
		virtual bool Intersects(const BoundVolume* bv) const;

		glm::vec2 center;
		float radius = 0.0f;
	};

	class Transformation {
	public:

	};

	class Actor {
	public:
		enum class state
		{
			ACTIVE,
			PAUSE,
			DEAD
		};

		Actor(class Game* game);
		virtual ~Actor();

		void Update(float dt);
		virtual void Draw(SDL_Renderer* render);
		void UpdateComponents(float deltatime);
		virtual void UpdateActor(float deltaTime);
		inline constexpr std::vector<class Component*>& GetComponents() { return mComponents;  };

		void AddComponents(class Component* comm);
		void RemvComponents(class Component* comm);

		inline constexpr state GetState() const { return mState;  }
		inline constexpr glm::vec2 GetPos() const { return mBoundVolume->center; };
		inline constexpr float GetScale() const { return scale; }
		inline constexpr float GetRotation() const { return rotation; }
		inline constexpr float GetRadius() const { return mBoundVolume->radius; }

		inline constexpr void SetState(state st) { mState = st; }
		inline constexpr void SetPos(const glm::vec2& v) { mBoundVolume->center = v; };
		inline constexpr void SetScale(float s) { scale = s; }
		inline constexpr void SetRotation(float rads) { rotation = rads; }
		inline constexpr void SetRadius(float radius) { mBoundVolume->radius = radius; }

		void ProcessInput(const std::uint8_t* state);
		virtual void ActorInput(const std::uint8_t* state) {}
		virtual void ActorMouseInput(const float x, const float y) {}

		glm::vec2 GetForward() const;
		void SetRotation(const glm::vec2& v);

		constexpr const Game* GetGame() const { return mGame; }
		constexpr Game* GetGame()  { return mGame; }

		virtual bool Intersects(const BoundVolume* bv) const;

		constexpr const BoundVolume* GetBound() const { return mBoundVolume; }
	protected:
		state mState = state::ACTIVE;
		float rotation = 0.0f;
		float scale = 1.0f;
		class Game* mGame = NULL;
		BoundVolume* mBoundVolume = NULL;

		std::vector<class Component*> mComponents;
	};
}
