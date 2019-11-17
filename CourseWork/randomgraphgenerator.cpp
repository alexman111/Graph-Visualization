#include "randomgraphgenerator.h"
#include <QDebug>

QVector<int> RandomGraphGenerator::generateRandomPermutation(int n)
{
    QVector<int> oneToN;
    for(int i = 0; i < n; i++){
        oneToN.push_back(i);
    }

    QVector<int> randomPermutation;
    while(!oneToN.empty()){
        int randomIndex = QRandomGenerator :: global()->generate() % oneToN.size();
        randomPermutation.push_back(oneToN[randomIndex]);        
        oneToN.erase(oneToN.begin() + randomIndex);
    }

    return randomPermutation;

}

QVector<QVector<int> > RandomGraphGenerator::generateErdosRenyiGraph(int nodeNu, qreal probability, bool oriented)
{
    QVector<QVector<int>> graph;

    for(int  i = 0; i < nodeNu; i++){
        graph.push_back(QVector<int>());
    }

    for(int i = 0; i < graph.size(); i++){
        for(int j = 0; j < graph.size(); j++){
            if(i != j){
                qreal random = QRandomGenerator :: global()->generateDouble();
                if(random < probability){
                    if(graph[i].indexOf(j) == -1 && graph[j].indexOf(i) == -1){
                        if(oriented == true){
                            int randomOrder = QRandomGenerator :: global() -> generate() % 2;
                            if(randomOrder == 0)
                                graph[i].push_back(j);
                            else
                                graph[j].push_back(i);
                        }
                    else{
                        graph[i].push_back(j);
                        graph[j].push_back(i);
                        }
                    }
                }
            }
        }
    }
    return  graph;
}

QVector<QVector<int>> RandomGraphGenerator::generateConnectedGraph(int nodeNu, int edgeNu, bool isOriented)
{
    QVector<QVector<int>> graph;
    QVector<int> randomVector = RandomGraphGenerator :: generateRandomPermutation(nodeNu);

    for(int i = 0; i < nodeNu; i++){
        graph.push_back(QVector<int>());               
    }

    int currentNode = randomVector.last();
    randomVector.pop_back();

    while(!randomVector.isEmpty()){
        int nextNode = randomVector.last();
        randomVector.pop_back();

        if(isOriented == true){
            int randomOrder = QRandomGenerator :: global()->generate() % 2;
            if(randomOrder == 0)
                 graph[currentNode].push_back(nextNode);
            else
                 graph[nextNode].push_back(currentNode);
            edgeNu--;
        }
        else {
            graph[currentNode].push_back(nextNode);
            graph[nextNode].push_back(currentNode);
            edgeNu--;
        }
        currentNode = nextNode;        
    }

    while(edgeNu > 0){
        qDebug() << edgeNu;
        qDebug() << 132;
        int nodeFrom  = QRandomGenerator :: global()->generate() % nodeNu;
        int nodeTo = QRandomGenerator :: global()->generate() % nodeNu;

        if(nodeFrom == nodeTo)
            continue;
        else {
             if(graph[nodeFrom].indexOf(nodeTo) == -1 && graph[nodeTo].indexOf(nodeFrom) == -1){
                if(isOriented == true){
                    int randomOrder = QRandomGenerator :: global() -> generate() % 2;
                    if(randomOrder == 0)
                        graph[nodeTo].push_back(nodeFrom);
                    else
                        graph[nodeFrom].push_back(nodeTo);
                    edgeNu--;

                }
            else{
                graph[nodeFrom].push_back(nodeTo);
                graph[nodeTo].push_back(nodeFrom);
                edgeNu--;
                }

             }
        }
    }
    return graph;
}

QVector<QVector<QPair<int, int> > > RandomGraphGenerator::generateWeight(const QVector<QVector<int>>& graph, bool oriented, bool weight)
{
    if(weight == true){
        QVector<QVector<QPair<int, int>>> weightGraph(graph.size());
        for(int i = 0; i < graph.size(); i++){

            for(int j = 0; j < graph[i].size(); j++){
                if(graph[i][j] > i || oriented == true){
                    qreal randomWeight = QRandomGenerator :: global()->generateDouble() * MAX_WEIGHT;
                    if(oriented == true){
                        weightGraph[i].push_back(qMakePair(graph[i][j], randomWeight));
                    }
                    else{
                        weightGraph[i].push_back(qMakePair(graph[i][j], randomWeight));
                        weightGraph[graph[i][j]].push_back(qMakePair(i, randomWeight));
                    }
                }
            }
        }
        return weightGraph;
    }
    else {
        QVector<QVector<QPair<int, int>>> unweightGraph;
        for(int i = 0; i < graph.size(); i++){
            unweightGraph.push_back(QVector<QPair<int, int>>());
            for(int j = 0; j < graph[i].size(); j++){
                unweightGraph[i].push_back(qMakePair(graph[i][j], 1));
            }
        }
        return  unweightGraph;
    }


}

QVector<QVector<int> > RandomGraphGenerator::generateTree(int nodeNu)
{
    QVector<int> pruferSequence;
    for(int i = 0; i < nodeNu - 2; i++){
        pruferSequence.push_back(QRandomGenerator :: global()->generate() % nodeNu);
    }

    QVector<int> nodesList;
    for(int i = 0; i < nodeNu; i++){
        nodesList.push_back(i);
    }

    QVector<QVector<int>> graph(nodeNu);


    while(nodesList.size() > 2){
        int smallestNodeNotInSequence;
        for(int i = 0; i < nodesList.size(); i++){
            int j = 0;
            for(; j < pruferSequence.size(); j++){
                if(nodesList[i] == pruferSequence[j]){
                    break;
                }
            }
            if(j != pruferSequence.size()){
                continue;
            }
            else {
                smallestNodeNotInSequence = nodesList[i];
                nodesList.erase(nodesList.begin() + i);
                break;
            }
        }
        graph[smallestNodeNotInSequence].push_back(pruferSequence.first());
        graph[pruferSequence.first()].push_back(smallestNodeNotInSequence);

        pruferSequence.erase(pruferSequence.begin());
    }

    graph[nodesList[0]].push_back(nodesList[1]);
    graph[nodesList[1]].push_back(nodesList[0]);

    return graph;
}
