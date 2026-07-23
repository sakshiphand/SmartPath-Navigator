#include "MapWidget.h"

#include <QPainter>
#include <QPaintEvent>
#include <QPen>
#include <QBrush>

#include <limits>
#include <algorithm>

using namespace std;


// ============================================================
// CONSTRUCTOR
// ============================================================

MapWidget::MapWidget(QWidget *parent)
    : QWidget(parent),
    graph(nullptr),
    sourceNode(-1),
    destinationNode(-1),
    minLatitude(0.0),
    maxLatitude(0.0),
    minLongitude(0.0),
    maxLongitude(0.0),
    boundsReady(false)
{
    setMinimumSize(400, 300);
}


// ============================================================
// SET GRAPH
// ============================================================

void MapWidget::setGraph(const Graph *newGraph)
{
    graph = newGraph;

    boundsReady = false;

    calculateBounds();

    update();
}


// ============================================================
// CALCULATE MAP BOUNDS
// ============================================================

void MapWidget::calculateBounds()
{
    if (graph == nullptr)
        return;

    const auto &adj = graph->getAdjacencyList();

    if (adj.empty())
        return;

    minLatitude = numeric_limits<double>::max();
    maxLatitude = numeric_limits<double>::lowest();

    minLongitude = numeric_limits<double>::max();
    maxLongitude = numeric_limits<double>::lowest();


    for (const auto &vertex : adj)
    {
        Node node = graph->getNode(vertex.first);

        minLatitude =
            min(minLatitude, node.latitude);

        maxLatitude =
            max(maxLatitude, node.latitude);

        minLongitude =
            min(minLongitude, node.longitude);

        maxLongitude =
            max(maxLongitude, node.longitude);
    }


    if (minLatitude <= maxLatitude &&
        minLongitude <= maxLongitude)
    {
        boundsReady = true;
    }
}


// ============================================================
// CONVERT LAT/LON TO SCREEN POSITION
// ============================================================

QPointF MapWidget::coordinateToPoint(
    double latitude,
    double longitude) const
{
    const double padding = 25.0;

    double availableWidth =
        width() - 2.0 * padding;

    double availableHeight =
        height() - 2.0 * padding;


    double lonRange =
        maxLongitude - minLongitude;

    double latRange =
        maxLatitude - minLatitude;


    if (lonRange <= 0.0 ||
        latRange <= 0.0)
    {
        return QPointF(
            width() / 2.0,
            height() / 2.0);
    }


    double x =
        padding +
        ((longitude - minLongitude) /
         lonRange) *
            availableWidth;


    // Latitude increases upward.
    // QWidget Y increases downward.
    double y =
        padding +
        ((maxLatitude - latitude) /
         latRange) *
            availableHeight;


    return QPointF(x, y);
}


// ============================================================
// SET ROUTE
// ============================================================

void MapWidget::setRoute(
    const vector<long long> &newRoute,
    long long newSourceNode,
    long long newDestinationNode)
{
    route = newRoute;

    sourceNode = newSourceNode;

    destinationNode =
        newDestinationNode;

    update();
}


// ============================================================
// CLEAR ROUTE
// ============================================================

void MapWidget::clearRoute()
{
    route.clear();

    sourceNode = -1;

    destinationNode = -1;

    update();
}


// ============================================================
// PAINT MAP
// ============================================================

void MapWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);


    QPainter painter(this);

    painter.setRenderHint(
        QPainter::Antialiasing,
        true);


    // Background
    painter.fillRect(
        rect(),
        QColor(248, 250, 252));


    if (graph == nullptr ||
        !boundsReady)
    {
        painter.setPen(Qt::darkGray);

        painter.drawText(
            rect(),
            Qt::AlignCenter,
            "Map not loaded");

        return;
    }


    const auto &adj =
        graph->getAdjacencyList();


    // ========================================================
    // DRAW ROAD NETWORK
    // ========================================================

    painter.setPen(
        QPen(
            QColor(165, 172, 180),
            1.0));


    for (const auto &vertex : adj)
    {
        Node source =
            graph->getNode(vertex.first);


        QPointF sourcePoint =
            coordinateToPoint(
                source.latitude,
                source.longitude);


        for (const auto &edge :
             vertex.second)
        {
            // Avoid drawing the same
            // undirected edge twice.
            if (vertex.first >= edge.first)
                continue;


            Node destination =
                graph->getNode(edge.first);


            QPointF destinationPoint =
                coordinateToPoint(
                    destination.latitude,
                    destination.longitude);


            painter.drawLine(
                sourcePoint,
                destinationPoint);
        }
    }


    // ========================================================
    // DRAW SHORTEST ROUTE
    // ========================================================

    if (route.size() >= 2)
    {
        painter.setPen(
            QPen(
                QColor(0, 105, 255),
                5.0,
                Qt::SolidLine,
                Qt::RoundCap,
                Qt::RoundJoin));


        for (size_t i = 0;
             i + 1 < route.size();
             ++i)
        {
            Node a =
                graph->getNode(route[i]);

            Node b =
                graph->getNode(route[i + 1]);


            QPointF p1 =
                coordinateToPoint(
                    a.latitude,
                    a.longitude);


            QPointF p2 =
                coordinateToPoint(
                    b.latitude,
                    b.longitude);


            painter.drawLine(
                p1,
                p2);
        }
    }


    // ========================================================
    // DRAW SOURCE MARKER
    // ========================================================

    if (sourceNode != -1)
    {
        Node source =
            graph->getNode(sourceNode);


        QPointF point =
            coordinateToPoint(
                source.latitude,
                source.longitude);


        painter.setPen(
            QPen(Qt::black, 1));


        painter.setBrush(
            QColor(0, 170, 80));


        painter.drawEllipse(
            point,
            7.0,
            7.0);
    }


    // ========================================================
    // DRAW DESTINATION MARKER
    // ========================================================

    if (destinationNode != -1)
    {
        Node destination =
            graph->getNode(destinationNode);


        QPointF point =
            coordinateToPoint(
                destination.latitude,
                destination.longitude);


        painter.setPen(
            QPen(Qt::black, 1));


        painter.setBrush(
            QColor(220, 50, 50));


        painter.drawEllipse(
            point,
            7.0,
            7.0);
    }
}