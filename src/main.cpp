#include <iostream>

#define SPDLOG_USE_STD_FORMART
#include <spdlog/spdlog.h>

#include <SDL2/SDL.h>
#include <stdio.h>
#include "game.h"
#include "Actor.h"
#include "animspritecomponent.h"
#include "movecomponent.h"
#include <random>


//Screen dimension constants
namespace pingpong {
    class Game {
    public:
        Game() {
            mBallPos = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
            mPaddlePos = { 10, SCREEN_HEIGHT / 2 };
            mPaddlePos2 = { SCREEN_WIDTH - 10 , SCREEN_HEIGHT / 2 };
            mBallDir = { -200.f, 235.0f };
        }
        ~Game() {
        }

        bool Init() {
            if (SDL_Init(SDL_INIT_VIDEO) < 0)
            {
                spdlog::error("SDL could not initialize! SDL_Error: {}\n", SDL_GetError());
                return false;
            }
            mWindow = SDL_CreateWindow("minty", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SKIP_TASKBAR);
            if (mWindow == NULL)
            {
                spdlog::error("Window could not be created! SDL_Error: {}\n", SDL_GetError());
                return -1;
            }

            //Get window surface
          //  mScreenSurface = SDL_GetWindowSurface(mWindow);

            //Fill the surface white ish
            //SDL_FillRect(mScreenSurface, NULL, SDL_MapRGB(mScreenSurface->format, 0xFA, 0x3A, 0x3A));

            mRender = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (mRender == nullptr) {
                spdlog::error("SDL could not create renderer! SDL_Error: {}\n", SDL_GetError());
                return false;
            }

            //Update the surface
            mIsRunning = true;
            SDL_UpdateWindowSurface(mWindow);
            return true;
        }

        void ProcessInput() {
            SDL_Event e;
            while (SDL_PollEvent(&e)) {
                switch (e.type)
                {
                case SDL_QUIT:
                    mIsRunning = false;
                default:
                    break;
                }
            }

            const std::uint8_t* state = SDL_GetKeyboardState(NULL);

            mPaddleDir = 0;
            if (state[SDL_SCANCODE_W]) {
                mPaddleDir -= 1;
            }

            if (state[SDL_SCANCODE_S]) {
                mPaddleDir += 1;
            }

            mPaddleDir2 = 0;
            if (state[SDL_SCANCODE_UP]) {
                mPaddleDir2 -= 1;
            }

            if (state[SDL_SCANCODE_DOWN]) {
                mPaddleDir2 += 1;
            }
        }

        void GenerateOutput() {
            SDL_SetRenderDrawColor(mRender, 0xFA, 0x3A, 0xFA, 0xFF);

            SDL_RenderClear(mRender);

            SDL_SetRenderDrawColor(mRender, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_Rect rect(0, 0, SCREEN_WIDTH, thickness);
            SDL_RenderFillRect(mRender, &rect);

            rect.y = SCREEN_HEIGHT - thickness;
            SDL_RenderFillRect(mRender, &rect);

            SDL_Rect ball{ static_cast<int>(mBallPos.x - thickness / 2),
                    static_cast<int>(mBallPos.y - thickness / 2), thickness, thickness };

            SDL_Rect paddle{ static_cast<int>(mPaddlePos.x - ((thickness) / 2)),
                   static_cast<int>(mPaddlePos.y - ((thickness * 5) / 2)), thickness, thickness * 5 };

            SDL_Rect paddle2{ static_cast<int>(mPaddlePos2.x - ((thickness) / 2)),
                  static_cast<int>(mPaddlePos2.y - ((thickness * 5) / 2)), thickness, thickness * 5 };

            SDL_SetRenderDrawColor(mRender, 0x00, 0xFF, 0x00, 0xFF);
            SDL_RenderFillRect(mRender, &ball);
            SDL_RenderFillRect(mRender, &paddle);
            SDL_RenderFillRect(mRender, &paddle2);


            SDL_RenderPresent(mRender);
        }

        void UpdateGame() {
            while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicks + 16));

            float dt = (SDL_GetTicks() - mTicks) / 1000.0f;
            mTicks = SDL_GetTicks();
            if (dt > 0.05f) dt = 0.05f; //clamp dt
            const int paddleH = thickness * 5;

            if (mPaddleDir != 0) {
                mPaddlePos.y += mPaddleDir * 300.0f * dt;

                if (mPaddlePos.y < ((paddleH / 2.0f) + thickness)) {
                    mPaddlePos.y = (paddleH / 2.0f) + thickness;
                }
                if (mPaddlePos.y > ((SCREEN_HEIGHT - thickness) - ((paddleH / 2.0f)))) {
                    mPaddlePos.y = (SCREEN_HEIGHT - thickness) - ((paddleH / 2.0f));
                }
            }

            if (mPaddleDir2 != 0) {
                mPaddlePos2.y += mPaddleDir2 * 300.0f * dt;

                if (mPaddlePos2.y < ((paddleH / 2.0f) + thickness)) {
                    mPaddlePos2.y = (paddleH / 2.0f) + thickness;
                }
                if (mPaddlePos2.y > ((SCREEN_HEIGHT - thickness) - ((paddleH / 2.0f)))) {
                    mPaddlePos2.y = (SCREEN_HEIGHT - thickness) - ((paddleH / 2.0f));
                }
            }

            if (mBallPos.y <= thickness && (mBallDir.y < 0.0f))
            {
                mBallDir.y *= -1;
            }
            else if (mBallPos.y >= SCREEN_HEIGHT - thickness && (mBallDir.y > 0.0f)) {
                mBallDir.y *= -1;
            }

            float distance = std::abs(mPaddlePos.y - mBallPos.y);
            if (distance <= paddleH / 2.0f &&
                mBallPos.x <= 25.0f && mBallPos.x >= 20.0f &&
                mBallDir.x < 0.0f) {
                mBallDir.x *= -1;
            }

            distance = std::abs(mPaddlePos2.y - mBallPos.y);
            if (distance <= paddleH / 2.0f &&
                mBallPos.x >= (SCREEN_WIDTH - 25.0f) && mBallPos.x <= (SCREEN_WIDTH - 20.0f) &&
                mBallDir.x > 0.0f) {
                mBallDir.x *= -1;
            }

            mBallPos.x += mBallDir.x * dt;
            mBallPos.y += mBallDir.y * dt;

            if (mBallPos.x < 0.0f || mBallPos.x > SCREEN_WIDTH) {
                //game over start again
                mBallPos = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
                mPaddlePos = { 10, SCREEN_HEIGHT / 2 };
                mPaddlePos2 = { SCREEN_WIDTH - 10 , SCREEN_HEIGHT / 2 };
                mBallDir = { -200.f, 235.0f };
            }
        }

        void RunLoop() {
            while (mIsRunning) {
                ProcessInput();

                UpdateGame();

                GenerateOutput();
            }
        }

        void Shutdown() {
            SDL_DestroyRenderer(mRender);
            SDL_DestroyWindow(mWindow);
            SDL_Quit();
        }

        struct vec2 {
            float x = 0.0f;
            float y = 0.0f;
        };

        vec2 mPaddlePos;
        vec2 mPaddlePos2;
        vec2 mBallPos;
        vec2 mBallDir;
        int mPaddleDir = 0;
        int mPaddleDir2 = 0;
        std::uint32_t mTicks = 0;
        const int thickness = 15;
        bool mIsRunning = false;
        SDL_Surface* mScreenSurface = NULL;
        SDL_Renderer* mRender = NULL;
        SDL_Window* mWindow = NULL;
    };
}


class Astriods : public minty::Game {
public:
    Astriods() {}
    virtual ~Astriods() {}
    std::vector<minty::Actor* > astriods;
    virtual bool LoadData() override {
        std::random_device rnd{};
        std::mt19937 mt(std::random_device{}());

        std::uniform_real_distribution<float> x(0.0f, 1024.0f);
        std::uniform_real_distribution<float> y(0.0f, 728.0f);
        std::uniform_real_distribution<float> rot(0.0f, glm::pi<float>());

        auto astriodTex = LoadTexture(fs::current_path() / "asserts"/ "astriod.png");
        constexpr int numAstrioid = 10;
        astriods.reserve(numAstrioid);
        for (int i = 0; i < numAstrioid; i++) {
            auto ac = new minty::Actor(this);
            ac->SetPos({ x(mt), y(mt) });

            auto spc = new minty::ColorComponent(ac);
            auto mvc = new minty::MoveComponent(ac);

            spc->SetHeight(50);
            spc->SetWidth(50);
            spc->SetColor(0x000000FF);
           // spc->SetTexture(astriodTex);
            mvc->mLinerSpeed = 90.0f;
            //mvc->mAngularSpeed = glm::pi<float>();
            mvc->mWrapScreen = true;
            ac->SetRotation(rot(mt));

            ac->AddComponents(spc);
            ac->AddComponents(mvc);

            astriods.emplace_back(ac);
            AddSprite(spc);
        }

        auto player = new minty::Actor(this);
        player->SetPos({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });
        auto col = new minty::ColorComponent(player, 1000);
        col->SetHeight(50);
        col->SetWidth(50);

        auto input = new minty::InputComponent(player);
        input->mForwardKey = SDL_SCANCODE_RIGHT;
        input->mBackwardKey = SDL_SCANCODE_LEFT;
        input->mClockwiseKey = SDL_SCANCODE_W;
        input->mCounterClockwiseKey = SDL_SCANCODE_S;
        input->mMaxLinerSpeed = 90;
        input->mMaxAngularSpeed = glm::pi<float>();

        player->AddComponents(col);
        player->AddComponents(input);

        AddSprite(col);
        return true;
    }
};



int main(int argc, char** argv)
{
    Astriods game;
    if (game.Init()) {
        if (game.LoadData()) {
             game.RunLoop();
        }
    }

    game.Shutdown();
    return 0;
}