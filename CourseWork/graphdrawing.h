#ifndef GRAPHDRAWING_H
#define GRAPHDRAWING_H

#include <QPair>
#include <QVector>
#include <QQueue>
#include <QtMath>
#include <QDebug>
#include <QRandomGenerator>

const int CIRCLE_RADIUS = 70;

const qreal K_KOEFFICIENT = 0.01;
const qreal L_KOEFFICIENT = 175;
const qreal Q_KOEFFICIENT = 10;
const qreal Dt = 0.1;
const qreal MIN_KINETIC = 50;
const qreal MAX_VELOCITY = 100;
const qreal MASS = 1000;
const qreal FRICTION = 10000;
const qreal VERTEXES_DISTANCE = 50;

const qreal TREE_LAYER_DISTANSE = 100;

struct Vector{
    Vector(qreal x, qreal y);
    Vector();

    double X;
    double Y;


    Vector operator+ (const Vector& second) const;
    Vector operator- (const Vector& second) const;
    Vector operator* (const qreal& scalar) const;
    Vector operator/ (const qreal& scalar) const;

    Vector operator-=(const Vector& second);
    Vector operator+= (const Vector& second);

    qreal computeVectorLength() const;
    Vector normalizeVector() const;
    static qreal computeDistanse(const Vector& first, const Vector& second) ;
};


class GraphDrawing
{
private:
    static qreal computeAngle(int nodesNu);
    static qreal computeHooke(Vector firstVector, Vector secondVector);
    static qreal computeLogHooke(Vector firstVector, Vector secondVector);
    static QVector<Vector> computeIteration(const QVector<Vector>& position,
                                            const QVector<Vector>& velocity,
                                            const QVector<QVector<QPair<int, int>>>& graph);
    static qreal computeKineticEnergy(const QVector<Vector>& velocity);
public:
    GraphDrawing();
    static QVector<QPair<int, int>> orderNodes(QVector<QVector<int>> graph);
    static QVector<QPair<int, QPair<qreal, qreal>>> computePositionOnCircle(QVector<QVector<QPair<int, int> > >  graph);
    static QVector<QPair<int, QPair<qreal, qreal>>> forceMethod(QVector<QVector<QPair<int, int> > >  graph);
    static QVector<QPair<int, QPair<qreal, qreal>>> computeTreePosition(QVector<QVector<QPair<int, int>>> graph);
};

#endif // GRAPHDRAWING_H
