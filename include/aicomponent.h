#include "component.h"
#include <unordered_map>
#include <spdlog/spdlog.h>

namespace minty {

	class AiState {
	public:
		AiState(class AiComponent* owner);
		virtual ~AiState() {};
		
		virtual void Update(float deltaTime) = 0;
		virtual void OnEnter() = 0;
		virtual void OnExit() = 0;
		// Getter for string name of state
		virtual const char* GetName() const = 0;
	protected:
		class AiComponent* mOwner = NULL;
	};

	class AiComponent : public minty::Component
	{
	public:
		AiComponent(class Actor* acc, std::uint32_t updateOrder = 100);
		virtual ~AiComponent();

		virtual void Update(float deltatime) override;
		void ChangeState(const std::string& state);

		void RegisterState(AiState* state);
	private:
		std::unordered_map<std::string, AiState*> mStates;
		AiState* mCurState;
	};
}