#include "Sonion.h"
#include "MonitorDisplay.h"
#include "PlotterDisplay.h"

#include <memory>
#include <qdockwidget.h>
#include <QDialog>
#include <qwindow.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qcombobox.h>
#include <qpushbutton.h>
#include <Qmessagebox.h>

Sonion::Sonion(QWidget *parent) : QMainWindow(parent)
{
	this->initLayout();
}

void Sonion::initLayout()
{
    this->setBaseSize(960, 1080);
    this->setDockNestingEnabled(true);
	this->setting = new SettingsWidget(this);
    this->setting->setAllowedAreas(Qt::AllDockWidgetAreas);
    this->setting->setFeatures(this->setting->features() & ~QDockWidget::DockWidgetClosable);
    this->addDockWidget(Qt::RightDockWidgetArea, this->setting);
}

QSerialPort* Sonion::openPort(QSerialPort* port)
{
    for each(QSerialPort * i in this->ports)
    {
        if (i->portName() == port->portName()) {
            // 이미 존재하는 포트
            return i;
        }
    }

    if (port->open(QIODevice::ReadWrite)) {
        QMessageBox::information(this, "info", tr("Connected to %1 : %2, %3, %4, %5, %6")
            .arg(port->portName()).arg(port->baudRate()).arg(port->dataBits())
            .arg(port->parity()).arg(port->stopBits()).arg(port->flowControl()));
        this->ports << port;
        connect(port, &QSerialPort::errorOccurred, this, &Sonion::portError);
        connect(port, &QSerialPort::readyRead, this, &Sonion::updateDisplay);
        return port;
    }
    else {
        QMessageBox::critical(this, tr("Error"), port->errorString());
        return NULL;
    }
}

// close port, remove from ports & subscribe
void Sonion::closePort(QString portName)
{
    auto port = ports.begin();
    for ( ; port != ports.end(); port = port + 1) 
    {
        if ((*port)->portName() == portName) { break; }
    }
    (*port)->close();
    QMessageBox::information(this, "Close Port", "Successfully closed " + portName);
    this->ports.removeAll(*port);
    this->subscribeInfo.remove((*port)->portName());
}

void Sonion::openDisplay(SettingsWidget::SerialSettings s)
{
    QSerialPort* port = new QSerialPort(this);
    port->setPortName(s.name);
    port->setBaudRate(s.baudRate);
    port->setDataBits(s.dataBits);
    port->setParity(s.parity);
    port->setStopBits(s.stopBits);
    port->setFlowControl(s.flowControl);
    
    port = this->openPort(port);
    if (port == NULL) { return; }

    AbstractDisplay* dock = nullptr;
    if (s.displayType.toStdString() == "Monitor") {
	    dock = new MonitorDisplay(this, new QSerialPortInfo(*port));
    }
    else if (s.displayType.toStdString() == "Plotter") {
        dock = new PlotterDisplay(this, new QSerialPortInfo(*port));
    }
    else {
        QMessageBox::critical(this, "Error", "Undefined Display" + s.displayType);
        exit(0);
    }
	dock->setAllowedAreas(Qt::AllDockWidgetAreas);
	this->addDockWidget(Qt::RightDockWidgetArea, dock);
    this->displays << dock;
    this->subscribeInfo.insert(port->portName(), dock);
}

void Sonion::unsubscribe(AbstractDisplay* display)
{
    QString portName = display->portname;
    this->displays.removeAll(display);
    for (AbstractDisplay* subscriber : this->subscribeInfo.values(portName))
    {
        if (display == subscriber) {
            this->subscribeInfo.remove(portName, subscriber);
            if (this->subscribeInfo.count(portName) == 0) {
                this->closePort(portName);
            }
            break;
        }
    }
}

void Sonion::writeData(QString portName, const QByteArray& data)
{
    for (QSerialPort* port : this->ports) {
        if (port->portName() == portName) {
            port->write(data);
            break;
        }
    }
}

void Sonion::portError(QSerialPort::SerialPortError error)
{
    return;
}

void Sonion::updateDisplay()
{
    for(QSerialPort* port : this->ports)
    {
        if (port->isReadable()) 
        {
            const QByteArray data = port->readAll();
            for (AbstractDisplay * display : this->subscribeInfo.values(port->portName()))
            {
                display->update(data);
            }
        }
    }
}