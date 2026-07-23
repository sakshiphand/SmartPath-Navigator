#include "Dijkstra.h"

#include <queue>
#include <unordered_map>
#include <limits>
#include <algorithm>
#include <functional>

using namespace std;

vector<long long> Dijkstra::shortestPath(
    const Graph &graph,
    long long source,
    long long destination,
    double &totalDistance)
{
    vector<long long> path;

    totalDistance = 0.0;

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

    const auto &adj =
        graph.getAdjacencyList();

    if (adj.find(source) == adj.end() ||
        adj.find(destination) == adj.end())
    {
        return path;
    }

    unordered_map<long long, double> distance;
    unordered_map<long long, long long> previous;

    using QueueItem =
        pair<double, long long>;

    priority_queue<
        QueueItem,
        vector<QueueItem>,
        greater<QueueItem>
        > pq;

    distance[source] = 0.0;

    pq.push({0.0, source});

    while (!pq.empty())
    {
        double currentDistance =
            pq.top().first;

        long long current =
            pq.top().second;

        pq.pop();

        auto currentDistanceIt =
            distance.find(current);

        if (currentDistanceIt == distance.end())
            continue;

        if (currentDistance >
            currentDistanceIt->second)
        {
            continue;
        }

        if (current == destination)
            break;

        auto adjIt = adj.find(current);

        if (adjIt == adj.end())
            continue;

        for (const auto &edge : adjIt->second)
        {
            long long neighbour =
                edge.first;

            double weight =
                edge.second;

            double newDistance =
                currentDistance + weight;

            auto neighbourIt =
                distance.find(neighbour);

            if (neighbourIt == distance.end() ||
                newDistance < neighbourIt->second)
            {
                distance[neighbour] =
                    newDistance;

                previous[neighbour] =
                    current;

                pq.push({
                    newDistance,
                    neighbour
                });
            }
        }
    }

    auto destinationIt =
        distance.find(destination);

    if (destinationIt == distance.end())
        return path;

    totalDistance =
        destinationIt->second;

    long long current =
        destination;

    while (true)
    {
        path.push_back(current);

        if (current == source)
            break;

        auto previousIt =
            previous.find(current);

        if (previousIt == previous.end())
        {
            path.clear();
            totalDistance = 0.0;

            return path;
        }

        current =
            previousIt->second;
    }

    reverse(path.begin(), path.end());

    return path;
}