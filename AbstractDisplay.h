#pragma once

#include <qdockwidget.h>
#include <qserialportinfo.h>

class Sonion;

class AbstractDisplay: public QDockWidget
{

public:
	QString portname;

	AbstractDisplay(QWidget* parent = Q_NULLPTR, QSerialPortInfo* info = Q_NULLPTR);
	virtual ~AbstractDisplay();

public slots:
	virtual void update(const QByteArray &data) = 0;

protected:
	Sonion* MainWindow;
	QSerialPortInfo info;
private:
	Q_OBJECT
};