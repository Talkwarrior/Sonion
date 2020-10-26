#include "MonitorDisplay.h"
#include <qgridlayout.h>
#include <QScrollbar>
#include <QDebug>

MonitorDisplay::MonitorDisplay(QWidget* parent, QSerialPortInfo* info): AbstractDisplay(parent, info)
{
	this->setWindowTitle(info->portName().append(" Monitor"));
	this->initLayout();
}

MonitorDisplay::~MonitorDisplay()
{
	//this->MainWindow
}

void MonitorDisplay::initLayout()
{
	this->setMinimumSize(300, 200);
	QWidget* main = new QWidget(this);
	QGridLayout* gl = new QGridLayout(main);
	
	this->monitorWidget = new QTextBrowser(main);
	this->inputLine = new QLineEdit(main);
	this->submitButton = new QPushButton("Enter",main);

	gl->addWidget(this->inputLine, 0, 0, 1, 4);
	gl->addWidget(this->submitButton, 0, 4, 1, 1);
	gl->addWidget(this->monitorWidget, 1, 0, 1, 5);

	main->setLayout(gl);
	this->setWidget(main);
}

void MonitorDisplay::update(const QByteArray &data)
{
	this->monitorWidget->append(data);
	QScrollBar* bar = this->monitorWidget->verticalScrollBar();
	bar->setValue(bar->maximum());
}
