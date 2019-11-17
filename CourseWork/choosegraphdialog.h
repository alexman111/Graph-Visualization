#ifndef CHOOSEGRAPHDIALOG_H
#define CHOOSEGRAPHDIALOG_H

#include <QDialog>
#include <QMessageBox>

#include "randomgraphgenerator.h"
#include "graphdrawing.h"

namespace Ui {
class chooseGraphDialog;
}

enum GenerationMode {SPANNING_TREE, ERDOS_RENYI, MANUAL_GENERATION};
enum DrawMode{CIRCLE, FORCE, MANUAL_DRAW};

class ChooseGraphDialog : public QDialog
{
    Q_OBJECT
private:
    GenerationMode generationMode_ = SPANNING_TREE;
    DrawMode drawMode_ = CIRCLE;

    bool oriented_ = false;
    bool weight_ = false;

    int nodeNo_ = 0;
    int edgeNo_ = 0;

public:
    explicit ChooseGraphDialog(QWidget *parent = nullptr);
    ~ChooseGraphDialog();

    QVector<QVector<QPair<int, int>>> generateGraph();
    QVector<QPair<int, QPair<qreal, qreal>>> generatePosition(QVector<QVector<QPair<int, int>>> graph);

    bool getOriented();
    bool getWeight();

    void accept() override;
private slots:
    void on_nodeNoSlider_valueChanged(int value);

    void on_edgeNoSlider_valueChanged(int value);

    void on_orientedButton_clicked();

    void on_unorientedButton_clicked();

    void on_weightButton_clicked();

    void on_unweightButton_clicked();

    void on_generateAlgorithmBox_currentIndexChanged(int index);

    void on_generateAlgorithmBox_2_currentIndexChanged(int index);

private:
    Ui::chooseGraphDialog *ui;

    GenerationMode getGenerationMode();
    DrawMode getDrawMode();
};

#endif // CHOOSEGRAPHDIALOG_H
