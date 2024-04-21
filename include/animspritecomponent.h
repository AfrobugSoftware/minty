#pragma once
#include "spritecomponent.h"
#include <vector>

namespace minty {
	class AnimSpriteComponent : public SpriteComponent
	{
	public:
		AnimSpriteComponent(class Actor* mOWner, std::uint32_t drawOrder = 100);
		virtual ~AnimSpriteComponent();

		virtual void Update(float detlatime) override;
		void SetAnimTextures(std::vector<SDL_Texture*>&& texs);
		inline constexpr std::uint32_t& AnimFPS() { return mAnimFPS; }
		inline constexpr const std::uint32_t& AnimFPS() const { return mAnimFPS; }
	private:
		std::uint32_t mAnimFPS = 24;
		float mCurrFrame = 0.0f; 
		std::vector<SDL_Texture*> mSprites;
	};

	class BGSpriteComponent : public SpriteComponent {
	public:
		BGSpriteComponent(class Actor* mOWner, std::uint32_t drawOrder = 100);
		virtual ~BGSpriteComponent();
		virtual void Update(float detlatime) override;

		virtual void Draw(SDL_Renderer* render) override; 

		void SetBGTextures(const std::vector<SDL_Texture*>& texs);

		inline constexpr void SetScreenSize(glm::vec2 size) { mScreenSize = size; }
		inline constexpr void SetScrollSpeed(float speed) { mScrollSpeed = speed; }
		inline constexpr float GetScrollSpeed() const { return mScrollSpeed; }
	private:
		struct BGTexture {
			glm::vec2 offset;
			SDL_Texture* tex = NULL;
		};
		std::vector<BGTexture> mSprites;
		glm::vec2 mScreenSize;
		float mScrollSpeed = 0.0f;

	};
};