#include "Node.h"

Node::Node()
    : id(0),
    latitude(0.0),
    longitude(0.0),
    name("")
{
}

Node::Node(long long id,
           double latitude,
           double longitude,
           std::string name)
    : id(id),
    latitude(latitude),
    longitude(longitude),
    name(name)
{
}