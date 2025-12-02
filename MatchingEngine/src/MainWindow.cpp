#include "MainWindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QTableWidget>
#include <QHeaderView>
#include <QMessageBox>
#include <QDateTime>
#include <QRadioButton>
#include <QDebug>
#include <QComboBox>
#include <QFormLayout> // Needed for form layout
// Include QtChart
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView> // Needed for QChartView

// IMPLEMENT THE SLOT (This function refreshes the QComboBox content)
void MainWindow::handleInstrumentCreated()
{
    // Check if the pointer to the combo box is valid
    if (!symbolCombo) return;

    // 1. Clear the old list
    symbolCombo->clear();

    // 2. Reload the current, updated list from the central manager
    for (const auto& instr : instrumentManager.getInstruments())
    {
        symbolCombo->addItem(QString::fromStdString(instr.name));
    }

    qDebug() << "Order form instrument list refreshed via handleInstrumentCreated. Total instruments: " << instrumentManager.getInstruments().size();
}

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

// --- Order Book Panel (No change needed) ---
QWidget *MainWindow::createOrderBookPanel()
{
    QGroupBox *groupBox = new QGroupBox("Order book");
    QVBoxLayout *layout = new QVBoxLayout(groupBox);

    // Table
    orderTable = new QTableWidget(0, 8); // CHANGE: Increased column count to 8
    orderTable->setHorizontalHeaderLabels({
        "Order ID",
        "Symbol",
        "Side",    // New: Combining Buy/Sell direction
        "Rate",    // New: The Price
        "Original Qty", // Adjusted name
        "Remaining Qty", // Adjusted name
        "Priority Time", // New: Creation timestamp
        "Status"   // New: Order Status (e.g., OPEN)
    });
    orderTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Depth chart
    QChartView *chartView = createDepthChart();
    chartView->setMinimumHeight(200);

    layout->addWidget(orderTable, 2);
    layout->addWidget(chartView, 1);

    return groupBox;
}

// --- Depth Chart (No change needed) ---
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


// --- Order Creation Panel (Change needed here) ---
QWidget *MainWindow::createOrderCreationPanel()
{
    QGroupBox *groupBox = new QGroupBox("Create an Order");
    QVBoxLayout *layout = new QVBoxLayout(groupBox);

    QGroupBox *entryOrderBox = new QGroupBox("Create Entry Order");
    QFormLayout *formLayout = new QFormLayout(entryOrderBox);

    // Account
    QLineEdit *accountEdit = new QLineEdit("1000174049");
    formLayout->addRow(new QLabel("Account:"), accountEdit);

    // Symbol
    // 1. Assign the newly created QComboBox to the member pointer `symbolCombo`
    symbolCombo = new QComboBox();

    symbolCombo->setObjectName("symbolCombo");
    for (const auto& instr : instrumentManager.getInstruments())
        symbolCombo->addItem(QString::fromStdString(instr.name));
    formLayout->addRow(new QLabel("Symbol:"), symbolCombo);

    // Sell/Buy
    QHBoxLayout *typeLayout = new QHBoxLayout;
    QRadioButton *sellButton = new QRadioButton("Sell");
    QRadioButton *buyButton = new QRadioButton("Buy");
    buyButton->setChecked(true);
    typeLayout->addWidget(sellButton);
    typeLayout->addWidget(buyButton);
    formLayout->addRow(new QLabel("Sell/Buy:"), typeLayout);

    // Amount / Rate
    QLineEdit *amountEdit = new QLineEdit("100"); // <-- Change 10 to 100 (multiple of lotsize=100)
    QLineEdit *priceEdit = new QLineEdit("1.12");  // <-- Change 1.12699 to 1.12 (2 decimals, pricedecimal=2)
    formLayout->addRow(new QLabel("Amount (K):"), amountEdit);
    formLayout->addRow(new QLabel("Rate:"), priceEdit);

    // Order Type / Trailing
    QComboBox *orderTypeCombo = new QComboBox();
    orderTypeCombo->addItems({"BID","ASK"});
    formLayout->addRow(new QLabel("Order Type:"), orderTypeCombo);

    QComboBox *trailingCombo = new QComboBox();
    trailingCombo->addItems({"LIMIT","MARKET"});
    formLayout->addRow(new QLabel("Trailing:"), trailingCombo);

    // Buttons
    QHBoxLayout *controlButtonLayout = new QHBoxLayout;
    controlButtonLayout->addStretch(1);
    QPushButton *validateButton = new QPushButton("Validate");
    QPushButton *cancelButton = new QPushButton("Cancel");
    controlButtonLayout->addWidget(validateButton);
    controlButtonLayout->addWidget(cancelButton);

    layout->addWidget(entryOrderBox);
    layout->addStretch(1);
    layout->addLayout(controlButtonLayout);

    // --- Connections ---
    connect(validateButton, &QPushButton::clicked, this, [=]() {
        bool ok;
        double price = priceEdit->text().toDouble(&ok);
        if(!ok) { QMessageBox::warning(this,"Error","Invalid price"); return; }
        int qty = amountEdit->text().toInt(&ok);
        if(!ok) { QMessageBox::warning(this,"Error","Invalid quantity"); return; }

        int index = symbolCombo->currentIndex();
        auto instruments = instrumentManager.getInstruments();
        if(index < 0 || index >= instruments.size()) {
            QMessageBox::warning(this,"Error","No instrument selected");
            return;
        }

        Instrument instr = instruments[index];

        OrderType type = buyButton->isChecked() ? OrderType::BID : OrderType::ASK;
        LimitType limit = trailingCombo->currentText() == "LIMIT" ? LimitType::LIMIT : LimitType::NONE;

        // Use instr.idinstrument instead of instr.id
        Order order(nextOrderId++, instr.name, "USD", std::chrono::system_clock::now(),
                    price, qty, TimeInForce::DAY, type, limit,
                    instr.idinstrument, qty, accountEdit->text().toInt());

        if(!order.validatePrice(instr) || !order.validateQuantity(instr)) {
            QMessageBox::warning(this,"Validation Error","Order failed validation");
            return;
        }

        orders.push_back(order);

        // Update table
        int row = orderTable->rowCount();
        orderTable->insertRow(row);
        // Helper function to format time (since std::ctime isn't great for UI)
        // For simplicity here, we'll use a placeholder/basic conversion.
        // In a real app, you'd use QDateTime::fromSecsSinceEpoch or QTime::currentTime().
        QDateTime submissionTime = QDateTime::currentDateTime();

        // COLUMN 0: Order ID
        orderTable->setItem(row, 0, new QTableWidgetItem(QString::number(order.idorder)));

        // COLUMN 1: Symbol
        orderTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(instr.name)));

        // COLUMN 2: Side (Combined Buy/Sell)
        QString side = order.ordertype == OrderType::BID ? "BUY" : "SELL";
        orderTable->setItem(row, 2, new QTableWidgetItem(side));

        // COLUMN 3: Rate (Price)
        orderTable->setItem(row, 3, new QTableWidgetItem(QString::number(order.price, 'f', instr.pricedecimal)));

        // COLUMN 4: Original Qty
        orderTable->setItem(row, 4, new QTableWidgetItem(QString::number(order.originalqty)));

        // COLUMN 5: Remaining Qty (Starts equal to Original Qty)
        orderTable->setItem(row, 5, new QTableWidgetItem(QString::number(order.quantity)));

        // COLUMN 6: Priority Time (Using current time as a simple timestamp)
        orderTable->setItem(row, 6, new QTableWidgetItem(submissionTime.toString("hh:mm:ss")));

        // COLUMN 7: Status
        orderTable->setItem(row, 7, new QTableWidgetItem("OPEN"));
        QMessageBox::information(this,"Success","Order created successfully!");
    });

    connect(cancelButton, &QPushButton::clicked, this, [=]() {
        accountEdit->clear();
        amountEdit->clear();
        priceEdit->clear();
        buyButton->setChecked(true);
        symbolCombo->setCurrentIndex(0);
    });

    return groupBox;
}


QWidget *MainWindow::createInstrumentFormPanel() {
    //create widget by passing instrument manager
    CreateInstrumentWidget *instrumentForm =
        new CreateInstrumentWidget(&instrumentManager, this);

    // 3. Connect the signal from the Instrument widget to the MainWindow slot
    // Assuming CreateInstrumentWidget emits: void instrumentCreated(int idinstrument);
    // and MainWindow slot is: void handleInstrumentCreated();
    connect(instrumentForm, &CreateInstrumentWidget::instrumentCreated,
            this, &MainWindow::handleInstrumentCreated);

    QGroupBox *groupBox = new QGroupBox("Create new Instrument");
    QVBoxLayout *layout = new QVBoxLayout(groupBox);
    layout->addWidget(instrumentForm);

    return groupBox;
}
