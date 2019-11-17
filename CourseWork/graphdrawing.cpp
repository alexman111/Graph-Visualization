#include "graphdrawing.h"

GraphDrawing::GraphDrawing()
{

}

QVector<QPair<int, QPair<qreal, qreal> > > GraphDrawing::computePositionOnCircle(QVector<QVector<QPair<int, int> > > graph)
{
    QVector<QPair<int, QPair<qreal, qreal>>> nodesPosition;
    qreal angle = computeAngle(graph.size());

    for(int i = 0; i < graph.size(); i++){
        qreal x = graph.size() * CIRCLE_RADIUS * qCos(i * angle);
        qreal y = graph.size() * CIRCLE_RADIUS * qSin(i * angle);
        nodesPosition.append(qMakePair(i, qMakePair(x, y)));
    }
    return nodesPosition;
}

QVector<QPair<int, QPair<qreal, qreal> > > GraphDrawing::forceMethod(QVector<QVector<QPair<int, int> > > graph)
{
    QVector<Vector> position;
    QVector<Vector> velocity;

    for(int i = 0; i < graph.size(); i++){               
        qreal randomVelocityX = QRandomGenerator :: global()->generateDouble() * MAX_VELOCITY;
        qreal randomVelocityY = QRandomGenerator :: global()->generateDouble() * MAX_VELOCITY;
        velocity.push_back(Vector(randomVelocityX, randomVelocityY));        
    }

    for(int i = 0; i < graph.size(); i++){
        qreal currentPositionX = QRandomGenerator :: global()->generateDouble() * L_KOEFFICIENT * graph.size();
        qreal currentPositionY = QRandomGenerator :: global()->generateDouble() * L_KOEFFICIENT * graph.size();
        for(int j = 0; j < graph[i].size(); j++){
            if( graph[i][j].first < position.size() &&
                    Vector :: computeDistanse(Vector(currentPositionX, currentPositionY), position[graph[i][j].first]) > L_KOEFFICIENT){
                currentPositionX = QRandomGenerator :: global()->generateDouble() * L_KOEFFICIENT * graph.size();
                currentPositionY = QRandomGenerator :: global()->generateDouble() * L_KOEFFICIENT * graph.size();
            }

        }
        position.push_back(Vector(currentPositionX, currentPositionY));
    }

    do{
        QVector<Vector> totalForce = computeIteration(position, velocity, graph);

        QVector<Vector> newPosition = position;
        QVector<Vector> newVelocity = velocity;

        for(int i = 0; i < graph.size(); i++){
            newVelocity[i] += totalForce[i] * Dt;
            newPosition[i] +=  velocity[i] * Dt;
        }
        velocity = newVelocity;
        position =  newPosition;

    }while(computeKineticEnergy(velocity) > MIN_KINETIC);

    QVector<QPair<int, QPair<qreal, qreal>>> resultPosition;
    for(int i = 0; i < graph.size(); i++){
        resultPosition.push_back(qMakePair(i,qMakePair(position[i].X, position[i].Y)));
    }
    return resultPosition;
}

QVector<QPair<int, QPair<qreal, qreal>>> GraphDrawing::computeTreePosition(QVector<QVector<QPair<int, int> > > graph)
{
    QVector<QPair<int, QPair<qreal, qreal>>> positions;
    QVector <int> parent(graph.size());
    QVector <QVector <int> > edgesOnLevel(graph.size() + 1);

    int root = 0, maxLevel = 0;

    std::function<void(int, int, int)> computeLevels = [&](int v, int p, int lvl){
        parent[v] = p;
        edgesOnLevel[lvl].push_back(v);
        maxLevel = std::max(maxLevel, lvl);
        for (auto to : graph[v]) if (to.first != p) {
            computeLevels(to.first, v, lvl + 1);
        }
    };
    computeLevels(root, -1, 0);

    positions.push_back(qMakePair(root, qMakePair(0, 0)));
    qreal curY = 0, minPreviousX = 0, maxPreviousX = 0;
    for (int level = 1; level <= maxLevel; ++level) {
        curY += TREE_LAYER_DISTANSE;
        int count = edgesOnLevel[level].size();
        qreal k = (qreal(count - 1) * VERTEXES_DISTANCE - maxPreviousX
                         +minPreviousX) / 2;

        minPreviousX = minPreviousX - k;
        maxPreviousX = maxPreviousX + k;
        qreal curX = minPreviousX;

        for (auto v : edgesOnLevel[level]) {
            positions.push_back(qMakePair(v, qMakePair(curX, curY)));
            curX += TREE_LAYER_DISTANSE;
        }
    }
    std :: sort (positions.begin(), positions.end(), [](const QPair<int, QPair<qreal,qreal>>& fi,
                 const QPair<int, QPair<qreal,qreal>>& sec){
        return fi.first < sec.first;
    });

    return positions;
}

qreal GraphDrawing::computeAngle(int nodesNu)
{
    return M_PI * 2 / nodesNu;
}

qreal GraphDrawing::computeHooke(Vector firstVector, Vector secondVector)
{
    return K_KOEFFICIENT * ((firstVector - secondVector).computeVectorLength() - L_KOEFFICIENT);
}

qreal GraphDrawing::computeLogHooke(Vector firstVector, Vector secondVector)
{
    return K_KOEFFICIENT * ((qLn( (firstVector - secondVector).computeVectorLength() / L_KOEFFICIENT)));
}

QVector<Vector> GraphDrawing::computeIteration(const QVector<Vector>& position, const QVector<Vector>& velocity, const QVector<QVector<QPair<int, int> > >&  graph)
{
    QVector<Vector> totalForce(graph.size()), coulombForce(graph.size()),
            hookeForce(graph.size()), fricForce(graph.size());

    for(int i = 0; i < graph.size(); i++){
        totalForce[i] = Vector(0, 0);
        coulombForce[i] = Vector(0, 0);
        hookeForce[i] = Vector(0, 0);
        fricForce[i] = Vector(0, 0);
    }


    for(int i = 0; i < graph.size(); i++){
        for(int j = 0; j < graph.size(); j++){
            if(i != j){
                coulombForce[i] += ((position[i] - position[j]).normalizeVector()) *
                        (qPow(Q_KOEFFICIENT, 2) / (position[i] - position[j]).computeVectorLength());
            }
        }
    }

    for(int i = 0; i < graph.size(); i++){
        for(int j = 0; j <  graph[i].size(); j++){
            hookeForce[i] += ((position[i] - position[graph[i][j].first])) *
                    computeLogHooke(position[i], position[graph[i][j].first]);
        }
    }

    for(int i = 0; i < graph.size(); i++){
        fricForce[i] = velocity[i].normalizeVector() * (-1) * FRICTION;
    }

    for(int i = 0; i < graph.size(); i++){
        totalForce[i] = (coulombForce[i] + hookeForce[i] + fricForce[i]) / MASS;            
    }

    return totalForce;
}

qreal GraphDrawing::computeKineticEnergy(const QVector<Vector> &velocity)
{
    qreal kineticEnergy = 0;
    for(int i = 0; i < velocity.size(); i++){
        kineticEnergy += qPow(velocity[i].computeVectorLength(), 2);
    }
    return kineticEnergy / 2;
}

//-------------------------------Vector struct-----------------------------------------------

Vector::Vector(qreal x, qreal y)
{
    this->X = x;
    this->Y = y;
}

Vector::Vector()
{

}

qreal Vector::computeVectorLength() const
{
    return qSqrt(pow(this->X, 2) + pow(this->Y, 2));
}

Vector Vector::normalizeVector() const
{
        Vector normalizedVector(this->X / this->computeVectorLength(),
                                this->Y / this->computeVectorLength());
        return  normalizedVector;
}

qreal Vector::computeDistanse(const Vector &first, const Vector &second)
{
    return qSqrt(qPow(first.X - second.X, 2) + qPow(first.Y - second.Y, 2));
}


Vector Vector::operator+(const Vector &second) const
{
    Vector newVector(this->X + second.X, this->Y + second.Y);
    return newVector;
}

Vector Vector::operator-(const Vector &second) const
{
    Vector newVector(this->X - second.X, this->Y - second.Y);
    return newVector;
}

Vector Vector::operator-=(const Vector &second)
{
    this->X -= second.X;
    this->Y -= second.Y;
    return *this;
}

Vector Vector::operator*(const qreal& scalar) const
{
    Vector newVector(this->X * scalar, this->Y * scalar);
    return newVector;
}

Vector Vector::operator/(const qreal& scalar) const
{
    Vector newVector(this->X / scalar, this->Y / scalar);
    return newVector;
}

Vector Vector::operator+=(const Vector &second)
{
    this->X += second.X;
    this->Y += second.Y;
    return *this;
}
