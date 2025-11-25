#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QValueAxis>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>

#include "CreateInstrumentWidget.h"
#include "InstrumentManager.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    InstrumentManager instrumentManager;
    QWidget *createOrderBookPanel();
    QWidget *createOrderCreationPanel();
    QChartView *createDepthChart();
    QWidget *createInstrumentFormPanel();

/*private slots:
    void handleInstrumentCreated(int id);*/
};

#endif // MAINWINDOW_H