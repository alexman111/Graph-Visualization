#include "choosealgorithmdialog.h"
#include "ui_choosealgorithmdialog.h"

ChooseAlgorithmDialog::ChooseAlgorithmDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChooseAlgorithmDialog)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowFlag(Qt::WindowStaysOnTopHint);
}

Algorithm ChooseAlgorithmDialog::getAlgorithm()
{
    return Algorithm(ui->algorithmBox->currentIndex());
}

ChooseAlgorithmDialog::~ChooseAlgorithmDialog()
{
    delete ui;
}
