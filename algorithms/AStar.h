#ifndef ASTAR_H
#define ASTAR_H

#include <vector>

#include "../graph/Graph.h"

class AStar
{
public:
    static std::vector<long long>
    shortestPath(
        Graph &graph,
        long long source,
        long long destination);
};

#endif // ASTAR_H