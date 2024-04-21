#pragma once
#include "component.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_rect.h>
#include <glm/glm.hpp>

namespace minty {
	class SpriteComponent : public Component
	{
	public:
		SpriteComponent(class Actor* owner, std::uint32_t drawOrder = 100);
		virtual ~SpriteComponent();

		virtual void Draw(SDL_Renderer* render);
		void SetTexture(SDL_Texture* texture);

		inline constexpr std::int32_t GetHeight() const { return mHeight; }
		inline constexpr std::int32_t GetWidth() const { return mWidth; }
		inline constexpr std::uint32_t GetDrawOrder() const { return mDrawOrder; }

		virtual void Update(float deltatime);
	protected:
		std::int32_t mHeight = 0;
		std::int32_t mWidth = 0;
		std::uint32_t mDrawOrder = 0;
		std::uint32_t mFormat = 0;
		SDL_Texture* mTexture = NULL;
		

	};

	class ColorComponent : public SpriteComponent
	{
	public:
		ColorComponent(class Actor* owner, std::uint32_t drawOrder = 100);
		virtual ~ColorComponent() {}


		virtual void Draw(SDL_Renderer* render);

		inline constexpr void SetHeight(std::uint32_t h) { mHeight = h; }
		inline constexpr void SetWidth(std::uint32_t w) { mWidth = w; }
		inline constexpr void SetDrawOrder(std::uint32_t h) { mDrawOrder = h; }
		inline constexpr void SetColor(std::uint32_t c) { color = c; }

	protected:
		std::uint32_t color = 0xFFFFFFFF;
	};
};