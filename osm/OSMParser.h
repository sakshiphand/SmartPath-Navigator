#ifndef OSMPARSER_H
#define OSMPARSER_H

#include <string>

#include "../graph/Graph.h"

class OSMParser
{
public:

    OSMParser();

    bool loadMap(
        const std::string &filename,
        Graph &graph);

private:

    bool fileExists(
        const std::string &filename) const;
};

#endif // OSMPARSER_H