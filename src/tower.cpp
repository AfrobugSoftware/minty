#include "tower.h"
#include "towergame.h"

minty::Enemy::Enemy(Game* game)
: minty::Actor(game){
	//might slow the creation of enemies, leave for now,
	//if bottle neck remove
	minty::TowerGame* tgame = dynamic_cast<minty::TowerGame*>(game);
	if (!game) {
		spdlog::critical("Cannot create enemy");
		throw std::logic_error("Cannot cast to tower game");
	}
	auto& enemies = tgame->GetEnemies();
	enemies.emplace_back(this);
	minty::SpriteComponent* sc = new minty::SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Airplane"));
	AddComponents(sc);
	mCurrentNode = tgame->GetGrid()->GetStartTile();
	mNextNode = mCurrentNode->GetParent();

	nv = new minty::NavComponent(this);
	nv->SetForwardSpeed(150.0f);
	SetPos(mCurrentNode->GetPos());
	nv->StartPath(mNextNode->GetPos());


	AddComponents(nv);
	mBoundVolume->radius = 25.0f;
}

minty::Enemy::~Enemy()
{
	// Remove from enemy vector
	minty::TowerGame* tgame = dynamic_cast<minty::TowerGame*>(GetGame());
	auto& ene = tgame->GetEnemies();
	auto iter = std::find(ene.begin(), ene.end(),this);
	ene.erase(iter);
}

void minty::Enemy::UpdateActor(float deltatime)
{
	Actor::UpdateActor(deltatime);
	minty::TowerGame* tgame = dynamic_cast<minty::TowerGame*>(GetGame());
	// Am I near the end tile?
	glm::vec2 diff = GetPos() - tgame->GetGrid()->GetEndTile()->GetPos();
	
	if (glm::length(diff) <= 10.0f)
	{
		SetState(minty::Actor::state::DEAD);
	}
	if (mNextNode) {
		glm::vec2 diff = GetPos() - mNextNode->GetPos();
		if (glm::length(diff) <= 2.0f) {
			nv->mNextPos = mNextNode->GetPos();
			mCurrentNode = mNextNode;
			mNextNode = mCurrentNode->GetParent();
			if (!mNextNode) nv->TurnTo(tgame->GetGrid()->GetEndTile()->GetPos());
			else nv->TurnTo(mNextNode->GetPos());
		}
	}

}

minty::Tile::Tile(Game* game)
: minty::Actor(game){
	mSprite = new SpriteComponent(this);
	AddComponents(mSprite);
	UpdateTexture();
}

minty::Tile::~Tile()
{
}

void minty::Tile::SetTileState(TileState state)
{
	mTileState = state;
	UpdateTexture();
}

void minty::Tile::ToggleSelect()
{
	mSelected = !mSelected;
	UpdateTexture();
}

void minty::Tile::UpdateActor(float deltatime)
{
	minty::Actor::UpdateActor(deltatime);
}

void minty::Tile::UpdateTexture()
{
	std::string text;
	switch (mTileState)
	{
	case minty::Tile::EPath:
		if (mSelected)
		{
			text = "TileGreySelected"s;
		}
		else {
			text = "TileGrey"s;
		}
		break;
	case minty::Tile::EStart:
		text = "TileTan"s;
		break;
	case minty::Tile::EBase:
		text = "TileGreen"s;
		break;
	case minty::Tile::EDefault:
	default:
		if (mSelected) {
			text = "TileBrownSelected"s;
		}
		else {
			text = "TileBrown"s;
		}
		break;
	}
	mSprite->SetTexture(GetGame()->GetTexture(text));
}

minty::Tower::Tower(Game* game)
: minty::Actor(game){
	SpriteComponent* sc = new SpriteComponent(this, 200);
	sc->SetTexture(game->GetTexture("Tower"s));

	AddComponents(sc);
	mMove = new MoveComponent(this);
	AddComponents(mMove);
	mNextAttack = AttackTime;
}

minty::Tower::~Tower()
{

}

void minty::Tower::UpdateActor(float deltatime)
{
	minty::TowerGame* tgame = dynamic_cast<minty::TowerGame*>(GetGame());
	mNextAttack -= deltatime;
	if (mNextAttack <= 0.0f)
	{
		Enemy* e = tgame->GetNearestEnemy(GetPos());
		if (e != nullptr)
		{
			// Vector from me to enemy
			glm::vec2 dir = e->GetPos() - GetPos();
			float dist = glm::length(dir);
			if (dist < AttackRange)
			{
				SetRotation(std::atan2(-dir.y, dir.x));
				Bullet* b = new Bullet(GetGame());
				b->SetPos(GetPos());
				b->SetRotation(GetRotation());
			}
		}
		mNextAttack += AttackTime;
	}
}

minty::Bullet::Bullet(Game* game)
: minty::Actor(game){
	minty::SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Missile"s));
	AddComponents(sc);

	MoveComponent* mc = new MoveComponent(this);
	mc->mLinerSpeed = 400.0f;
	AddComponents(mc);

	mBoundVolume->radius = 5.0f;
}

void minty::Bullet::UpdateActor(float deltatime)
{
	minty::Actor::UpdateActor(deltatime);
	minty::TowerGame* tgame = dynamic_cast<minty::TowerGame*>(GetGame());

	// Check for collision vs enemies
	for (Enemy* e : tgame->GetEnemies())
	{
		if (Intersects(e->GetBound()))
		{
			// We both die on collision
			e->SetState(minty::Actor::state::DEAD);
			SetState(minty::Actor::state::DEAD);
			break;
		}
	}

	mLiveTime -= deltatime;
	if (mLiveTime <= 0.0f)
	{
		// Time limit hit, die
		SetState(minty::Actor::state::DEAD);
	}
}

minty::Grid::Grid(Game* game)
: minty::Actor(game){
	mTiles.resize(NumRows);
	for (size_t i = 0; i < mTiles.size(); i++)
	{
		mTiles[i].resize(NumCols);
	}

	// Create tiles
	for (size_t i = 0; i < NumRows; i++)
	{
		for (size_t j = 0; j < NumCols; j++)
		{
			mTiles[i][j] = new Tile(GetGame());
			mTiles[i][j]->SetPos(glm::vec2(TileSize / 2.0f + j * TileSize, StartY + i * TileSize));
		}
	}
	mStartTile = mTiles[3][0];
	mEndTile = mTiles[3][15];

	mStartLocation = { 3, 0 };
	mEndLocation = { 3, 15 };
	// Set start/end tiles
	GetStartTile()->SetTileState(Tile::EStart);
	GetEndTile()->SetTileState(Tile::EBase);

	// Set up adjacency lists
	for (size_t i = 0; i < NumRows; i++)
	{
		for (size_t j = 0; j < NumCols; j++)
		{
			if (i > 0)
			{
				mTiles[i][j]->mAdjacent.push_back(mTiles[i - 1][j]);
			}
			if (i < NumRows - 1)
			{
				mTiles[i][j]->mAdjacent.push_back(mTiles[i + 1][j]);
			}
			if (j > 0)
			{
				mTiles[i][j]->mAdjacent.push_back(mTiles[i][j - 1]);
			}
			if (j < NumCols - 1)
			{
				mTiles[i][j]->mAdjacent.push_back(mTiles[i][j + 1]);
			}
		}
	}

	// Find path (in reverse)
	FindPath(GetEndTile(), GetStartTile());
	UpdatePathTiles(GetStartTile());

	
	mNextEnemy = EnemyTime;
}

minty::Grid::~Grid()
{
}

void minty::Grid::ProcessClick(int x, int y)
{
	y -= static_cast<int>(StartY - TileSize / 2);
	if (y >= 0)
	{
		x /= static_cast<int>(TileSize);
		y /= static_cast<int>(TileSize);
		if (x >= 0 && x < static_cast<int>(NumCols) && y >= 0 && y < static_cast<int>(NumRows))
		{
			SelectTile(y, x);
		}
	}
}

bool minty::Grid::FindPath(Tile* start, Tile* goal)
{
	for (size_t i = 0; i < NumRows; i++)
	{
		for (size_t j = 0; j < NumCols; j++)
		{
			mTiles[i][j]->mScratch.mActualFromStart = 0.0f;
			mTiles[i][j]->mScratch.mInOpenSet = false;
			mTiles[i][j]->mScratch.mInCloseSet = false;
			mTiles[i][j]->mParent = NULL;
		}
	}
	std::vector<Tile*> openSet;
	openSet.reserve(mTiles.size());
	Tile* current = start;
	current->mScratch.mInCloseSet = true;
	do {
		for (auto neighbor : current->mAdjacent)
		{
			if (neighbor->mBlocked)
			{
				continue;
			}
			if (!neighbor->mScratch.mInCloseSet) {
				if (!neighbor->mScratch.mInOpenSet) {
					neighbor->mParent = current;
					neighbor->mScratch.mHuristic = glm::distance(neighbor->GetPos(), goal->GetPos());
					neighbor->mScratch.mActualFromStart = current->mScratch.mActualFromStart + TileSize;
					neighbor->f = neighbor->mScratch.mActualFromStart + neighbor->mScratch.mHuristic;

					openSet.emplace_back(neighbor);
					neighbor->mScratch.mInOpenSet = true;
				}
				else {
					//in the open set, check if current can be its parent
					float newG = current->mScratch.mActualFromStart + TileSize;
					if (newG < neighbor->mScratch.mActualFromStart) {
						neighbor->mParent = current;
						neighbor->mScratch.mActualFromStart = newG;
						neighbor->f = neighbor->mScratch.mActualFromStart + neighbor->mScratch.mHuristic;

					}
				}
			}
		}

		if (openSet.empty()) break; //no more neighbors
		auto iter = std::ranges::min_element(openSet, [](Tile* a, Tile* b) -> bool {
			return a->GetF() < b->GetF();
			});
		current = *iter;
		openSet.erase(iter);
		current->mScratch.mInOpenSet = false;
		current->mScratch.mInCloseSet = true;
			
	}while (current != goal);
	return (current == goal);
}

void minty::Grid::BuildTower()
{
	if (mSelectedTile && !mSelectedTile->mBlocked)
	{
		mSelectedTile->mBlocked = true;
		if (FindPath(GetEndTile(), GetStartTile()))
		{
			Tower* t = new Tower(GetGame());
			t->SetPos(mSelectedTile->GetPos());
		}
		else
		{
			// This tower would block the path, so don't allow build
			mSelectedTile->mBlocked = false;
			FindPath(GetEndTile(), GetStartTile());
		}
		UpdatePathTiles(GetStartTile());
	}
}

void minty::Grid::MakeStart()
{
	if (mSelectedTile && !mSelectedTile->mBlocked) {
		mStartTile = mSelectedTile;
		//mStartTile->mBlocked = true;
		FindPath(mEndTile, mStartTile);

		mStartLocation = mLoc;
		UpdatePathTiles(mStartTile);
	}
}

void minty::Grid::MakeEnd()
{
	if (mSelectedTile && !mSelectedTile->mBlocked) {
		mEndTile = mSelectedTile;
		//mEndTile->mBlocked = true;
		FindPath(mEndTile, mStartTile);

		mEndLocation = mLoc;
		UpdatePathTiles(mStartTile);
	}
}

minty::Tile* minty::Grid::GetStartTile()
{
	return mStartTile;
}

minty::Tile* minty::Grid::GetEndTile()
{
	return mEndTile;
}

void minty::Grid::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	// Is it time to spawn a new enemy?
	mNextEnemy -= deltaTime;
	if (mNextEnemy <= 0.0f)
	{
		new Enemy(GetGame());
		mNextEnemy += EnemyTime;
	}
}

void minty::Grid::ActorInput(const std::uint8_t* state)
{
	if (!state) return;
	if (state[SDL_SCANCODE_B]) {
		BuildTower();
	}
	if (state[SDL_SCANCODE_Z]) {
		MakeStart();
	}
	if (state[SDL_SCANCODE_X]) {
		MakeEnd();
	}
}

void minty::Grid::ActorMouseInput(const float x, const float y)
{
	ProcessClick(static_cast<int>(x), static_cast<int>(y));
}

void minty::Grid::SelectTile(size_t row, size_t col)
{
	// Make sure it's a valid selection
	Tile::TileState tstate = mTiles[row][col]->GetTileState();
	if (tstate != Tile::EStart && tstate != Tile::EBase)
	{
		// Deselect previous one
		if (mSelectedTile)
		{
			mSelectedTile->ToggleSelect();
		}
		mSelectedTile = mTiles[row][col];
		mSelectedTile->ToggleSelect();
	}
	mLoc = { row, col };
}

void minty::Grid::UpdatePathTiles(Tile* start)
{
	// Reset all tiles to normal (except for start/end)
	for (size_t i = 0; i < NumRows; i++)
	{
		for (size_t j = 0; j < NumCols; j++)
		{
			if (!(i == mStartLocation.x && j == mStartLocation.y) && !(i == mEndLocation.x && j == mEndLocation.y))
			{
				mTiles[i][j]->SetTileState(Tile::EDefault);
			}
			else if ((i == mStartLocation.x && j == mStartLocation.y))
			{
				mTiles[i][j]->SetTileState(Tile::EStart);
			}
			else if ((i == mEndLocation.x && j == mEndLocation.y))
			{
				mTiles[i][j]->SetTileState(Tile::EBase);
			}
		}
	}

	Tile* t = start->mParent;
	while (t != GetEndTile())
	{
		t->SetTileState(Tile::EPath);
		t = t->mParent;
	}
}
