#include "graph.h"

minty::Graph::Graph() {
	
}

minty::Graph::~Graph() {

}

bool minty::BFS(Graph& graph, GraphNode* start, GraphNode* end, NodeToPointerMap& outMap)
{
	if (graph.mNodes.empty() || !start || !end) return false;

	bool pathFound = false;
	std::queue<GraphNode*> q;
	q.push(start);

	while (!q.empty()) {
		GraphNode* cur = q.front();
		q.pop();
		if (cur == end) {
			pathFound = true;
			break;
		}

		for (auto adj : cur->mAjacentNodes) {
			const GraphNode* parent = outMap[adj];
			if (parent == nullptr && adj != start) {
				outMap[adj] = parent;
				q.push(adj);
			}
		}
	}
	return pathFound;
}

bool minty::GBFS(WeightedGraph& graph, WeightedNode* start, WeightedNode* end, GBFSMap& outMap)
{
	if (graph.mNodes.empty() || !start || !end) return false;
	std::vector<WeightedNode*> openSet;
	openSet.reserve(graph.mNodes.size());
	const WeightedNode* current = start;

	outMap[current].mInCloseSet = true;
	do {
		for (auto edge : current->mOutgoingEdges) {
			GBFSScratch& sc = outMap[edge->mTo];
			if (!sc.mInCloseSet) {
				sc.mParentEdge = edge;
				if (!sc.mInOpenSet) {
					sc.mHuristic = ComputeHusteric(edge->mTo, end);
					openSet.emplace_back(edge->mTo);
					sc.mInOpenSet = true;
				}
			}
		}
		if (openSet.empty()) break;
		auto iter = std::min_element(openSet.begin(), openSet.end(), 
			[&outMap](const WeightedNode* a,
			const WeightedNode* b) {
				return outMap[a].mHuristic < outMap[b].mHuristic;
			});
		current = *iter;
		openSet.erase(iter);
		outMap[current].mInOpenSet = false;
		outMap[current].mInCloseSet = true;
	} while (current != end);

	return (current == end);
}

float minty::ComputeHusteric(const WeightedNode* from, const WeightedNode* to)
{

	return 0.0f;
}

bool minty::Astar(WeightedGraph& graph, WeightedNode* start, WeightedNode* end, AstarMap& outMap)
{
	if (graph.mNodes.empty() || !start || !end) return false;
	std::vector<WeightedNode*> openSet;
	openSet.reserve(graph.mNodes.size());
	const WeightedNode* current = start;

	outMap[current].mInCloseSet = true;
	do {
		for (auto edge : current->mOutgoingEdges) {
			WeightedNode* neighbor = edge->mTo;
			AstarScratch& data = outMap[neighbor];
			if (!data.mInCloseSet) {
				if (!data.mInOpenSet) {
					data.mParentEdge = edge;
					data.mHuristic = ComputeHusteric(neighbor, end);
					data.mActualFromStart = outMap[current].mActualFromStart +
						edge->mWeight;
					data.mInOpenSet = true;
					openSet.emplace_back(neighbor);
				}
				else {
					//compute what the actual would be if the current becomes the parenet
					float newG = outMap[current].mActualFromStart + edge->mWeight;
					if (newG < data.mActualFromStart) {
						data.mActualFromStart = newG;
						data.mParentEdge = edge;
					}
				}
			}
		}
		// If open set is empty, all possible paths are exhausted
		if (openSet.empty()) break;

		// Find lowest cost node in open set
		auto iter = std::min_element(openSet.begin(), openSet.end(),
			[&outMap](const WeightedNode* a, const WeightedNode* b) {
				// Calculate f(x) for nodes a/b
				float fOfA = outMap[a].mHuristic + outMap[a].mActualFromStart;
		float fOfB = outMap[b].mHuristic + outMap[b].mActualFromStart;
		return fOfA < fOfB;
			});
		// Set to current and move from open to closed
		current = *iter;
		openSet.erase(iter);
		outMap[current].mInOpenSet = true;
		outMap[current].mInCloseSet = true;
	
	} while (current != end);
	return (current == end);
}
