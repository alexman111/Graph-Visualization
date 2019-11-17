
#ifndef RANDOMGRAPHGENERATOR_H
#define RANDOMGRAPHGENERATOR_H

#include <QVector>
#include <QRandomGenerator>
#include <algorithm>

const int MAX_WEIGHT = 100;

class RandomGraphGenerator
{
private:
    static QVector<int> generateRandomPermutation(int n);
public:
    RandomGraphGenerator() = delete;
    QVector<QVector<int>> static generateErdosRenyiGraph (int nodeNu, qreal probability, bool oriented  );
    QVector<QVector<int>> static generateConnectedGraph (int nodeNu, int edgeNu, bool isOriented);
    QVector<QVector<QPair<int, int>>> static generateWeight(const QVector<QVector<int>>& graph, bool oriented, bool weight);
    QVector<QVector<int>> static generateTree(int nodeNu);
};

#endif // RANDOMGRAPHGENERATOR_H
