#include "OSMParser.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "../xml/tinyxml2.h"

using namespace tinyxml2;
using namespace std;

// =====================================================
// CONSTRUCTOR
// =====================================================

OSMParser::OSMParser()
{
}

// =====================================================
// CHECK FILE EXISTS
// =====================================================

bool OSMParser::fileExists(
    const string &filename) const
{
    ifstream file(filename);

    return file.is_open();
}

// =====================================================
// LOAD MAP
// =====================================================

bool OSMParser::loadMap(
    const string &filename,
    Graph &graph)
{
    // =================================================
    // CHECK FILE
    // =================================================

    if (!fileExists(filename))
    {
        cout << "Error: map.osm not found.\n";

        return false;
    }

    cout << "Loading map: "
         << filename
         << '\n';

    // =================================================
    // LOAD XML
    // =================================================

    XMLDocument doc;

    if (doc.LoadFile(filename.c_str())
        != XML_SUCCESS)
    {
        cout << "Unable to load OSM XML file.\n";

        return false;
    }

    XMLElement *root =
        doc.FirstChildElement("osm");

    if (root == nullptr)
    {
        cout << "Invalid OSM file.\n";

        return false;
    }

    // =================================================
    // STATISTICS
    // =================================================

    int nodeCount = 0;

    int namedNodePlaces = 0;

    int namedWays = 0;

    int wayCount = 0;

    int highwayWays = 0;

    int roadConnections = 0;

    // =================================================
    // STEP 1:
    // LOAD ALL OSM NODES
    // =================================================

    for (XMLElement *element =
         root->FirstChildElement("node");
         element != nullptr;
         element =
         element->NextSiblingElement("node"))
    {
        long long id =
            element->Int64Attribute("id");

        double latitude =
            element->DoubleAttribute("lat");

        double longitude =
            element->DoubleAttribute("lon");

        string name;

        // =============================================
        // READ NODE TAGS
        // =============================================

        for (XMLElement *tag =
             element->FirstChildElement("tag");
             tag != nullptr;
             tag =
             tag->NextSiblingElement("tag"))
        {
            const char *key =
                tag->Attribute("k");

            const char *value =
                tag->Attribute("v");

            if (key == nullptr ||
                value == nullptr)
            {
                continue;
            }

            string keyString =
                key;

            // -----------------------------------------
            // Use standard OSM name
            // -----------------------------------------

            if (keyString == "name")
            {
                name = value;

                break;
            }
        }

        // =============================================
        // CREATE NODE
        // =============================================

        Node node(
            id,
            latitude,
            longitude,
            name);

        graph.addNode(node);

        // =============================================
        // ADD NAMED NODE AS SEARCHABLE PLACE
        // =============================================

        if (!name.empty())
        {
            graph.addPlace(
                name,
                id);

            namedNodePlaces++;
        }

        nodeCount++;
    }

    // =================================================
    // STEP 2:
    // PROCESS ALL WAYS
    //
    // This performs two jobs:
    //
    // 1. Add highway connections to the graph.
    // 2. Add named ways as searchable places.
    // =================================================

    for (XMLElement *way =
         root->FirstChildElement("way");
         way != nullptr;
         way =
         way->NextSiblingElement("way"))
    {
        wayCount++;

        bool highway = false;

        string wayName;

        // =============================================
        // READ WAY TAGS
        // =============================================

        for (XMLElement *tag =
             way->FirstChildElement("tag");
             tag != nullptr;
             tag =
             tag->NextSiblingElement("tag"))
        {
            const char *key =
                tag->Attribute("k");

            const char *value =
                tag->Attribute("v");

            if (key == nullptr ||
                value == nullptr)
            {
                continue;
            }

            string keyString =
                key;

            // -----------------------------------------
            // Is this way a road?
            // -----------------------------------------

            if (keyString == "highway")
            {
                highway = true;
            }

            // -----------------------------------------
            // Save way name
            // -----------------------------------------

            if (keyString == "name")
            {
                wayName = value;
            }
        }

        // =============================================
        // GET ALL NODE REFERENCES OF WAY
        // =============================================

        vector<long long> refs;

        for (XMLElement *nd =
             way->FirstChildElement("nd");
             nd != nullptr;
             nd =
             nd->NextSiblingElement("nd"))
        {
            long long ref =
                nd->Int64Attribute("ref");

            if (graph.hasNode(ref))
            {
                refs.push_back(ref);
            }
        }

        // =============================================
        // ADD NAMED WAY AS SEARCHABLE PLACE
        //
        // Example:
        // FC Road
        // JM Road
        // Karve Road
        // etc.
        //
        // We use a node near the middle of the way.
        // =============================================

        if (!wayName.empty() &&
            !refs.empty())
        {
            size_t middleIndex =
                refs.size() / 2;

            long long representativeNode =
                refs[middleIndex];

            graph.addPlace(
                wayName,
                representativeNode);

            namedWays++;
        }

        // =============================================
        // IGNORE NON-HIGHWAY WAYS FOR ROUTING
        // =============================================

        if (!highway)
        {
            continue;
        }

        highwayWays++;

        // =============================================
        // NEED AT LEAST TWO NODES TO FORM ROAD
        // =============================================

        if (refs.size() < 2)
        {
            continue;
        }

        // =============================================
        // CREATE ROAD EDGES
        // =============================================

        for (size_t i = 0;
             i + 1 < refs.size();
             ++i)
        {
            long long source =
                refs[i];

            long long destination =
                refs[i + 1];

            // -----------------------------------------
            // Safety check
            // -----------------------------------------

            if (!graph.hasNode(source) ||
                !graph.hasNode(destination))
            {
                continue;
            }

            // -----------------------------------------
            // Calculate geographical distance
            // -----------------------------------------

            double distance =
                graph.calculateDistance(
                    source,
                    destination);

            // -----------------------------------------
            // Add road connection
            // -----------------------------------------

            graph.addEdge(
                source,
                destination,
                distance);

            roadConnections++;
        }
    }

    // =================================================
    // STEP 3:
    // PROCESS RELATIONS
    //
    // Some OSM locations may be represented by
    // relations rather than simple nodes.
    //
    // If a named relation contains node members,
    // use one of those nodes as its representative.
    // =================================================

    int namedRelations = 0;

    for (XMLElement *relation =
         root->FirstChildElement("relation");
         relation != nullptr;
         relation =
         relation->NextSiblingElement("relation"))
    {
        string relationName;

        // =============================================
        // FIND RELATION NAME
        // =============================================

        for (XMLElement *tag =
             relation->FirstChildElement("tag");
             tag != nullptr;
             tag =
             tag->NextSiblingElement("tag"))
        {
            const char *key =
                tag->Attribute("k");

            const char *value =
                tag->Attribute("v");

            if (key == nullptr ||
                value == nullptr)
            {
                continue;
            }

            if (string(key) == "name")
            {
                relationName =
                    value;

                break;
            }
        }

        if (relationName.empty())
        {
            continue;
        }

        // =============================================
        // FIND A VALID NODE MEMBER
        // =============================================

        long long representativeNode = -1;

        for (XMLElement *member =
             relation->FirstChildElement("member");
             member != nullptr;
             member =
             member->NextSiblingElement("member"))
        {
            const char *type =
                member->Attribute("type");

            if (type == nullptr)
            {
                continue;
            }

            if (string(type) != "node")
            {
                continue;
            }

            long long ref =
                member->Int64Attribute("ref");

            if (graph.hasNode(ref))
            {
                representativeNode =
                    ref;

                break;
            }
        }

        // =============================================
        // ADD RELATION AS SEARCHABLE PLACE
        // =============================================

        if (representativeNode != -1)
        {
            graph.addPlace(
                relationName,
                representativeNode);

            namedRelations++;
        }
    }

    // =================================================
    // PRINT STATISTICS
    // =================================================

    cout << "\n";

    cout << "========== MAP STATISTICS ==========\n";

    cout << "Total Nodes Loaded : "
         << nodeCount
         << '\n';

    cout << "Named Node Places  : "
         << namedNodePlaces
         << '\n';

    cout << "Named Ways         : "
         << namedWays
         << '\n';

    cout << "Named Relations    : "
         << namedRelations
         << '\n';

    cout << "Total Search Places: "
         << namedNodePlaces +
                namedWays +
                namedRelations
         << '\n';

    cout << "Total Ways         : "
         << wayCount
         << '\n';

    cout << "Highway Ways       : "
         << highwayWays
         << '\n';

    cout << "Road Connections   : "
         << roadConnections
         << '\n';

    cout << "====================================\n";

    return true;
}