#include "MonitorDisplay.h"
#include "Sonion.h"

#include <qkeysequence.h>
#include <qgridlayout.h>
#include <QScrollbar>
#include <qshortcut.h>
#include <QtWidgets\qmessagebox.h>

MonitorDisplay::MonitorDisplay(QWidget* parent, QSerialPortInfo* info): AbstractDisplay(parent, info)
{
	this->setWindowTitle(info->portName().append(" Monitor"));
	this->setMinimumSize(300, 200);
	QWidget* main = new QWidget(this);
	QGridLayout* gl = new QGridLayout(main);

	this->monitorWidget = new QTextBrowser(main);
	this->inputLine = new QLineEdit(main);
	this->submitButton = new QPushButton("Enter", main);

	gl->addWidget(this->inputLine, 0, 0, 1, 4);
	gl->addWidget(this->submitButton, 0, 4, 1, 1);
	gl->addWidget(this->monitorWidget, 1, 0, 1, 5);

	main->setLayout(gl);
	this->setWidget(main);
	
	QShortcut* shortcutEnter = new QShortcut(QKeySequence(Qt::Key_Enter), this);
	QShortcut* shortcutReturn = new QShortcut(QKeySequence(Qt::Key_Return), this);
	connect(shortcutEnter, &QShortcut::activated, this, &MonitorDisplay::submit);
	connect(shortcutReturn, &QShortcut::activated, this, &MonitorDisplay::submit);
	connect(this->submitButton, &QPushButton::clicked, this, &MonitorDisplay::submit);
}

MonitorDisplay::~MonitorDisplay()
{
	this->MainWindow->unsubscribe(this);
}

void MonitorDisplay::submit()
{
	QByteArray b = this->inputLine->text().toLocal8Bit();
	this->MainWindow->writeData(this->portname, b);
	this->inputLine->clear();
}

void MonitorDisplay::update(const QByteArray &data)
{
	if (data.isEmpty() || data.startsWith('\n')) { return; }
	this->monitorWidget->moveCursor(QTextCursor::End);
	this->monitorWidget->insertPlainText(data);
	this->monitorWidget->moveCursor(QTextCursor::End);
	QScrollBar* bar = this->monitorWidget->verticalScrollBar();
	bar->setValue(bar->maximum());
}
