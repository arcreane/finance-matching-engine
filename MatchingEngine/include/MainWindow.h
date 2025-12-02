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
#include "Order.hpp"

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
    QComboBox *symbolCombo;
    std::vector<Order> orders; // stores all created orders
    QTableWidget *orderTable;  // pointer to the order book table
    int nextOrderId = 1;       // auto-increment order IDs for orders


private slots:
    void handleInstrumentCreated();
};

#endif // MAINWINDOW_H
