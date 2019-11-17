#ifndef MAKEEDGEWIDGET_H
#define MAKEEDGEWIDGET_H

#include <QDialog>
#include <QSignalBlocker>
#include <QMessageBox>

namespace Ui {
class MakeEdgeWidget;
}

class MakeEdgeWidget : public QDialog
{
    Q_OBJECT

public:
    explicit MakeEdgeWidget(QWidget *parent = nullptr);
    ~MakeEdgeWidget();

    int getWeight();
private slots:
    void on_orientedButton_clicked();

    void on_unorientedButton_clicked();

    void on_weightSlider_valueChanged(int value);

    void on_weightLine_editingFinished();

private:
    Ui::MakeEdgeWidget *ui;
    bool isOriented_;
};

#endif // MAKEEDGEWIDGET_H
