#include "MainWindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QTableWidget>
#include <QHeaderView>
#include <QRadioButton>
#include <QDebug>
#include <QComboBox>
// Include QtChart
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("Matching Engine - Interface");

    // --- 1. Création du layout principal (pour les deux panneaux) ---
    QWidget *orderBookPanel = createOrderBookPanel();
    QWidget *orderCreationPanel = createOrderCreationPanel();
    QWidget *instrumentFormPanel = createInstrumentFormPanel();

    QVBoxLayout *rightPanelLayout = new QVBoxLayout;
    rightPanelLayout->addWidget(orderCreationPanel, 1); // Order Creation (Haut)
    rightPanelLayout->addWidget(instrumentFormPanel, 1); // Instrument Form (Bas)

    QWidget *rightPanelContainer = new QWidget;
    rightPanelContainer->setLayout(rightPanelLayout);

    QHBoxLayout *mainHorizontalLayout = new QHBoxLayout();
    mainHorizontalLayout->addWidget(orderBookPanel,60);
    mainHorizontalLayout->addWidget(rightPanelContainer,40);
    // --- 2. Widget Conteneur ---
    QWidget *centralWidget = new QWidget;
    centralWidget->setLayout(mainHorizontalLayout);

    // Définition de la barre de titre et du menu (simulée avec des QWidget simples)
    QWidget *headerWidget = new QWidget;
    QHBoxLayout *headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->addWidget(new QLabel("<h2>User's Interface</h2>"));
    headerLayout->addStretch(1);
    headerLayout->addWidget(new QPushButton("My account"));

    // Assemblage final dans un layout vertical
    QVBoxLayout *rootLayout = new QVBoxLayout;
    rootLayout->addWidget(headerWidget);
    rootLayout->addWidget(centralWidget);

    QWidget *rootContainer = new QWidget;
    rootContainer->setLayout(rootLayout);

    setCentralWidget(rootContainer);
}

MainWindow::~MainWindow()
{
}

// --- Fonction de construction du panneau Carnet d'Ordres et Graphique TO DO ---
QWidget *MainWindow::createOrderBookPanel()
{
    QGroupBox *groupBox = new QGroupBox("Order book");
    QVBoxLayout *layout = new QVBoxLayout(groupBox);

    // a. Table des ordres (QTableWidget)
    QTableWidget *orderTable = new QTableWidget(10, 6);
    orderTable->setHorizontalHeaderLabels({"Order", "Volume", "Buy", "Sell", "Volume", "Order"});
    orderTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // (Ici, vous ajouterez le code pour remplir le tableau avec les données réelles)

    // b. Graphique de Profondeur
    QChartView *chartView = createDepthChart();
    chartView->setMinimumHeight(200);

    layout->addWidget(orderTable, 2); // 2/3 de l'espace vertical
    layout->addWidget(chartView, 1);  // 1/3 de l'espace vertical

    return groupBox;
}

// --- Fonction de construction du graphique de profondeur (Qt Charts) TO DO ---
QChartView *MainWindow::createDepthChart()
{
    QChart *chart = new QChart();
    chart->setTitle("Profondeur du Marché");
    chart->legend()->setVisible(false);
    chart->setTheme(QChart::ChartThemeLight);

    // Séries d'Ordres d'achat (Bid - Vert) et de vente (Ask - Rouge)
    QLineSeries *bidSeries = new QLineSeries(chart);
    QLineSeries *askSeries = new QLineSeries(chart);

    // DONNÉES D'EXEMPLE : (Prix, Volume cumulé)
    *bidSeries << QPointF(2.824, 60000) << QPointF(2.836, 40000) << QPointF(2.846, 10000) << QPointF(2.850, 0);
    *askSeries << QPointF(2.850, 0) << QPointF(2.856, 15000) << QPointF(2.868, 30000) << QPointF(2.898, 60000);

    // Création des aires
    QAreaSeries *bidArea = new QAreaSeries(bidSeries);
    bidArea->setColor(QColor("#a8e6a8")); // Vert clair
    bidArea->setBorderColor(QColor("#4CAF50"));

    QAreaSeries *askArea = new QAreaSeries(askSeries);
    askArea->setColor(QColor("#f4a7a7")); // Rouge clair
    askArea->setBorderColor(QColor("#FF5252"));

    chart->addSeries(bidArea);
    chart->addSeries(askArea);

    // Configuration des axes
    QValueAxis *axisX = new QValueAxis();
    axisX->setTitleText("Prix");
    axisX->setLabelFormat("%.4f");
    chart->addAxis(axisX, Qt::AlignBottom);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Volume");
    chart->addAxis(axisY, Qt::AlignLeft);

    // Attacher les séries aux axes
    bidArea->attachAxis(axisX);
    bidArea->attachAxis(axisY);
    askArea->attachAxis(axisX);
    askArea->attachAxis(axisY);

    axisX->setRange(2.820, 2.900);
    axisY->setRange(0, 65000);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    return chartView;
}


// --- Fonction de construction du panneau Création d'Ordre TO DO ---
QWidget *MainWindow::createOrderCreationPanel()
{
    QGroupBox *groupBox = new QGroupBox("Create an Order");
    QVBoxLayout *layout = new QVBoxLayout(groupBox);

    // Formulaire d'Ordre d'Entrée
    QGroupBox *entryOrderBox = new QGroupBox("Create Entry Order");
    QFormLayout *formLayout = new QFormLayout(entryOrderBox);

    // Exemples de champs
    formLayout->addRow(new QLabel("Account:"), new QLineEdit("1000174049"));
    formLayout->addRow(new QLabel("Symbol:"), new QComboBox()); // QComboBox pour les symboles

    // Champ Sell/Buy (Radio buttons)
    QHBoxLayout *typeLayout = new QHBoxLayout;
    typeLayout->addWidget(new QRadioButton("Sell"));
    QRadioButton *buyButton = new QRadioButton("Buy");
    buyButton->setChecked(true); // Par défaut sur Buy
    typeLayout->addWidget(buyButton);
    formLayout->addRow(new QLabel("Sell/Buy:"), typeLayout);

    formLayout->addRow(new QLabel("Amount (K):"), new QLineEdit("10"));
    formLayout->addRow(new QLabel("Rate:"), new QLineEdit("1.12699"));
    formLayout->addRow(new QLabel("Order Type:"), new QComboBox());
    formLayout->addRow(new QLabel("Trailing:"), new QComboBox());

    // Boutons de contrôle (OK/Cancel/Advanced)
    QHBoxLayout *controlButtonLayout = new QHBoxLayout;
    controlButtonLayout->addStretch(1);
    controlButtonLayout->addWidget(new QPushButton("Validate"));
    controlButtonLayout->addWidget(new QPushButton("Cancel"));

    layout->addWidget(entryOrderBox);
    layout->addStretch(1); // Espace vide pour pousser les boutons vers le bas
    layout->addLayout(controlButtonLayout);

    return groupBox;
}

QWidget *MainWindow::createInstrumentFormPanel() {
    //create widget by passing instrument manager
    CreateInstrumentWidget *instrumentForm =
        new CreateInstrumentWidget(&instrumentManager, this);
    QGroupBox *groupBox = new QGroupBox("Create new Instrument");
    QVBoxLayout *layout = new QVBoxLayout(groupBox);
    layout->addWidget(instrumentForm);

    return groupBox;
}

