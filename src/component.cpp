#include "component.h"

minty::Component::Component(Actor* actor, std::uint32_t updateOrder)
: mActor(actor), mUpdateOrder(updateOrder){
}

minty::Component::~Component()
{
}

void minty::Component::Update(float deltatime)
{
}

void minty::Component::Draw(SDL_Renderer* render)
{
}
