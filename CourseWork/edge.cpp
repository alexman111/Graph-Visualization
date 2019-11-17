#include "edge.h"

Edge::Edge (const Node& from, const Node& to, bool ordered) : xFrom_(from.getX() + NODE_RADIUS ), yFrom_(from.getY() + NODE_RADIUS),
    xTo_(to.getX() + NODE_RADIUS), yTo_(to.getY() + NODE_RADIUS), ordered_(ordered), hasWeight_(false)
{
    computeEdgePosition();
    this->setZValue(-1);
    this->setPos(startPosition);
}

Edge::Edge(const Node &from, const Node &to, QString weight, bool ordered) : xFrom_(from.getX() + NODE_RADIUS ), yFrom_(from.getY() + NODE_RADIUS),
    xTo_(to.getX() + NODE_RADIUS), yTo_(to.getY() + NODE_RADIUS), weight_(weight), ordered_(ordered), hasWeight_(true)
{
    computeEdgePosition();

    this->setPos(startPosition);
}

void Edge::select()
{
    selected_ = true;
    this->update();
}

void Edge::unselect()
{
    selected_ = false;
    this->update();
}

void Edge::unsetCost()
{
    hasCost_ = false;
}

void Edge::setCost(int cost)
{
    cost_ = cost;
    hasCost_ = true;

}

QRectF Edge::boundingRect() const
{
    QPointF topOfStartPosition = startPosition + QPointF(WIDTH * qCos(M_PI_4), (-WIDTH * qSin(M_PI_4)));
    QPointF topOfFinishPosition = finishPosition + QPointF(WIDTH * qCos(M_PI_4), (-WIDTH * qSin(M_PI_4)));
    QPolygonF polygonOfBound(QVector<QPointF>{QPointF(0, 0), topOfStartPosition - startPosition, topOfFinishPosition - startPosition, finishPosition - startPosition});

    return polygonOfBound.boundingRect();

}
void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setRenderHint(QPainter::Antialiasing);
    if(selected_ == true){
        painter->setPen(QPen(QBrush(selectedColor_), ARROW_PEN));
        painter->setBrush(QBrush(selectedColor_));
    }
    else{
        painter->setPen(QPen(QBrush(unselectedColor_), ARROW_PEN));
        painter->setBrush(QBrush(unselectedColor_));
    }

    painter->drawLine(QPointF(0, 0), finishPosition - startPosition);

    if(hasWeight_){
        QPointF directionVector = computeDirectionVector();
        QPointF normalVector = QPointF(directionVector.y(), -directionVector.x());
        QPointF startingPoint = (finishPosition - startPosition) / 2;

        QPointF firstPoint = startingPoint - directionVector * BOX_WIDTH + normalVector *  BOX_HEIGHT;
        QPointF secondPoint = startingPoint - directionVector * BOX_WIDTH - normalVector * BOX_HEIGHT;
        QPointF thirdPoint = startingPoint + directionVector * BOX_WIDTH + normalVector * BOX_HEIGHT;
        QPointF fourthPoint = startingPoint + directionVector * BOX_WIDTH - normalVector * BOX_HEIGHT;

        QRectF box = QPolygonF(QVector<QPointF>{firstPoint, secondPoint, thirdPoint, fourthPoint}).boundingRect();

        painter->setBrush(QBrush(boxColor_));
        painter->drawRect(box);

        painter->setFont(QFont("Calibri", FONT_SIZE));
        if(hasCost_){
            painter->drawText(box, Qt :: AlignCenter, weight_ + "|" + QString :: number(cost_));
        }
        else {
            painter->drawText(box, Qt :: AlignCenter, weight_);
        }
    }

    if(ordered_){
        QPointF directionVector = computeDirectionVector();
        QPointF normalVector = QPointF(directionVector.y(), -directionVector.x());


        QPointF arrowFirstPoint = -(ARROW_WIDTH) * normalVector + (finishPosition - startPosition ) + directionVector * ARROW_HEIGHT;
        QPointF arrowSecondPoint = (ARROW_WIDTH) * normalVector + (finishPosition - startPosition ) + directionVector * ARROW_HEIGHT;
        QPointF arrowThirdPoint = finishPosition - startPosition;
        QPolygonF arrow = QPolygonF(QVector<QPointF>{arrowFirstPoint, arrowSecondPoint, arrowThirdPoint});

        painter->setBrush(QBrush(Qt::black));
        painter->drawPolygon(arrow);
    }
}

void Edge::computeEdgePosition()
{

    QPointF directionVector = computeDirectionVector();

    qreal xEdgeStart = xFrom_ + directionVector.x() * NODE_RADIUS;
    qreal yEdgeStart = yFrom_ + directionVector.y() * NODE_RADIUS;

    qreal xEdgeFinish = xTo_ + directionVector.x() * NODE_RADIUS;
    qreal yEdgeFinish = yTo_ + directionVector.y() * NODE_RADIUS;


    startPosition = QPointF(xEdgeStart, yEdgeStart);
    finishPosition = QPointF (xEdgeFinish, yEdgeFinish);
}

QPointF Edge::computeDirectionVector()
{
    qreal vectorLength = qSqrt(qPow(xFrom_ - xTo_, 2) + qPow(yFrom_ - yTo_, 2));

    QPointF directionVector( (xFrom_ - xTo_) / vectorLength,
                             (yFrom_ - yTo_) / vectorLength);

    return directionVector;
}

