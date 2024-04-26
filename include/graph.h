#pragma once
#include <vector>
#include <algorithm>
#include <iterator>
#include <unordered_map>
#include <queue>
#include <glm/glm.hpp>

namespace minty {
	struct GraphNode {
		glm::vec2 mPos;
		std::vector<GraphNode*> mAjacentNodes;
	};

	class Graph {
	public:
		Graph();
		~Graph();
		
		std::vector<GraphNode*> mNodes;
	};

	struct WeightedEgde {
		class WeightedNode* mFrom = NULL;
		class WeightedNode* mTo = NULL;
		float mWeight = 0.0f;
	};

	struct WeightedNode {
		glm::vec2 mPos;
		std::vector<WeightedEgde*> mOutgoingEdges;
	};

	struct WeightedGraph {
		std::vector<WeightedNode*> mNodes;
	};
	using NodeToPointerMap = std::unordered_map<const GraphNode*,
		const GraphNode*>;

	struct GBFSScratch {
		const WeightedEgde* mParentEdge = NULL;
		float mHuristic = 0.0f;
		bool mInOpenSet = false;
		bool mInCloseSet = false;
	};

	struct AstarScratch : public GBFSScratch {
		float mActualFromStart = 0.0f;
	};

	using GBFSMap = std::unordered_map<const WeightedNode*, GBFSScratch>;
	using AstarMap = std::unordered_map<const WeightedNode*, AstarScratch>;

	bool BFS(Graph& graph, GraphNode* start, GraphNode* end, NodeToPointerMap& outMap);
	bool GBFS(WeightedGraph& graph, WeightedNode* start, WeightedNode* end, GBFSMap& outMap);
	float ComputeHusteric(const WeightedNode* from, const WeightedNode* to);
	bool Astar(WeightedGraph& graph, WeightedNode* start, WeightedNode* end, AstarMap& outMap);
};