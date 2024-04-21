#include "Actor.h"
#include "game.h"
#include "component.h"

minty::Actor::Actor(Game* game)
	: mGame(game){
	game->AddActor(this); //must be created on the heap, the game gets ownership of the actor
	mBoundVolume = new BoundVolume;
	mBoundVolume->center = { 0.0f, 0.0f };
}

minty::Actor::~Actor()
{
}

void minty::Actor::Update(float dt)
{
	UpdateActor(dt);
	UpdateComponents(dt);
}

void minty::Actor::UpdateComponents(float deltatime)
{
	for (auto& comp : mComponents) {
		comp->Update(deltatime);
	}
}

void minty::Actor::UpdateActor(float deltaTime)
{
}

void minty::Actor::AddComponents(Component* comm)
{
	auto iter = std::lower_bound(mComponents.begin(), mComponents.end(),
		comm, [](Component* com, Component* cc) ->bool {
			return com->GetUpdateOrder() < cc->GetUpdateOrder();
		});
	if (iter == mComponents.end()) {
		mComponents.emplace_back(comm);
	}
	else mComponents.insert(iter, comm);
}

void minty::Actor::RemvComponents(Component* comm)
{
	auto iter = std::find_if(mComponents.begin(), mComponents.end(), [&](Component* p) ->bool {return p == comm; });
	if (iter != mComponents.end()) {
		mComponents.erase(iter);
	}
}

void minty::Actor::ProcessInput(const std::uint8_t* state)
{
	if (mState == state::ACTIVE) {
		for (auto& com : mComponents) {
			com->ProcessInput(state);
		}
		ActorInput(state);
	}
}

glm::vec2 minty::Actor::GetForward() const
{
	//rotation is in radianss
	//+y is up in cirle but down in SDL
	return glm::vec2( glm::cos(rotation), -glm::sin(rotation) );
}

void minty::Actor::SetRotation(const glm::vec2& v)
{
	rotation = std::atan2(-v.y, v.x); 
}

bool minty::Actor::Intersects(const BoundVolume* bv) const
{
	return mBoundVolume->Intersects(bv);
}

minty::BoundVolume::BoundVolume()
	: center{}, radius{0.0f} {
}

minty::BoundVolume::~BoundVolume()
{
}

bool minty::BoundVolume::Intersects(const BoundVolume* bv) const
{
	//assume base is a circle
	const auto v = center - bv->center;
	const float radii = radius + bv->radius;
	return (glm::length(v) <= radii);
}
