#include "abstractgraph.h"

AbstractGraph::AbstractGraph()
{


}

AbstractGraph::AbstractGraph(bool oriented, bool weight) : weight_(weight), oriented_(oriented)
{

}

AbstractGraph::~AbstractGraph()
{
    for(int i = 0; i < edges_.size(); i++){
        delete edges_[i].first;
    }
    for(int i = 0; i < nodes_.size(); i++){
        delete nodes_[i];
    }

}

void AbstractGraph::addEdge(Node *from, Node *to, int weight)
{

    int fromNodeNo = 0, toNodeNo = 0;
    for(int i = 0; i < nodes_.size(); i++){
        if(from == nodes_[i])
            fromNodeNo = i;
        if(to == nodes_[i])
            toNodeNo = i;

    }
    if(oriented_ == true){
        graph_[fromNodeNo].push_back(qMakePair(toNodeNo, weight));
    }
    else{
        graph_[fromNodeNo].push_back(qMakePair(toNodeNo, weight));
        graph_[toNodeNo].push_back(qMakePair(fromNodeNo, weight));
    }


    Edge* newEdge;
    if(weight_ == true)
        newEdge = new Edge(*from, *to, QString :: number(weight), oriented_);
    else
        newEdge = new Edge(*from, *to, oriented_);

    addEdgeToScene(newEdge, fromNodeNo, toNodeNo);
}

void AbstractGraph::addNode(qreal x, qreal y)
{    
    graph_.push_back(QVector<QPair<int, int>>());
    Node* newNode = new Node(x, y);

    addNodeToScene(newNode);
}

void AbstractGraph::addNodeToScene(Node *node)
{
    nodes_.push_back(node);
    node->setNodeNumber(nodes_.size());
    if(this->scene_ != nullptr)
        this->scene_->addItem(node);
}

void AbstractGraph::addEdgeToScene(Edge *edge, int from, int to)
{
    edges_.push_back(qMakePair(edge, qMakePair(from, to)));

    if(this->scene_ != nullptr)
        this->scene_->addItem(edge);
}


void AbstractGraph::selectNode(int nodeNo)
{
    nodes_[nodeNo]->select();
    nodes_[nodeNo]->update();
}

void AbstractGraph::unselectNode(int nodeNo)
{
    nodes_[nodeNo]->select();
    nodes_[nodeNo]->update();
}

int AbstractGraph::findNode(Node *node)
{
    for(int i = 0; i < nodes_.size(); i++){
        if(node == nodes_[i])
            return i;
    }
}

void AbstractGraph::clearSelection()
{
    for(int i = 0; i < edges_.size(); i++){
        edges_[i].first->unselect();
        edges_[i].first->unsetCost();
    }

    for(int i = 0; i < nodes_.size(); i++){
        nodes_[i]->unselect();
        nodes_[i]->unsetDijictraCost();
    }
}

void AbstractGraph::readFromCvs(QString txt)
{
   QStringList node = txt.split('\n', QString :: SplitBehavior ::SkipEmptyParts);

   for(int i = 0; i < node.size(); i++){
        QStringList currentNodeEdges = node[i].split(';', QString :: SplitBehavior :: SkipEmptyParts);
        for(int j = 0; j < currentNodeEdges.size(); j++){
            //graph_[i].push_back(currentNodeEdges[j].toInt());
        }
   }
}

void AbstractGraph::setAdjacencyList(const QVector<QVector<QPair<int, int> > >& graph)
{
    graph_ = graph;
}


void AbstractGraph::drawGraph(const QVector<QPair<int, QPair<qreal, qreal> > > &nodesPosition)
{  
   for(int i = 0; i < nodesPosition.size(); i++){
       Node* newNode = new Node(nodesPosition[i].second.first, nodesPosition[i].second.second);
       this->addNodeToScene(newNode);
   }

   for(int i = 0; i < graph_.size(); i++){
       for(int j = 0; j < graph_[i].size(); j++){           
           if(graph_[i][j].first > i || oriented_ == true){
               Edge* newEdge;
               if(weight_ == true){
                   newEdge = new Edge(*nodes_[i], *nodes_[graph_[i][j].first], QString :: number(graph_[i][j].second), oriented_);
               }
               else {
                   newEdge = new Edge(*nodes_[i], *nodes_[graph_[i][j].first], oriented_);
               }
               this->addEdgeToScene(newEdge, i, graph_[i][j].first);
           }

       }
   }
}

void AbstractGraph::selectEdges(QVector<edge> edges)
{
    for(int i = 0; i < edges.size(); i++){
        for(int j = 0; j < edges_.size(); j++){
            if((edges[i].from == edges_[j].second.first && edges[i].to == edges_[j].second.second) ||
                    (edges[i].from == edges_[j].second.second && edges[i].to == edges_[j].second.first)){
                edges_[j].first->select();
            }
        }
    }
}

void AbstractGraph:: prepareGraphAlgorithm()
{
    graphAlgorithm_.clearGraph();
    graphAlgorithm_.resizeGraph(graph_.size());

    for(int i = 0; i < graph_.size(); i++){
        for(int j = 0; j < graph_[i].size(); j++){           
            graphAlgorithm_.addEdge(i + 1, graph_[i][j].first + 1, graph_[i][j].second);
        }
    }

}

void AbstractGraph::showBridges()
{
    this->prepareGraphAlgorithm();

    QVector<edge> bridges = QVector<edge> :: fromStdVector(graphAlgorithm_.findBridges());
    this->selectEdges(bridges);
}

void AbstractGraph::showMaxFlow(int from, int to)
{
    this->prepareGraphAlgorithm();
    std :: pair <int, std :: vector<edge>> maxFlow = graphAlgorithm_.maxFlow(from, to);
    int flow = maxFlow.first;
    QVector<edge> edges = QVector<edge> :: fromStdVector(maxFlow.second);


    for(int i = 0; i < edges.size(); i++){

        for(int j = 0; j < edges_.size(); j++){
            if((edges[i].from == edges_[j].second.first && edges[i].to == edges_[j].second.second) || (edges[i].from == edges_[j].second.second && edges[i].to == edges_[j].second.first)){
                edges_[j].first->select();
                edges_[j].first->setCost(qAbs(edges[i].cost));
            }
        }
    }

}

void AbstractGraph::showDijicstra(int from, int to)
{
    this->prepareGraphAlgorithm();

    std :: pair <std :: vector<long long>, std :: vector<int>> answer = graphAlgorithm_.djikstra(from);        
    QVector<long long> cost = QVector<long long> :: fromStdVector(answer.first);

    for(int i = 0; i < cost.size(); i++){        
        nodes_[i]->setDijictraCost(cost[i]);
    }
    QVector<int> path = QVector<int> :: fromStdVector(answer.second);
    QVector <QPair <int, int> > edges;
    int curNode = to;
    while(path[curNode] != -1) {
        edges.push_back(qMakePair(curNode, path[curNode]));
        curNode = path[curNode];
    }

    for(int i = 0; i < edges.size(); i++){
        for(int j = 0; j < edges_.size(); j++){
            if(edges[i].first == edges_[j].second.first && edges[i].second == edges_[j].second.second ||
                    edges[i].first == edges_[j].second.second && edges[i].second == edges_[j].second.first)
                edges_[j].first->select();
        }
    }

}

void AbstractGraph::showCruskal()
{
    this->prepareGraphAlgorithm();
    std :: pair <long long, std :: vector<edge>> answer = graphAlgorithm_.kruskal();
    long long minSpanWeight = answer.first;

    QVector<edge> edges = QVector<edge> :: fromStdVector(answer.second);

    this->selectEdges(edges);
}

void AbstractGraph::showLCA(int first, int second)
{

    this->prepareGraphAlgorithm();    
    int answer = graphAlgorithm_.lca(first, second);
    nodes_[answer]->select();
}

void AbstractGraph::setScene(QGraphicsScene *scene)
{
    scene_ = scene;
}

bool AbstractGraph::getOriention()
{
    return oriented_;
}

bool AbstractGraph::getWeight()
{
    return weight_;
}
