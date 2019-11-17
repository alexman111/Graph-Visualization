#include "node.h"

QRectF Node::boundingRect() const
{
    return QRectF(0, 0, NODE_RADIUS * 2, NODE_RADIUS * 2);
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setPen(Qt :: black);
    painter->setRenderHint(QPainter::Antialiasing);

    if(isSelected_ == true){
        painter->setBrush(selectedColor);
    }
    else {

        painter->setBrush(standartColor);
    }

    painter->drawEllipse(0, 0, NODE_RADIUS * 2, NODE_RADIUS * 2);

    painter->setFont(QFont("Calibri", 20));

    if(hasDijicstraCost_ == true){
        painter->drawText(this->boundingRect(),Qt :: AlignCenter, QString :: number(dijicstraCost));
    }
    else {
        painter->drawText(this->boundingRect(),Qt :: AlignCenter, QString :: number(nodeNo_));
    }
}

QPainterPath Node::shape() const
{
    QPainterPath path;
    path.addEllipse(boundingRect());
    return path;
}

Node::Node(qreal x, qreal y)
{
    this->setZValue(1);
    this->setPos(QPointF(x, y));
}

void Node::setNodeNumber(int number)
{
    nodeNo_ = number;
}

void Node::unsetDijictraCost()
{
    hasDijicstraCost_ = false;
}

void Node::setDijictraCost(int cost)
{
    hasDijicstraCost_ = true;
    dijicstraCost = cost;

    this->update();
}

qreal Node::getX() const
{
    return this->x();
}

qreal Node::getY() const
{
    return this->y();
}

void Node::select()
{
    isSelected_ = true;
    this->update();
}

void Node::unselect()
{
    isSelected_ = false;
    this->update();
}
