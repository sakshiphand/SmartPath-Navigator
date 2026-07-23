#include "AStar.h"

#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <limits>
#include <algorithm>
#include <cmath>

// =====================================================
// A* SHORTEST PATH
// =====================================================

std::vector<long long> AStar::shortestPath(
    Graph &graph,
    long long source,
    long long destination)
{
    std::vector<long long> path;

    // -------------------------------------------------
    // Validate source and destination
    // -------------------------------------------------

    if (!graph.hasNode(source) ||
        !graph.hasNode(destination))
    {
        return path;
    }

    if (source == destination)
    {
        path.push_back(source);
        return path;
    }

    // -------------------------------------------------
    // Priority queue entry
    //
    // first  = fScore
    // second = node ID
    // -------------------------------------------------

    using QueueEntry =
        std::pair<double, long long>;

    std::priority_queue<
        QueueEntry,
        std::vector<QueueEntry>,
        std::greater<QueueEntry>>
        openSet;

    // -------------------------------------------------
    // A* data structures
    // -------------------------------------------------

    std::unordered_map<long long, double>
        gScore;

    std::unordered_map<long long, double>
        fScore;

    std::unordered_map<long long, long long>
        previous;

    std::unordered_set<long long>
        closedSet;

    const double INF =
        std::numeric_limits<double>::infinity();

    // -------------------------------------------------
    // Initialize
    // -------------------------------------------------

    gScore[source] = 0.0;

    double initialHeuristic =
        graph.calculateDistance(
            source,
            destination);

    fScore[source] =
        initialHeuristic;

    openSet.push(
        {initialHeuristic, source});

    const auto &adjacency =
        graph.getAdjacencyList();

    // -------------------------------------------------
    // A* main loop
    // -------------------------------------------------

    while (!openSet.empty())
    {
        long long current =
            openSet.top().second;

        openSet.pop();

        // Already processed
        if (closedSet.find(current)
            != closedSet.end())
        {
            continue;
        }

        // Destination reached
        if (current == destination)
        {
            break;
        }

        closedSet.insert(current);

        auto adjacencyIt =
            adjacency.find(current);

        if (adjacencyIt ==
            adjacency.end())
        {
            continue;
        }

        // ---------------------------------------------
        // Explore neighbours
        // ---------------------------------------------

        for (const auto &edge :
             adjacencyIt->second)
        {
            long long neighbor =
                edge.first;

            double edgeWeight =
                edge.second;

            if (closedSet.find(neighbor)
                != closedSet.end())
            {
                continue;
            }

            double currentGScore = INF;

            auto currentScoreIt =
                gScore.find(current);

            if (currentScoreIt !=
                gScore.end())
            {
                currentGScore =
                    currentScoreIt->second;
            }

            double tentativeGScore =
                currentGScore +
                edgeWeight;

            double neighborGScore = INF;

            auto neighborScoreIt =
                gScore.find(neighbor);

            if (neighborScoreIt !=
                gScore.end())
            {
                neighborGScore =
                    neighborScoreIt->second;
            }

            // -----------------------------------------
            // Better path found
            // -----------------------------------------

            if (tentativeGScore <
                neighborGScore)
            {
                previous[neighbor] =
                    current;

                gScore[neighbor] =
                    tentativeGScore;

                double heuristic =
                    graph.calculateDistance(
                        neighbor,
                        destination);

                double totalScore =
                    tentativeGScore +
                    heuristic;

                fScore[neighbor] =
                    totalScore;

                openSet.push(
                    {totalScore,
                     neighbor});
            }
        }
    }

    // -------------------------------------------------
    // Check whether destination was reached
    // -------------------------------------------------

    if (previous.find(destination) ==
        previous.end())
    {
        return path;
    }

    // -------------------------------------------------
    // Reconstruct path
    // -------------------------------------------------

    long long current =
        destination;

    path.push_back(current);

    while (current != source)
    {
        auto it =
            previous.find(current);

        if (it == previous.end())
        {
            path.clear();
            return path;
        }

        current =
            it->second;

        path.push_back(current);
    }

    std::reverse(
        path.begin(),
        path.end());

    return path;
}