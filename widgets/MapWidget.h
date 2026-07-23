#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QWidget>
#include <QPointF>

#include <vector>

#include "../graph/Graph.h"

class MapWidget : public QWidget
{
    Q_OBJECT

private:

    const Graph *graph;

    std::vector<long long> route;

    long long sourceNode;
    long long destinationNode;

    double minLatitude;
    double maxLatitude;
    double minLongitude;
    double maxLongitude;

    bool boundsReady;

    void calculateBounds();

    QPointF coordinateToPoint(
        double latitude,
        double longitude) const;

protected:

    void paintEvent(QPaintEvent *event) override;

public:

    explicit MapWidget(
        QWidget *parent = nullptr);

    void setGraph(const Graph *graph);

    void setRoute(
        const std::vector<long long> &route,
        long long sourceNode,
        long long destinationNode);

    void clearRoute();
};

#endif // MAPWIDGET_H