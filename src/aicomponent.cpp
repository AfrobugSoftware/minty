#include "aicomponent.h"
#include "Actor.h"

minty::AiComponent::AiComponent(minty::Actor* acc, std::uint32_t updateOrder) 
	:minty::Component(acc, updateOrder){
}

minty::AiComponent::~AiComponent()
{
	if (!mStates.empty()) {
		for (auto& s : mStates) {
			delete s.second;
		}
		mStates.clear();
	}
}

void minty::AiComponent::Update(float deltatime)
{
	if (mCurState) mCurState->Update(deltatime);
}

void minty::AiComponent::ChangeState(const std::string& state)
{
	if (state.empty()) return;
	auto iter = mStates.find(state);
	if(iter == mStates.end()) {
		mCurState = nullptr;
		spdlog::error("Cannot find {}", state);
		return;
	}
	if (mCurState) mCurState->OnExit();
	mCurState = iter->second;
	iter->second->OnEnter();
}

void minty::AiComponent::RegisterState(AiState* state)
{
	mStates.emplace(state->GetName(), state);
}

minty::AiState::AiState(AiComponent* owner)
	: mOwner{owner} {
}
