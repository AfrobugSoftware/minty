#pragma
#include "game.h"

namespace minty {
	class TowerGame : public minty::Game
	{
	public:
		TowerGame();
		virtual ~TowerGame();
		class Grid* GetGrid() { return mGrid; }
		std::vector<class Enemy*>& GetEnemies() { return mEnemies; }
		class Enemy* GetNearestEnemy(const glm::vec2& pos);
		virtual bool LoadData() override;
	private:
		class Grid* mGrid = nullptr;
		std::vector<class Enemy*> mEnemies;
		float mNextEnemy = 0.0f;
	};
};