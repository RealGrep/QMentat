#ifndef STATISTICSDIALOG_H
#define STATISTICSDIALOG_H

#include <QDialog>

class QStandardItemModel;
class QAbstractItemView;

namespace Ui {
    class StatisticsDialog;
}

class StatisticsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StatisticsDialog(QWidget *parent = 0);
    ~StatisticsDialog();

    void set(int totalCorrect, int totalWrong);

private:
    Ui::StatisticsDialog *ui;

    QStandardItemModel *model;
    QAbstractItemView *pieChart;

    void setupModel();
    void setupViews();
    void setupData(int totalCorrect, int totalWrong);
};

#endif // STATISTICSDIALOG_H
