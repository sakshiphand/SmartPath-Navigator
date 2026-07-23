#ifndef NODE_H
#define NODE_H

#include <string>

class Node
{
public:
    long long id;
    double latitude;
    double longitude;
    std::string name;

    Node();

    Node(long long id,
         double latitude,
         double longitude,
         std::string name = "");
};

#endif // NODE_H