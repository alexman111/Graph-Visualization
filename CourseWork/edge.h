#pragma once

#include <QGraphicsItem>
#include <QDebug>
#include <QtMath>

#include "node.h"

class Node;

const qreal WIDTH = 10;
const qreal ARROW_PEN = 2;
const qreal ARROW_WIDTH = 10;
const qreal ARROW_HEIGHT = 20;
const int FONT_SIZE = 12;

const int BOX_HEIGHT = 25;
const int BOX_WIDTH = 25;

class Edge : public QGraphicsItem
{
private:
    qreal xFrom_;
    qreal yFrom_;

    qreal xTo_;
    qreal yTo_;

    QPointF startPosition;
    QPointF finishPosition;

    QString weight_;

    QColor selectedColor_ = Qt :: blue;
    QColor unselectedColor_ = Qt::black;
    QColor boxColor_ = Qt :: red;

    bool selected_ = false;

    int cost_;
    bool hasCost_ = false;

    const bool ordered_;
    const bool hasWeight_;


    QPointF computeDirectionVector();


    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void computeEdgePosition();
public:
    Edge(const Node& from,  const Node& to, bool ordered);
    Edge(const Node& from, const Node& to, QString weight, bool ordered);

    void select();
    void unselect();


    void unsetCost();
    void setCost(int cost);

};

