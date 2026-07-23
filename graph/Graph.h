#ifndef GRAPH_H
#define GRAPH_H

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <iostream>

#include "Node.h"

class Graph
{
private:

    // All OSM nodes
    std::unordered_map<long long, Node> nodes;

    // Road adjacency list
    std::unordered_map<
        long long,
        std::vector<std::pair<long long, double>>
        > adj;

    // Normalized place name -> OSM node ID
    std::unordered_map<std::string, long long> placeIndex;

    // Nodes belonging to largest connected road component
    std::unordered_set<long long> mainRoadComponent;

    // Normalize place name
    std::string normalizePlaceName(
        const std::string &name) const;

public:

    // =====================================================
    // NODE FUNCTIONS
    // =====================================================

    void addNode(
        const Node &node);

    bool hasNode(
        long long id) const;

    Node getNode(
        long long id) const;


    // =====================================================
    // ROAD FUNCTIONS
    // =====================================================

    void addEdge(
        long long source,
        long long destination,
        double distance);


    // =====================================================
    // PLACE FUNCTIONS
    // =====================================================

    void addPlace(
        const std::string &name,
        long long id);

    bool hasPlace(
        const std::string &name) const;

    long long getPlaceId(
        const std::string &name) const;

    std::vector<std::string>
    getPlaceNames() const;


    // =====================================================
    // ROAD COMPONENT
    // =====================================================

    void buildMainRoadComponent();

    bool isInMainRoadComponent(
        long long nodeId) const;


    // =====================================================
    // DISTANCE
    // =====================================================

    double calculateDistance(
        long long source,
        long long destination) const;


    // =====================================================
    // NEAREST ROAD NODE
    // =====================================================

    long long findNearestRoadNode(
        long long placeNodeId) const;


    // =====================================================
    // GETTERS
    // =====================================================

    const std::unordered_map<
        long long,
        std::vector<std::pair<long long, double>>
        >& getAdjacencyList() const;

    const std::unordered_map<long long, Node>&
    getNodes() const;


    // =====================================================
    // DEBUG
    // =====================================================

    void printGraph() const;
};

#endif // GRAPH_H