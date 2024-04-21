#include "game.h"
#include "Actor.h"

minty::Game::Game() {

}

minty::Game::~Game() {

}

bool minty::Game::Init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        spdlog::error("SDL could not initialize! SDL_Error: {}\n", SDL_GetError());
        return false;
    }
    mWindow = SDL_CreateWindow("minty", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SKIP_TASKBAR);
    mScreenSize = glm::ivec2(SCREEN_WIDTH, SCREEN_HEIGHT);
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
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

    //Update the surface
    mIsRunning = true;
    SDL_UpdateWindowSurface(mWindow);
    return true;
}

void minty::Game::ProcessInput() {
    SDL_Event e;
    bool isResized = false;
    while (SDL_PollEvent(&e)) {
        switch (e.type)
        {
        case SDL_WINDOWEVENT_SIZE_CHANGED:
            isResized = true;
            break;
        case SDL_QUIT:
            mIsRunning = false;
        default:
            break;
        }
    }

    if (isResized) {
        isResized = false;
        int w, h;
        SDL_GetWindowSize(mWindow, &w, &h);
        mScreenSize = { w,h };
    }

    const std::uint8_t* state = SDL_GetKeyboardState(NULL);
    mUpdatingActor = true;
    for (auto& ac : mActors) {
        ac->ProcessInput(state);
    }
    mUpdatingActor = false;
}

void minty::Game::Update() {
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicks + 16));

    float dt = (SDL_GetTicks() - mTicks) / 1000.f;
    mTicks = SDL_GetTicks(); 

    mUpdatingActor = true;
    std::vector<Actor*> deads;
    for (auto actor : mActors) {
        actor->Update(dt);
      /*  if (actor->GetState() == minty::Actor::state::DEAD) {
            deads.emplace_back(actor);
        }*/
    }
    mUpdatingActor = false;

    for (auto pactor : mPendingActors) {
        mActors.emplace_back(pactor);
    }
    mPendingActors.clear();

    while (!deads.empty()) {
        delete deads.back();
        deads.pop_back();
    }
}

void minty::Game::Render() {
    //how to render
    SDL_SetRenderDrawColor(mRender, 0xFA, 0x3A, 0xFA, 0xFF);
    SDL_RenderClear(mRender);

    for (auto& sp : mSprites) {
        sp->Draw(mRender);
    }

    SDL_RenderPresent(mRender);
}

void minty::Game::Shutdown() {
    while (!mActors.empty()) {
        delete mActors.back();
        mActors.pop_back();
    }
    while (!mPendingActors.empty()) {
        delete mPendingActors.back();
        mPendingActors.pop_back();
    }
    IMG_Quit();
    SDL_DestroyRenderer(mRender);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}

void minty::Game::AddActor(Actor* ac)
{
    auto& put = mUpdatingActor ? mPendingActors : mActors;
    put.emplace_back(ac);
}

void minty::Game::RemvActor(Actor* ac)
{
    auto iter = std::find_if(mActors.begin(), mActors.end(), [&](Actor* a) -> bool {
        return a == ac;
    });
    if (iter != mActors.end()) mActors.erase(iter);
}

SDL_Texture* minty::Game::LoadTexture(const fs::path& filename)
{
    if (!fs::exists(filename)) {
        spdlog::error("No such file {}", filename.string());
        return nullptr;
    }
    SDL_Surface* sur = IMG_Load(filename.string().c_str());
    if (!sur) {
        spdlog::error("Failed loading surface from {}, SDL ERROR: {}", filename.string(), SDL_GetError());
        return nullptr;
    }
    auto text = SDL_CreateTextureFromSurface(mRender, sur);
    SDL_FreeSurface(sur);
    return text;
}

void minty::Game::RunLoop() {
    while (mIsRunning) {
        ProcessInput();

        Update();

        Render();
    }
}

void minty::Game::AddSprite(minty::SpriteComponent* sprite)
{
    auto iter = std::lower_bound(mSprites.begin(), mSprites.end(), sprite, [](minty::SpriteComponent* sp, minty::SpriteComponent* sprite) -> bool {
        return sp->GetDrawOrder() < sprite->GetDrawOrder();
    });
    if (iter == mSprites.end()) {
        mSprites.emplace_back(sprite);
    }
    else mSprites.insert(iter, sprite);
}
