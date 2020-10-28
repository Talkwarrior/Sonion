#pragma once

#include <QtWidgets/QMainWindow>
#include <qgroupbox.h>
#include <qserialport.h>
#include "SettingsWidget.h"
#include "AbstractDisplay.h"

class Sonion : public QMainWindow
{
public:
    Sonion(QWidget *parent = Q_NULLPTR);

    void openDisplay(SettingsWidget::SerialSettings s);
    void unsubscribe(AbstractDisplay* display);

private slots:
    void updateDisplay();
    void portError(QSerialPort::SerialPortError error);

private:
    Q_OBJECT
    QList<QSerialPort*> ports;
    QList<AbstractDisplay*> displays;
    QMultiMap<QString, AbstractDisplay*> subscribeInfo;

    SettingsWidget* setting;

    void initLayout();
    QSerialPort* openPort(QSerialPort* port);
    void closePort(QString portName);
};
