#include "mainwindow.h"

#include <QApplication>

#include <iostream>

#include "graph/Graph.h"
#include "osm/OSMParser.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Graph graph;

    OSMParser parser;

    const std::string mapPath =
        "../../osm/map.osm";

    // ==========================================
    // LOAD OSM MAP
    // ==========================================

    if (!parser.loadMap(
            mapPath,
            graph))
    {
        std::cerr
            << "Failed to load map."
            << std::endl;

        return 1;
    }

    // ==========================================
    // BUILD MAIN CONNECTED ROAD NETWORK
    //
    // IMPORTANT:
    // This MUST be called after OSMParser has
    // finished adding all road edges.
    // ==========================================

    graph.buildMainRoadComponent();

    // ==========================================
    // DEBUG INFORMATION
    // ==========================================

    graph.printGraph();

    // ==========================================
    // START MAIN WINDOW
    // ==========================================

    MainWindow window(&graph);

    window.show();

    return app.exec();
}