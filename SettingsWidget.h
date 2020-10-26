#pragma once

#include <qdockwidget.h>
#include <qserialport.h>
#include <qgroupbox.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qradiobutton.h>

class Sonion; // 클래스 간 상호 참조를 위한 문법

class SettingsWidget : public QDockWidget
{
public:
    SettingsWidget(QWidget* parent = Q_NULLPTR);

    // from qt examples
    typedef struct {
        QString name;
        qint32 baudRate;
        QString stringBaudRate;
        QSerialPort::DataBits dataBits;
        QString stringDataBits;
        QSerialPort::Parity parity;
        QString stringParity;
        QSerialPort::StopBits stopBits;
        QString stringStopBits;
        QSerialPort::FlowControl flowControl;
        QString stringFlowControl;
        bool localEchoEnabled;
        QString displayType;
    } SerialSettings;

private slots:
    void createDisplay();

private:
    Q_OBJECT
    Sonion* MainWindow;

    QWidget* mainWidget;
    QPushButton* openDisplayButton;
    QGroupBox* form;

    QComboBox* serialPortInfoListBox;
    QComboBox* baudRateBox;
    QComboBox* dataBitsBox;
    QComboBox* parityBox;
    QComboBox* stopBitsBox;
    QComboBox* flowControlBox;
    QCheckBox* localEchoCheckBox;
    QRadioButton* monitorRadio;
    QRadioButton* plotterRadio;

    void initLayout();
    void updatePortInfoBox();
    QGroupBox* createForm();
    SerialSettings getSetting();
};