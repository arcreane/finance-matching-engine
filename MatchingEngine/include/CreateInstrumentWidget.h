//
// Created by Elodie COULLOC'H on 19/11/2025.
//

#ifndef CREATEINSTRUMENTWIDGET_H
#define CREATEINSTRUMENTWIDGET_H

#include <QWidget>
#include <QSpinBox>
#include <QLineEdit>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QPushButton>

class InstrumentManager;

class CreateInstrumentWidget : public QWidget {
    Q_OBJECT
    public:
        explicit CreateInstrumentWidget(InstrumentManager *manager, QWidget *parent = nullptr);
    signals:
        //signal emoted after a successful creation
        void instrumentCreated(int idInstrument);
    private slots:
        void submitInstrument();
    private:
        InstrumentManager *instrumentManager;

        //fields
        QSpinBox *idInstrumentInput;
        QLineEdit *marketIdentificationCodeInput;
        QComboBox *tradingCurrencyInput;
        QLineEdit *nameInput;
        QSpinBox *issueInput;
        QComboBox *stateInput;
        QDoubleSpinBox *refPriceInput;
        QSpinBox *idTradingGroupInput;
        QSpinBox *lotSizeInput;
        QSpinBox *priceDecimalInput;
        QSpinBox *currentOrderIdInput;
        QSpinBox *currentTradeIdInput;
        QSpinBox *idAPFInput;

        void setupUi();
};
#endif //CREATEINSTRUMENTWIDGET_H
