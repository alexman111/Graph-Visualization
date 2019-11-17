#ifndef CHOOSEALGORITHMDIALOG_H
#define CHOOSEALGORITHMDIALOG_H

#include <QDialog>

enum Algorithm {BRIDGES, MAX_FLOW, DIJICSTRA, CRUSKAL, LCA};

namespace Ui {
class ChooseAlgorithmDialog;
}

class ChooseAlgorithmDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChooseAlgorithmDialog(QWidget *parent = nullptr);
    Algorithm getAlgorithm();
    ~ChooseAlgorithmDialog();

private:
    Ui::ChooseAlgorithmDialog *ui;
};

#endif // CHOOSEALGORITHMDIALOG_H
