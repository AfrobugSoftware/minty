#include "spritecomponent.h"
#include "Actor.h"

minty::SpriteComponent::SpriteComponent(minty::Actor* owner, std::uint32_t drawOrder)
: minty::Component(owner), mDrawOrder(drawOrder) {

}

minty::SpriteComponent::~SpriteComponent() {
}

void minty::SpriteComponent::Draw(SDL_Renderer* render)
{
	if (!mTexture) return;
	SDL_Rect r{};

	r.w = static_cast<int>(mWidth * mActor->GetScale());
	r.h = static_cast<int>(mHeight * mActor->GetScale());
	r.x = static_cast<int>(mActor->GetPos().x - static_cast<float>(r.w / 2));
	r.y = static_cast<int>(mActor->GetPos().y - static_cast<float>(r.h / 2));

	//draw the texture
	SDL_RenderCopyEx(render, mTexture, nullptr, &r, -glm::degrees(mActor->GetRotation()), nullptr, SDL_FLIP_NONE);
}

void minty::SpriteComponent::SetTexture(SDL_Texture* texture)
{
	mTexture = texture;
	SDL_QueryTexture(texture, &mFormat, nullptr, &mWidth, &mHeight);
}

void minty::SpriteComponent::Update(float deltatime)
{
}

minty::ColorComponent::ColorComponent(Actor* owner, std::uint32_t drawOrder)
: minty::SpriteComponent(owner, drawOrder){
}

void minty::ColorComponent::Draw(SDL_Renderer* render)
{
	SDL_Rect r{};

	r.w = static_cast<int>(mWidth * mActor->GetScale());
	r.h = static_cast<int>(mHeight * mActor->GetScale());
	r.x = static_cast<int>(mActor->GetPos().x - static_cast<float>(r.w / 2));
	r.y = static_cast<int>(mActor->GetPos().y - static_cast<float>(r.h / 2));

	auto f = mActor->GetForward() * (static_cast<float>(mWidth) / 2 );
	f += mActor->GetPos();
	//draw the texture
	std::uint8_t* c = reinterpret_cast<std::uint8_t*>(&color);
	SDL_SetRenderDrawColor(render, c[3], c[2], c[1], c[0]);
	SDL_RenderFillRect(render, &r);

	SDL_SetRenderDrawColor(render, 0xFF, 0x00, 0x00, 0xFF);
	SDL_RenderDrawLine(render, mActor->GetPos().x, mActor->GetPos().y, f.x, f.y);

}
