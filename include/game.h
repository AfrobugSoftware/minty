#pragma once
#include <type_traits>
#include <memory>
#include <spdlog/spdlog.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <filesystem>
#include <ranges>
#include <algorithm>

//components
#include "spritecomponent.h"

//Screen dimension constants
#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480

namespace fs = std::filesystem;
namespace minty
{
	class Game :  public std::enable_shared_from_this<Game>{
	public:
		Game();
		virtual ~Game();


		bool Init();
		void ProcessInput();
		void Update();
		void Render();
		void RunLoop();
		void Shutdown();

		void AddActor(class Actor* ac);
		void RemvActor(class Actor* ac);

		SDL_Texture* LoadTexture(const fs::path& filename);
		inline constexpr glm::ivec2 GetScreenSize() const { return mScreenSize; }

		//virtual
		virtual bool LoadData() { return true; }
		SDL_Texture* GetTexture(const std::string& name);
	protected:
		std::unordered_map<std::string, SDL_Texture*> mTexMap;
		std::uint32_t mTicks = 0;
		bool mUpdatingActor = false;
		bool mIsRunning = false;
		SDL_Surface* mScreenSurface = NULL;
		SDL_Renderer* mRender = NULL;
		SDL_Window* mWindow = NULL;

		glm::ivec2 mScreenSize;
		std::vector<class Actor*> mActors;
		std::vector<class Actor*> mPendingActors;
	};
};