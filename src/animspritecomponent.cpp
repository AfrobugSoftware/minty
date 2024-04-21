#include "animspritecomponent.h"
#include "Actor.h"

minty::AnimSpriteComponent::AnimSpriteComponent(minty::Actor* actor,
	std::uint32_t drawOrder)
	:minty::SpriteComponent(actor, drawOrder){
	}
minty::AnimSpriteComponent::~AnimSpriteComponent() {
	//who has ownership of the textures in the 
}

void minty::AnimSpriteComponent::Update(float deltatime)
{
	SpriteComponent::Update(deltatime);
	if (mSprites.empty()) return;

	mCurrFrame = mAnimFPS * deltatime; //update current frame based on the framerate
	while (mCurrFrame >= mSprites.size()) { mCurrFrame -= mSprites.size();}
	SetTexture(mSprites[static_cast<int>(mCurrFrame)]);
}

void minty::AnimSpriteComponent::SetAnimTextures(std::vector<SDL_Texture*>&& texs)
{
	mSprites = std::move(texs); 
	mCurrFrame = 0.0f;
	if (!mSprites.empty()) {
		SetTexture(mSprites.front());
	}
}

minty::BGSpriteComponent::BGSpriteComponent(Actor* mOWner, std::uint32_t drawOrder)
: SpriteComponent(mOWner, drawOrder){
}

minty::BGSpriteComponent::~BGSpriteComponent()
{
}

void minty::BGSpriteComponent::Update(float deltatime)
{
	SpriteComponent::Update(deltatime);
	for (auto& bg : mSprites) {
		bg.offset.x -= mScrollSpeed * deltatime;

		if (bg.offset.x < -mScreenSize.x) {
			bg.offset.x = (mSprites.size() - 1) * mScreenSize.x - 1;
		}
	}
}

void minty::BGSpriteComponent::Draw(SDL_Renderer* render)
{
	if (mSprites.empty()) return;
	for (auto& bg : mSprites) {
		SDL_Rect r;
		r.w = static_cast<int>(mScreenSize.x);
		r.h = static_cast<int>(mScreenSize.y);
		r.x = static_cast<int>(mActor->GetPos().x - static_cast<float>(r.w / 2)) + bg.offset.x;
		r.y = static_cast<int>(mActor->GetPos().y - static_cast<float>(r.h / 2)) + bg.offset.y;

		SDL_RenderCopy(render, bg.tex, nullptr, &r);
	}
}

void minty::BGSpriteComponent::SetBGTextures(const std::vector<SDL_Texture*>& texs)
{
	int count = 0;
	for (auto& tex : texs) {
		BGTexture bgt;
		bgt.tex = tex;
		glm::vec2 off;
		bgt.offset.x = count * mScreenSize.x;
		bgt.offset.y = 0;
		
		mSprites.emplace_back(std::move(bgt));
		count++;
	}
}
