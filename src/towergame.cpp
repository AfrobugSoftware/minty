#include "towergame.h"
#include "tower.h"

minty::TowerGame::TowerGame()
{
	mScreenSize = { 16 * 64, 7 * 64 };
}

minty::TowerGame::~TowerGame()
{
}

minty::Enemy* minty::TowerGame::GetNearestEnemy(const glm::vec2& pos)
{
	minty::Enemy* best = nullptr;

	if (mEnemies.size() > 0)
	{
		best = mEnemies[0];
		float bestDist = glm::distance(pos, mEnemies[0]->GetPos());
		for (size_t i = 1; i < mEnemies.size(); i++)
		{
			float newDist = glm::distance(pos, mEnemies[i]->GetPos());
			if (newDist < bestDist)
			{
				bestDist = newDist;
				best = mEnemies[i];
			}
		}
	}

	return best;
}

bool minty::TowerGame::LoadData()
{
	//load the textures
	auto path = fs::current_path() / "asserts";
	(void)LoadTexture(path / "Airplane.png");
	(void)LoadTexture(path / "TileTan.png");
	(void)LoadTexture(path / "TileTanSelected.png");
	(void)LoadTexture(path / "TileBrown.png");
	(void)LoadTexture(path / "TileBrownSelected.png");
	(void)LoadTexture(path / "TileGreen.png");
	(void)LoadTexture(path / "TileGreenSelected.png");
	(void)LoadTexture(path / "TileGreySelected.png");
	(void)LoadTexture(path / "TileGrey.png");
	(void)LoadTexture(path / "Projectile.png");
	(void)LoadTexture(path / "Tower.png");
	(void)LoadTexture(path / "Base.png");
	(void)LoadTexture(path / "Missile.png");


	mGrid = new minty::Grid(this);
	return true;
}
