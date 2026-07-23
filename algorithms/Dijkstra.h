#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <vector>

#include "../graph/Graph.h"

class Dijkstra
{
public:

    static std::vector<long long> shortestPath(
        const Graph &graph,
        long long source,
        long long destination,
        double &totalDistance);
};

#endif // DIJKSTRA_H