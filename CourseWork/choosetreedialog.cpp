#include "choosetreedialog.h"
#include "ui_choosetreedialog.h"

ChooseTreeDialog::ChooseTreeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::choosetreedialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowFlag(Qt::WindowStaysOnTopHint);
}

QVector<QVector<QPair<int, int> > > ChooseTreeDialog::generateTree()
{
    auto graph = RandomGraphGenerator :: generateTree(nodeNo_);
    return RandomGraphGenerator :: generateWeight(graph, false, false);
}

QVector<QPair<int, QPair<qreal, qreal> > > ChooseTreeDialog::generatePosition(QVector<QVector<QPair<int, int>>> adjacencyList)
{
    return GraphDrawing :: computeTreePosition(adjacencyList);
}

ChooseTreeDialog::~ChooseTreeDialog()
{
    delete ui;
}

void ChooseTreeDialog::on_horizontalSlider_valueChanged(int value)
{
    ui->nodeEdit->setText(QString :: number(value));
    nodeNo_ = value;
}

void ChooseTreeDialog::on_nodeEdit_editingFinished()
{
    bool okToInt;
    int newWeight = ui->nodeEdit->text().toInt(&okToInt);
    if(okToInt){
        QSignalBlocker blocker(ui->horizontalSlider);

        if(ui->horizontalSlider->maximum() < newWeight){
            ui->horizontalSlider->setMaximum(newWeight);
        }

        ui->horizontalSlider->setValue(newWeight);
    }
    else {
        QMessageBox :: information(this, "Ошибка", "Не может быть весом");
    }
}
