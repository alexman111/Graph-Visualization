#pragma once

#include <QGraphicsItem>
#include <QPainter>

#include "edge.h"

const qreal NODE_RADIUS = 30;

class Node : public QGraphicsEllipseItem
{
private:
    QColor standartColor = Qt :: red;
    QColor selectedColor = Qt :: blue;

    bool isSelected_ = false;
    bool hasDijicstraCost_ = false;

    int nodeNo_;
    int dijicstraCost;

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QPainterPath shape() const override;
public:
    Node(qreal x, qreal y);

    qreal getX() const;
    qreal getY() const;

    void select();
    void unselect();

    void setNodeNumber(int number);

    void unsetDijictraCost();
    void setDijictraCost(int cost);
};


