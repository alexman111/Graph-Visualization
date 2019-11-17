#ifndef INTERACTIVE_VIEW_H
#define INTERACTIVE_VIEW_H

#include <QGraphicsView>
#include <QWheelEvent>

#include "node.h"
#include "abstractgraph.h"
#include "makeedgewidget.h"
#include "choosealgorithmdialog.h"

const double DELTA_ZOOM = 0.1;
enum MouseMode {ADD_NODE, ADD_EDGE, DRAW, DRAG, MAX_FLOW_MOUSE, DIJICSTRA_MOUSE, LCA_MOUSE};


class GraphWidget : public QGraphicsView
{
public:
    GraphWidget();
    ~GraphWidget() override;

    void zoomIn();
    void zoomOut();
    void addGraph(AbstractGraph* graph);
    void setMouseMode(MouseMode mode);

    void showAlgorithm(Algorithm algorithm);
    void setAdjacencyList(QVector<QVector<QPair<int, int>>> adjacencyList);
    void drawGraph(const QVector<QPair<int, QPair<qreal, qreal>>>& positions);
    void setGraph(bool oriented, bool weight);

    void clearGraph();
protected:
    void wheelEvent(QWheelEvent*) override;
    void mousePressEvent(QMouseEvent* e) override;
    void mouseMoveEvent(QMouseEvent* e) override;
private:
    AbstractGraph* graph_ = nullptr;
    MakeEdgeWidget* makeEdgeWidget_;

    MouseMode mode_ = DRAG;
    QPoint previousPointOnView_;

    double zoomDelta_;
    double currentScale_;

    void setMaxSize();
    void zoom(double scaleFactor);
};

#endif // INTERACTIVE_VIEW_H
