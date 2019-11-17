#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QLayout>
#include <QMessageBox>

#include "graphwidget.h"
#include "abstractgraph.h"
#include "choosegraphdialog.h"
#include "choosetreedialog.h"
#include "choosealgorithmdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_moveButton_clicked();

    void on_drawButton_clicked();

    void on_addNodeButton_clicked();

    void on_addEdgeButton_clicked();    

    void on_addTreeButton_clicked();

    void on_addGraphButton_clicked();

    void on_chooseAlgorithm_clicked();

private:
    Ui::MainWindow *ui;
    GraphWidget* graphWidget_; 
    ChooseGraphDialog* chooseGraphDialog_ = nullptr;
    ChooseTreeDialog* chooseTreeDialog_ = nullptr;
    ChooseAlgorithmDialog* chooseAlgorithmDialog = nullptr;
};

#endif // MAINWINDOW_H
