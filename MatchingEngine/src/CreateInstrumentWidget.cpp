//
// Created by Elodie COULLOC'H on 19/11/2025.
//

#include "CreateInstrumentWidget.h"
#include "InstrumentManager.hpp" // Inclure le manager réel
#include "Instrument.hpp"        // Inclure la classe Instrument
#include <QFormLayout>
#include <QVBoxLayout>
#include <QMessageBox>



CreateInstrumentWidget::CreateInstrumentWidget(InstrumentManager *manager, QWidget *parent) : QWidget(parent),instrumentManager(manager){
    if (!instrumentManager) {
        QMessageBox::critical(this, "Error","The instrument manager is missing.");
        return;
    }
    setupUi();
}

void CreateInstrumentWidget::setupUi() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QFormLayout *formLayout = new QFormLayout();

    idInstrumentInput = new QSpinBox();
    idInstrumentInput->setRange(1, 99999);

    marketIdentificationCodeInput = new QLineEdit("XPAR");
    tradingCurrencyInput = new QComboBox();
    tradingCurrencyInput->addItems({"EUR", "USD", "GBP"});

    nameInput = new QLineEdit("AAPL");
    issueInput = new QSpinBox();
    issueInput->setRange(19000101, 21000101);
    issueInput->setValue(20251119);

    stateInput = new QComboBox();
    stateInput->addItem("ACTIVE", static_cast<int>(State::ACTIVE));
    stateInput->addItem("INACTIVE", static_cast<int>(State::INACTIVE));
    stateInput->addItem("SUSPENDED", static_cast<int>(State::SUSPENDED));
    stateInput->addItem("DELISTED", static_cast<int>(State::DELISTED));

    refPriceInput = new QDoubleSpinBox();
    refPriceInput->setDecimals(4);
    refPriceInput->setRange(0.01, 10000.0);

    idTradingGroupInput = new QSpinBox();
    idTradingGroupInput->setRange(0, 99999);
    idTradingGroupInput->setValue(1001);

    lotSizeInput = new QSpinBox();
    lotSizeInput->setRange(0, 99999);
    lotSizeInput->setValue(100);

    priceDecimalInput = new QSpinBox();
    priceDecimalInput->setValue(2);

    currentOrderIdInput = new QSpinBox();
    currentTradeIdInput = new QSpinBox();
    idAPFInput = new QSpinBox();
    idAPFInput->setRange(0, 99999);
    idAPFInput->setValue(2022);

    formLayout->addRow("Instrument ID:", idInstrumentInput);
    formLayout->addRow("Market Code:", marketIdentificationCodeInput);
    formLayout->addRow("Currency:", tradingCurrencyInput);
    formLayout->addRow("Name:", nameInput);
    formLayout->addRow("Issue Date:", issueInput);
    formLayout->addRow("State:", stateInput);
    formLayout->addRow("Ref Price:", refPriceInput);
    formLayout->addRow("Trading Group ID:", idTradingGroupInput);
    formLayout->addRow("Lot Size:", lotSizeInput);
    formLayout->addRow("Price Decimals:", priceDecimalInput);
    formLayout->addRow("Current Order ID:", currentOrderIdInput);
    formLayout->addRow("Current Trade ID:", currentTradeIdInput);
    formLayout->addRow("ID APF:", idAPFInput);

    QPushButton *submitButton = new QPushButton("Create");
    connect(submitButton, &QPushButton::clicked, this, &CreateInstrumentWidget::submitInstrument);

    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(submitButton);
}

void CreateInstrumentWidget::submitInstrument() {
    if (!instrumentManager) return;

    // Get the data from the form
    int idinstrument = idInstrumentInput->value();
    std::string mic = marketIdentificationCodeInput->text().toStdString();
    std::string currency = tradingCurrencyInput->currentText().toStdString();
    std::string name = nameInput->text().toStdString();
    int issue = issueInput->value();
    State state = static_cast<State>(stateInput->currentData().toInt());    double refprice = refPriceInput->value();
    int idtradinggroup = idTradingGroupInput->value();
    int lotsize = lotSizeInput->value();
    int pricedecimal = priceDecimalInput->value();
    int currentorderid = currentOrderIdInput->value();
    int currenttradeid = currentTradeIdInput->value();
    int idapf = idAPFInput->value();

    // Create the object
    Instrument newInstrument(idinstrument, mic, currency, name, issue, state, refprice,
        idtradinggroup, lotsize, pricedecimal,
        currentorderid, currenttradeid, idapf);

    //call addInstrument
    if (instrumentManager->addInstrument(newInstrument)) {
        QMessageBox::information(this, "Success", QString("Instrument '%1' created (ID: %2).").arg(name.c_str()).arg(idinstrument));
        emit instrumentCreated(idinstrument);
    }else {

        QMessageBox::warning(this, "Error Duplicate",
            QString("Failure : The instrument %1 is already in the manager.").arg(idinstrument));
    }
}