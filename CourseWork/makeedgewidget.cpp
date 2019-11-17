#include "makeedgewidget.h"
#include "ui_makeedgewidget.h"

MakeEdgeWidget::MakeEdgeWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MakeEdgeWidget)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowFlag(Qt::WindowStaysOnTopHint);

}

MakeEdgeWidget::~MakeEdgeWidget()
{
    delete ui;
}


void MakeEdgeWidget::on_orientedButton_clicked()
{
    isOriented_ = true;
}

void MakeEdgeWidget::on_unorientedButton_clicked()
{
    isOriented_ = false;
}

int MakeEdgeWidget::getWeight()
{
    return ui->weightSlider->value();
}

void MakeEdgeWidget::on_weightSlider_valueChanged(int value)
{
    ui->weightLine->setText(QString :: number(value));
}

void MakeEdgeWidget::on_weightLine_editingFinished()
{
    bool okToInt;
    int newWeight = ui->weightLine->text().toInt(&okToInt);
    if(okToInt){
        QSignalBlocker blocker(ui->weightSlider);

        if(ui->weightSlider->maximum() < newWeight){
            ui->weightSlider->setMaximum(newWeight);
        }

        ui->weightSlider->setValue(newWeight);
    }
    else {
        QMessageBox :: information(this, "Ошибка", "Не может быть весом");
    }
}
