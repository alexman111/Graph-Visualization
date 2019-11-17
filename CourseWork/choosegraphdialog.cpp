#include "choosegraphdialog.h"
#include "ui_choosegraphdialog.h"

ChooseGraphDialog::ChooseGraphDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::chooseGraphDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowFlag(Qt::WindowStaysOnTopHint);
}

ChooseGraphDialog::~ChooseGraphDialog()
{
    delete ui;
}

 QVector<QVector<QPair<int, int>>> ChooseGraphDialog::generateGraph()
{
    QVector <QVector<int>> graph;
    if(generationMode_ == ERDOS_RENYI)
        graph =  RandomGraphGenerator :: generateErdosRenyiGraph(nodeNo_, edgeNo_ / 100.0, oriented_);
    if(generationMode_ == SPANNING_TREE)
        graph = RandomGraphGenerator :: generateConnectedGraph(nodeNo_, edgeNo_, oriented_);
    return RandomGraphGenerator :: generateWeight(graph, oriented_, weight_);
 }

QVector<QPair<int, QPair<qreal, qreal> > > ChooseGraphDialog::generatePosition(QVector<QVector<QPair<int, int>>> graph)
{
    if(drawMode_ == CIRCLE)
        return GraphDrawing :: computePositionOnCircle(graph);
    if(drawMode_ == FORCE)
        return GraphDrawing :: forceMethod(graph);
}

void ChooseGraphDialog::on_nodeNoSlider_valueChanged(int value)
{
    ui->nodeNoAmountLabel->setNum(value);
    nodeNo_ = value;
}

void ChooseGraphDialog::on_edgeNoSlider_valueChanged(int value)
{
    ui->edgeNoAmountLabel->setNum(value);
    edgeNo_ = value;
}

void ChooseGraphDialog::on_orientedButton_clicked()
{
    oriented_ = true;
}

void ChooseGraphDialog::on_unorientedButton_clicked()
{
    oriented_ = false;
}

void ChooseGraphDialog::on_weightButton_clicked()
{
    weight_ = true;
}

void ChooseGraphDialog::on_unweightButton_clicked()
{
    weight_ = false;
}

bool ChooseGraphDialog::getOriented()
{
    return oriented_;
}

bool ChooseGraphDialog::getWeight()
{
    return weight_;
}

void ChooseGraphDialog::accept()
{
    if(generationMode_ == SPANNING_TREE){     
        if(nodeNo_ == 0 || nodeNo_ > edgeNo_ - 1){
            QMessageBox :: information(this, "Ошибка", "Неверное количество ребер и вершин");
            emit reject();
            return;
        }
    }
    QDialog :: accept();
}

GenerationMode ChooseGraphDialog::getGenerationMode()
{
    return generationMode_;
}

DrawMode ChooseGraphDialog::getDrawMode()
{
    return drawMode_;
}

void ChooseGraphDialog::on_generateAlgorithmBox_currentIndexChanged(int index)
{
    generationMode_ = GenerationMode(index);
}


void ChooseGraphDialog::on_generateAlgorithmBox_2_currentIndexChanged(int index)
{
    drawMode_ = DrawMode(index);
}
