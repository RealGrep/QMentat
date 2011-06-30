#include "statisticsdialog.h"
#include "ui_statisticsdialog.h"

#include <QStandardItemModel>
#include "pieview.h"

StatisticsDialog::StatisticsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StatisticsDialog)
{
    ui->setupUi(this);

    setupModel();
    setupViews();
    ui->statisticsDisplay->layout()->addWidget(pieChart);
    ui->statisticsDisplay->update();
}

StatisticsDialog::~StatisticsDialog()
{
    delete model;
    delete ui;
}

void StatisticsDialog::set(int totalCorrect, int totalWrong)
{
    setupData(totalCorrect, totalWrong);
    this->resize(pieChart->size());
}

void StatisticsDialog::setupModel()
{
    model = new QStandardItemModel(8, 2, this);
    model->setHeaderData(0, Qt::Horizontal, tr("Label"));
    model->setHeaderData(1, Qt::Horizontal, tr("Quantity"));
}

void StatisticsDialog::setupViews()
{
//    QSplitter *splitter = new QSplitter;
//    QTableView *table = new QTableView;
    pieChart = new PieView;
//    splitter->addWidget(table);
//    splitter->addWidget(pieChart);
//    splitter->setStretchFactor(0, 0);
//    splitter->setStretchFactor(1, 1);

  //  table->setModel(model);
    pieChart->setModel(model);
    pieChart->doItemsLayout();

    QItemSelectionModel *selectionModel = new QItemSelectionModel(model);
//    table->setSelectionModel(selectionModel);
    pieChart->setSelectionModel(selectionModel);

//    QHeaderView *headerView = table->horizontalHeader();
//    headerView->setStretchLastSection(true);

//    table->setEnabled(false);
//    setCentralWidget(splitter);
    //setCentralWidget(pieChart);
}

void StatisticsDialog::setupData(int totalCorrect, int totalWrong)
{
    //model->clear();
    model->insertRows(0, 1, QModelIndex());
    model->setData(model->index(0, 0, QModelIndex()),
                   tr("Correct - %1").arg(totalCorrect));
    model->setData(model->index(0, 1, QModelIndex()),
                   totalCorrect);
    model->setData(model->index(0, 0, QModelIndex()),
                   QColor("green"), Qt::DecorationRole);

    model->insertRows(1, 1, QModelIndex());
    model->setData(model->index(1, 0, QModelIndex()),
                   tr("Wrong - %1").arg(totalWrong));
    model->setData(model->index(1, 1, QModelIndex()),
                   totalWrong);
    model->setData(model->index(1, 0, QModelIndex()),
                   QColor("red"), Qt::DecorationRole);
}
