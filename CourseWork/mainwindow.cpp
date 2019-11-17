#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "graphwidget.h"
#include "node.h"
#include "edge.h"
#include "abstractgraph.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    graphWidget_ = new GraphWidget();
    ui->gridLayout->addWidget(graphWidget_);
}


MainWindow::~MainWindow()
{
    delete ui;
    if(graphWidget_ != nullptr)
        delete graphWidget_;
}


void MainWindow::on_moveButton_clicked()
{
    graphWidget_->setMouseMode(DRAG);
}

void MainWindow::on_drawButton_clicked()
{
    graphWidget_->setMouseMode(DRAW);

}

void MainWindow::on_addNodeButton_clicked()
{
    graphWidget_->setMouseMode(ADD_NODE);
}

void MainWindow::on_addEdgeButton_clicked()
{
    graphWidget_->setMouseMode(ADD_EDGE);

}

void MainWindow::on_addTreeButton_clicked()
{
    if(chooseTreeDialog_ == nullptr){
        chooseTreeDialog_ = new ChooseTreeDialog();
    }

    int result = chooseTreeDialog_->exec();
    if(result == QDialog :: Accepted){
        auto adjacencyList = chooseTreeDialog_->generateTree();
        graphWidget_->setGraph(false, false);
        graphWidget_->setAdjacencyList(adjacencyList);
        graphWidget_->drawGraph(chooseTreeDialog_->generatePosition(adjacencyList));
    }
    delete chooseTreeDialog_;
    chooseTreeDialog_ = nullptr;
}

void MainWindow::on_addGraphButton_clicked()
{
    if(chooseGraphDialog_ == nullptr){
        chooseGraphDialog_ = new ChooseGraphDialog();
    }

    int result = chooseGraphDialog_->exec();
    if(result == QDialog :: Accepted){
        auto adjacencyList = chooseGraphDialog_->generateGraph();
        graphWidget_->setGraph(chooseGraphDialog_->getOriented(), chooseGraphDialog_->getWeight());
        graphWidget_->setAdjacencyList(adjacencyList);
        graphWidget_->drawGraph(chooseGraphDialog_->generatePosition(adjacencyList));
    }

    delete chooseGraphDialog_;
    chooseGraphDialog_ = nullptr;
}

void MainWindow::on_chooseAlgorithm_clicked()
{
    if(chooseAlgorithmDialog == nullptr)
        chooseAlgorithmDialog = new ChooseAlgorithmDialog();

    int result = chooseAlgorithmDialog->exec();
    if(result == QDialog :: Accepted){
        Algorithm selectedAlgorithm = chooseAlgorithmDialog->getAlgorithm();
        graphWidget_->showAlgorithm(selectedAlgorithm);
    }


    delete chooseAlgorithmDialog;
    chooseAlgorithmDialog = nullptr;
}
