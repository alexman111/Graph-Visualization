#ifndef ABSTRACTGRAPH_H
#define ABSTRACTGRAPH_H

#include <QGraphicsScene>
#include "node.h"
#include "edge.h"
#include "randomgraphgenerator.h"
#include "graphdrawing.h"
#include "graphalgorithm.h"



const int DISTANSE_BETWEEN = 100;


class AbstractGraph
{
private:
    QVector <QPair<Edge*, QPair<int, int>>> edges_;
    QVector <Node*> nodes_;
    QVector <QVector<QPair<int, int>>> graph_;
    QGraphicsScene* scene_;
    Graph graphAlgorithm_;

    const bool weight_ = false;
    const bool oriented_ = false;

    void prepareGraphAlgorithm();
    void selectEdges(QVector<edge> edges);
public:
    AbstractGraph();
    AbstractGraph(bool oriented, bool weight);

    ~AbstractGraph();

    void setScene(QGraphicsScene* scene);

    bool getOriention();
    bool getWeight();

    void addEdge(Node *from, Node *to, int weight = 0);
    void addNode(qreal x, qreal y);
    void addNodeToScene(Node* node);
    void addEdgeToScene(Edge* edge, int from, int to);

    void selectNode(int nodeNo);
    void unselectNode(int nodeNo);
    void selectEdge(Edge* edge);
    void unselectEdge(Edge* edge);

    void readFromCvs(QString txt);

    void setAdjacencyList(const QVector<QVector<QPair<int, int>>>& graph);
    void drawGraph(const QVector<QPair<int, QPair<qreal, qreal>>>& nodesPosition);

    int findNode(Node* node);

    void clearSelection();

    void showBridges();
    void showMaxFlow(int from, int to);
    void showDijicstra(int from, int to);
    void showCruskal();
    void showLCA(int first, int second);

};

#endif // ABSTRACTGRAPH_H
