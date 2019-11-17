#include "graphwidget.h"


GraphWidget :: GraphWidget()
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setMaxSize();
    centerOn(0, 0);

    zoomDelta_ = DELTA_ZOOM;
    currentScale_ = 1.0;

    makeEdgeWidget_ = new MakeEdgeWidget();
    this->setScene(new QGraphicsScene());
    this->setBackgroundBrush(QBrush(Qt :: gray, Qt :: SolidPattern));
}

GraphWidget::~GraphWidget()
{
    if(makeEdgeWidget_ != nullptr)
        delete makeEdgeWidget_;
    if(graph_ != nullptr)
        delete graph_;
}


void GraphWidget::wheelEvent(QWheelEvent *event)
{
    QPoint scrollAmount = event->angleDelta();
    scrollAmount.y() > 0 ? zoomIn() : zoomOut();
}

void GraphWidget::mousePressEvent(QMouseEvent* e)
{
    QPoint currentPointOnView(e->x(), e->y());
    QPointF currentPointOnScene = this->mapToScene(currentPointOnView);

    if(mode_ == ADD_NODE){
        setDragMode( DragMode::NoDrag);        
        graph_->addNode(currentPointOnScene.x(), currentPointOnScene.y());
    }

    if(mode_ == ADD_EDGE){
        setDragMode( DragMode::NoDrag);
        Node *first = dynamic_cast<Node*>(this->itemAt(previousPointOnView_));
        Node *second = dynamic_cast<Node*>(this->itemAt(currentPointOnView));
        if(first != nullptr && second != nullptr){
            if(graph_->getWeight()){
                makeEdgeWidget_->show();
                int result = makeEdgeWidget_->exec();
                if(result == QDialog :: Accepted){
                    graph_->addEdge(first, second, makeEdgeWidget_->getWeight());
                }
            }
            else
                graph_->addEdge(first, second, 1);
            previousPointOnView_ = QPoint(INT_MIN / 2, INT_MIN/2);
        }
        else
            previousPointOnView_ = currentPointOnView;
    }

    if(mode_ == DRAW){
        setDragMode( DragMode::NoDrag);
        this->scene()->addEllipse(currentPointOnScene.x() - 5, currentPointOnScene.y() - 5,
                                  10, 10, QPen(Qt :: NoPen), QBrush(Qt :: red));
        previousPointOnView_ = currentPointOnView;
    }

    if(mode_ == DRAG){
        setDragMode( DragMode::ScrollHandDrag);
        QGraphicsView :: mousePressEvent(e);
    }

    if(mode_ == MAX_FLOW_MOUSE || mode_ == DIJICSTRA_MOUSE || mode_ == LCA_MOUSE){
        setDragMode( DragMode::NoDrag);

        Node *first = dynamic_cast<Node*>(this->itemAt(previousPointOnView_));
        if(first != nullptr)
            first->select();

        Node *second = dynamic_cast<Node*>(this->itemAt(currentPointOnView));
        if(second != nullptr)
            second->select();

        if(first != nullptr && second != nullptr){
            int firstNo = graph_->findNode(first);
            int secondNo = graph_->findNode(second);
            if(mode_ == MAX_FLOW_MOUSE)
                graph_->showMaxFlow(firstNo, secondNo);
            if(mode_ == DIJICSTRA_MOUSE)
                graph_->showDijicstra(firstNo, secondNo);
            if(mode_ == LCA_MOUSE)
                graph_->showLCA(firstNo, secondNo);
            previousPointOnView_ = QPoint(INT_MIN / 2, INT_MIN/2);
        }
            else
            previousPointOnView_ = currentPointOnView;
    }
}


void GraphWidget::mouseMoveEvent(QMouseEvent *e)
{
    if(mode_ == DRAW){

        QPointF previousPointOnScene = this->mapToScene(previousPointOnView_);
        QPointF currentPointOnScene = this->mapToScene(e->pos());

        this->scene()->addLine(previousPointOnScene.x(),
                previousPointOnScene.y(),
                currentPointOnScene.x(),
                currentPointOnScene.y(),
                QPen(Qt::red,10,Qt::SolidLine,Qt::RoundCap));
        previousPointOnView_ = e->pos();
    }
    if(mode_ == DRAG){
        setDragMode( DragMode::ScrollHandDrag);
        QGraphicsView :: mouseMoveEvent(e);
    }

}

void GraphWidget::setMaxSize()
{
    setSceneRect(INT_MIN/2, INT_MIN/2, INT_MAX, INT_MAX);
}

void GraphWidget::zoom(double scaleFactor)
{
    scale(scaleFactor, scaleFactor);
    currentScale_ *= scaleFactor;
}

void GraphWidget::zoomIn()
{
    zoom(1 + zoomDelta_);
}

void GraphWidget::zoomOut()
{
    zoom (1 - zoomDelta_);
}

void GraphWidget::addGraph(AbstractGraph *graph)
{

    graph_ = graph;
    graph_->setScene(this->scene());
}

void GraphWidget::setMouseMode(MouseMode mode)
{
    if(graph_ == nullptr){
        QMessageBox :: information(this, "Ошибка", "Нет графа");
    }
    mode_ = mode;
    if(mode == MAX_FLOW_MOUSE || mode == ADD_EDGE || mode == DIJICSTRA_MOUSE){
        previousPointOnView_ = QPoint(INT_MIN / 2, INT_MIN/2);
    }
}

void GraphWidget::showAlgorithm(Algorithm algorithm)
{
    this->clearGraph();
    if(graph_ == nullptr)
        QMessageBox :: information(this, "Ошибка", "Нет графа");
    switch (algorithm) {
    case BRIDGES:
        if(!graph_->getOriention())
            graph_->showBridges();
        else
            QMessageBox :: information(this, "Ошибка", "Неправильный формат графа");
        break;
    case CRUSKAL:
        if(!graph_->getOriention())
            graph_->showCruskal();
        else
            QMessageBox :: information(this, "Ошибка", "Неправильный формат графа");
        break;
    case MAX_FLOW:
        if(graph_->getWeight() && graph_->getOriention())
            this->setMouseMode(MAX_FLOW_MOUSE);
        else
            QMessageBox :: information(this, "Ошибка", "Неправильный формат графа");
        break;
    case DIJICSTRA:
        if(!graph_->getOriention())
            this->setMouseMode(DIJICSTRA_MOUSE);
        else
            QMessageBox :: information(this, "Ошибка", "Неправильный формат графа");
        break;
    case LCA:
        if(!graph_->getOriention())
            this->setMouseMode(LCA_MOUSE);
        else
            QMessageBox :: information(this, "Ошибка", "Неправильный формат графа");
        break;
    }
}

void GraphWidget::setAdjacencyList(QVector<QVector<QPair<int, int>>> adjacencyList)
{
    graph_->setAdjacencyList(adjacencyList);
}

void GraphWidget::drawGraph(const QVector<QPair<int, QPair<qreal, qreal>>>& positions)
{
    graph_->drawGraph(positions);
}

void GraphWidget::setGraph(bool oriented, bool weight)
{
    if(graph_ != nullptr)
        delete graph_;
    graph_ = new AbstractGraph(oriented, weight);
    graph_->setScene(this->scene());
}

void GraphWidget::clearGraph()
{
    graph_->clearSelection();
}
