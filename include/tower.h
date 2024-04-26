#pragma once
#include "Actor.h"
#include "movecomponent.h"
#include "spritecomponent.h"
#include "graph.h"
#include <string>
using namespace std::string_literals;

namespace minty {
	class Bullet : public Actor
	{
	public:
		Bullet(Game* game);
		virtual ~Bullet() {};
		virtual void UpdateActor(float deltatime) override;

		float mLiveTime = 5.0f;
	};


	class Enemy : public Actor
	{
	public:
		Enemy(Game* game);
		virtual ~Enemy();
		virtual void UpdateActor(float deltatime) override;
		class Tile* mCurrentNode = NULL;
		class Tile* mNextNode = NULL;
		minty::NavComponent* nv = NULL;

	};

	class Tower : public Actor
	{
	public:
		Tower(Game* game);
		virtual ~Tower();

		virtual void UpdateActor(float deltatime) override;
		MoveComponent* mMove;
		float mNextAttack;
		const float AttackTime = 2.5f;
		const float AttackRange = 150.0f;
	};

	class Tile : public Actor
	{
	public:
		Tile(Game* game);
		virtual ~Tile();

		friend class Grid;
		enum TileState
		{
			EDefault,
			EPath,
			EStart,
			EBase
		};
		void SetTileState(TileState state);
		inline constexpr TileState GetTileState() const { return mTileState; }
		void ToggleSelect();
		inline constexpr const Tile* GetParent() const { return mParent; }
		inline constexpr Tile* GetParent() { return mParent; }
		virtual void UpdateActor(float deltatime) override;
		inline AstarScratch& GetScratch() { mScratch; }
		inline constexpr const float GetF() const { return f; }
	private:
		float f = 0.0f;
		float g = 0.0f;
		float h = 0.0f;
		
		Tile* mParent = nullptr;
		TileState mTileState = EDefault;
		AstarScratch mScratch;
		bool mBlocked = false;


		void UpdateTexture();
		class SpriteComponent* mSprite = nullptr;
		bool mSelected = false;
		std::vector<Tile*> mAdjacent;
	};

	class Grid : public Actor
	{	
	public:
		Grid(Game* game);
		virtual ~Grid();

		void ProcessClick(int x, int y);

		bool FindPath(class Tile* start, class Tile* goal);

		void BuildTower();
		void MakeStart();
		void MakeEnd();

		class Tile* GetStartTile();
		class Tile* GetEndTile();

		virtual void UpdateActor(float deltaTime) override;
		virtual void ActorInput(const std::uint8_t* state) override;
		virtual void ActorMouseInput(const float x, const float y) override;

	private:
		// Select a specific tile
		void SelectTile(size_t row, size_t col);

		void UpdatePathTiles(Tile* start);

		Tile* mSelectedTile = nullptr;

		std::vector<std::vector<class Tile*>> mTiles;

		float mNextEnemy = 0.0f;

		const size_t NumRows = 7;
		const size_t NumCols = 16;

		Tile* mStartTile = NULL;
		Tile* mEndTile = NULL;
		glm::ivec2 mStartLocation;
		glm::ivec2 mEndLocation;
		glm::ivec2 mLoc;

		constexpr static const float StartY = 50.0f;
		constexpr static const float TileSize = 64.0f;
		constexpr static const float EnemyTime = 1.5f;
	};


};