#include "Graph.h"

#include <cmath>
#include <algorithm>
#include <cctype>
#include <limits>
#include <queue>

using namespace std;


// =====================================================
// NORMALIZE PLACE NAME
// =====================================================

string Graph::normalizePlaceName(
    const string &name) const
{
    string normalized = name;

    transform(
        normalized.begin(),
        normalized.end(),
        normalized.begin(),
        [](unsigned char c)
        {
            return static_cast<char>(
                tolower(c));
        });

    // Remove leading spaces
    normalized.erase(
        normalized.begin(),
        find_if(
            normalized.begin(),
            normalized.end(),
            [](unsigned char c)
            {
                return !isspace(c);
            }));

    // Remove trailing spaces
    normalized.erase(
        find_if(
            normalized.rbegin(),
            normalized.rend(),
            [](unsigned char c)
            {
                return !isspace(c);
            }).base(),
        normalized.end());

    return normalized;
}


// =====================================================
// ADD NODE
// =====================================================

void Graph::addNode(
    const Node &node)
{
    nodes[node.id] = node;
}


// =====================================================
// HAS NODE
// =====================================================

bool Graph::hasNode(
    long long id) const
{
    return nodes.find(id)
    != nodes.end();
}


// =====================================================
// GET NODE
// =====================================================

Node Graph::getNode(
    long long id) const
{
    auto it = nodes.find(id);

    if (it != nodes.end())
    {
        return it->second;
    }

    return Node();
}


// =====================================================
// ADD EDGE
// =====================================================

void Graph::addEdge(
    long long source,
    long long destination,
    double distance)
{
    adj[source].push_back(
        {destination, distance});

    adj[destination].push_back(
        {source, distance});
}


// =====================================================
// ADD PLACE
// =====================================================

void Graph::addPlace(
    const string &name,
    long long id)
{
    string normalized =
        normalizePlaceName(name);

    if (!normalized.empty())
    {
        placeIndex[normalized] = id;
    }
}


// =====================================================
// HAS PLACE
// =====================================================

bool Graph::hasPlace(
    const string &name) const
{
    string normalized =
        normalizePlaceName(name);

    return placeIndex.find(normalized)
           != placeIndex.end();
}


// =====================================================
// GET PLACE ID
// =====================================================

long long Graph::getPlaceId(
    const string &name) const
{
    string normalized =
        normalizePlaceName(name);

    auto it =
        placeIndex.find(normalized);

    if (it == placeIndex.end())
    {
        return -1;
    }

    return it->second;
}


// =====================================================
// GET PLACE NAMES
// =====================================================

vector<string> Graph::getPlaceNames() const
{
    vector<string> names;

    names.reserve(
        placeIndex.size());

    for (const auto &entry :
         placeIndex)
    {
        names.push_back(
            entry.first);
    }

    sort(
        names.begin(),
        names.end());

    return names;
}


// =====================================================
// BUILD MAIN ROAD COMPONENT
//
// Finds the largest connected component of the
// road graph.
//
// Places will be connected only to this component.
// =====================================================

void Graph::buildMainRoadComponent()
{
    mainRoadComponent.clear();

    if (adj.empty())
    {
        return;
    }

    unordered_set<long long> visited;

    vector<long long> largestComponent;


    for (const auto &entry : adj)
    {
        long long startNode =
            entry.first;

        if (visited.find(startNode)
            != visited.end())
        {
            continue;
        }


        vector<long long> currentComponent;

        queue<long long> q;


        q.push(startNode);

        visited.insert(
            startNode);


        while (!q.empty())
        {
            long long current =
                q.front();

            q.pop();


            currentComponent.push_back(
                current);


            auto adjIt =
                adj.find(current);


            if (adjIt == adj.end())
            {
                continue;
            }


            for (const auto &edge :
                 adjIt->second)
            {
                long long neighbor =
                    edge.first;


                if (visited.find(neighbor)
                    == visited.end())
                {
                    visited.insert(
                        neighbor);

                    q.push(
                        neighbor);
                }
            }
        }


        if (currentComponent.size() >
            largestComponent.size())
        {
            largestComponent =
                std::move(
                    currentComponent);
        }
    }


    for (long long nodeId :
         largestComponent)
    {
        mainRoadComponent.insert(
            nodeId);
    }


    cout
        << "Main Road Component : "
        << mainRoadComponent.size()
        << " nodes\n";
}


// =====================================================
// CHECK MAIN ROAD COMPONENT
// =====================================================

bool Graph::isInMainRoadComponent(
    long long nodeId) const
{
    return mainRoadComponent.find(nodeId)
    != mainRoadComponent.end();
}


// =====================================================
// CALCULATE DISTANCE
// =====================================================

double Graph::calculateDistance(
    long long source,
    long long destination) const
{
    auto sourceIt =
        nodes.find(source);

    auto destinationIt =
        nodes.find(destination);


    if (sourceIt == nodes.end() ||
        destinationIt == nodes.end())
    {
        return numeric_limits<double>::infinity();
    }


    const Node &a =
        sourceIt->second;

    const Node &b =
        destinationIt->second;


    constexpr double PI =
        3.14159265358979323846;

    constexpr double EARTH_RADIUS =
        6371000.0;


    auto toRadians =
        [PI](double degrees)
    {
        return degrees *
               PI / 180.0;
    };


    double lat1 =
        toRadians(
            a.latitude);

    double lat2 =
        toRadians(
            b.latitude);


    double deltaLat =
        toRadians(
            b.latitude -
            a.latitude);


    double deltaLon =
        toRadians(
            b.longitude -
            a.longitude);


    double h =
        sin(deltaLat / 2.0) *
            sin(deltaLat / 2.0)
        +
        cos(lat1) *
            cos(lat2) *
            sin(deltaLon / 2.0) *
            sin(deltaLon / 2.0);


    double c =
        2.0 *
        atan2(
            sqrt(h),
            sqrt(1.0 - h));


    return EARTH_RADIUS * c;
}


// =====================================================
// FIND NEAREST ROAD NODE
//
// IMPORTANT:
// Search only inside the largest connected
// road component.
// =====================================================

long long Graph::findNearestRoadNode(
    long long placeNodeId) const
{
    auto placeIt =
        nodes.find(placeNodeId);


    if (placeIt == nodes.end())
    {
        return -1;
    }


    // If the place itself is already part of
    // the main road network, use it directly.
    if (isInMainRoadComponent(
            placeNodeId))
    {
        return placeNodeId;
    }


    const Node &placeNode =
        placeIt->second;


    long long nearestNodeId =
        -1;


    double minimumDistance =
        numeric_limits<double>::infinity();


    constexpr double PI =
        3.14159265358979323846;

    constexpr double EARTH_RADIUS =
        6371000.0;


    auto toRadians =
        [PI](double degrees)
    {
        return degrees *
               PI / 180.0;
    };


    // Search ONLY the main connected road network.
    for (long long roadNodeId :
         mainRoadComponent)
    {
        auto roadNodeIt =
            nodes.find(
                roadNodeId);


        if (roadNodeIt ==
            nodes.end())
        {
            continue;
        }


        const Node &roadNode =
            roadNodeIt->second;


        double lat1 =
            toRadians(
                placeNode.latitude);


        double lat2 =
            toRadians(
                roadNode.latitude);


        double deltaLat =
            toRadians(
                roadNode.latitude -
                placeNode.latitude);


        double deltaLon =
            toRadians(
                roadNode.longitude -
                placeNode.longitude);


        double h =
            sin(deltaLat / 2.0) *
                sin(deltaLat / 2.0)
            +
            cos(lat1) *
                cos(lat2) *
                sin(deltaLon / 2.0) *
                sin(deltaLon / 2.0);


        double c =
            2.0 *
            atan2(
                sqrt(h),
                sqrt(1.0 - h));


        double distance =
            EARTH_RADIUS * c;


        if (distance <
            minimumDistance)
        {
            minimumDistance =
                distance;

            nearestNodeId =
                roadNodeId;
        }
    }


    return nearestNodeId;
}


// =====================================================
// GET ADJACENCY LIST
// =====================================================

const unordered_map<
    long long,
    vector<pair<long long, double>>
    >& Graph::getAdjacencyList() const
{
    return adj;
}


// =====================================================
// GET NODES
// =====================================================

const unordered_map<long long, Node>&
Graph::getNodes() const
{
    return nodes;
}


// =====================================================
// PRINT GRAPH
// =====================================================

void Graph::printGraph() const
{
    cout
        << "\n========= GRAPH =========\n";


    int count = 0;


    for (const auto &vertex : adj)
    {
        cout
            << vertex.first
            << " -> ";


        for (const auto &edge :
             vertex.second)
        {
            cout
                << "("
                << edge.first
                << ", "
                << edge.second
                << "m) ";
        }


        cout << '\n';


        count++;


        if (count >= 20)
        {
            break;
        }
    }


    cout
        << "\nDisplayed first 20 nodes only.\n";


    cout
        << "Total Nodes : "
        << nodes.size()
        << '\n';


    cout
        << "Total Vertices with Roads : "
        << adj.size()
        << '\n';


    cout
        << "Named Places : "
        << placeIndex.size()
        << '\n';


    cout
        << "Main Road Component : "
        << mainRoadComponent.size()
        << '\n';


    cout
        << "=========================\n";
}