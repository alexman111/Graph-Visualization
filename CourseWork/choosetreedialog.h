#ifndef CHOOSETREEDIALOG_H
#define CHOOSETREEDIALOG_H

#include <QDialog>

#include "randomgraphgenerator.h"
#include "graphdrawing.h"

#include <QMessageBox>

namespace Ui {
class choosetreedialog;
}

class ChooseTreeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChooseTreeDialog(QWidget *parent = nullptr);
    ~ChooseTreeDialog();
    QVector<QVector<QPair<int, int>>> generateTree();
    QVector<QPair<int, QPair<qreal, qreal>>> generatePosition(QVector<QVector<QPair<int, int>>> adjacencyList);

private slots:
    void on_horizontalSlider_valueChanged(int value);

    void on_nodeEdit_editingFinished();

private:
    Ui::choosetreedialog *ui;
    int nodeNo_;
};

#endif // CHOOSETREEDIALOG_H
