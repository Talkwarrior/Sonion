#include "SettingsWidget.h"
#include "Sonion.h"
#include <Qlayout>
#include <qpushbutton.h>
#include <QComboBox>
#include <qserialport.h>
#include <qserialportinfo.h>
#include <QLabel>
#include <QFormLayout>
#include <qmessagebox.h>

static const char blankString[] = QT_TRANSLATE_NOOP("SettingsDialog", "N/A");

SettingsWidget::SettingsWidget(QWidget* parent)
{
	if (typeid(*parent) == typeid(Sonion))
	{
		this->MainWindow = static_cast<Sonion*>(parent);
	}
	else {
		QMessageBox::critical(this, "Error", "parent is not Sonion");
		exit(0);
	}
	this->initLayout();
	this->setWindowTitle(QString("Settings"));
}

void SettingsWidget::initLayout()
{
	this->mainWidget = new QWidget(this);
	QVBoxLayout* mainLayout = new QVBoxLayout(this->mainWidget);
	mainLayout->addWidget(this->createForm());

	openDisplayButton = new QPushButton("Open", this);
	mainLayout->addWidget(openDisplayButton);

	this->mainWidget->setLayout(mainLayout);
	this->setWidget(this->mainWidget);
	connect(openDisplayButton, &QPushButton::clicked, this, &SettingsWidget::createDisplay);
}

void SettingsWidget::updatePortInfoBox()
{
	this->serialPortInfoListBox->clear();
	QString description;
	QString manufacturer;
	QString serialNumber;
	const auto infos = QSerialPortInfo::availablePorts();
	for (const QSerialPortInfo &info : infos) {
		QStringList list;
		description = info.description();
		manufacturer = info.manufacturer();
		serialNumber = info.serialNumber();
		list << info.portName()
			 << (!description.isEmpty() ? description : blankString)
			 << (!manufacturer.isEmpty() ? manufacturer : blankString)
			 << (!serialNumber.isEmpty() ? serialNumber : blankString)
			 << info.systemLocation()
			 << (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : blankString)
			 << (info.productIdentifier() ? QString::number(info.productIdentifier(), 16) : blankString);

		this->serialPortInfoListBox->addItem(list.first(), list);
	}
}

QGroupBox* SettingsWidget::createForm()
{
	this->form = new QGroupBox("form", this->mainWidget);
	QFormLayout* l = new QFormLayout(form);

	// create widgets
	this->serialPortInfoListBox = new QComboBox(this->mainWidget);
	this->baudRateBox			= new QComboBox(this->mainWidget);
	this->dataBitsBox			= new QComboBox(this->mainWidget);
	this->parityBox				= new QComboBox(this->mainWidget);
	this->stopBitsBox			= new QComboBox(this->mainWidget);
	this->flowControlBox		= new QComboBox(this->mainWidget);
	this->localEchoCheckBox		= new QCheckBox(this->mainWidget);
	
	this->monitorRadio = new QRadioButton("Monitor", this->mainWidget);
	this->plotterRadio = new QRadioButton("Plotter", this->mainWidget);
	// init widgets
	this->updatePortInfoBox();
	this->baudRateBox->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
	this->baudRateBox->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
	this->baudRateBox->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
	this->baudRateBox->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);

	this->dataBitsBox->addItem(QStringLiteral("5"), QSerialPort::Data5);
	this->dataBitsBox->addItem(QStringLiteral("6"), QSerialPort::Data6);
	this->dataBitsBox->addItem(QStringLiteral("7"), QSerialPort::Data7);
	this->dataBitsBox->addItem(QStringLiteral("8"), QSerialPort::Data8);

	this->parityBox->addItem(tr("None"), QSerialPort::NoParity);
	this->parityBox->addItem(tr("Even"), QSerialPort::EvenParity);
	this->parityBox->addItem(tr("Odd"), QSerialPort::OddParity);
	this->parityBox->addItem(tr("Mark"), QSerialPort::MarkParity);
	this->parityBox->addItem(tr("Space"), QSerialPort::SpaceParity);

	this->stopBitsBox->addItem(QStringLiteral("1"), QSerialPort::OneStop);
#ifdef Q_OS_WIN
	this->stopBitsBox->addItem(tr("1.5"), QSerialPort::OneAndHalfStop);
#endif
	this->stopBitsBox->addItem(QStringLiteral("2"), QSerialPort::TwoStop);

	this->flowControlBox->addItem(tr("None"), QSerialPort::NoFlowControl);
	this->flowControlBox->addItem(tr("RTS/CTS"), QSerialPort::HardwareControl);
	this->flowControlBox->addItem(tr("XON/XOFF"), QSerialPort::SoftwareControl);

	// init form
	l->addRow(new QLabel(tr("port: "), form), this->serialPortInfoListBox);
	l->addRow(new QLabel(tr("baudrate: "), form), this->baudRateBox);
	l->addRow(new QLabel(tr("databits: "), form), this->dataBitsBox);
	l->addRow(new QLabel(tr("parity: "), form), this->parityBox);
	l->addRow(new QLabel(tr("stopBits: "), form), this->stopBitsBox);
	l->addRow(new QLabel(tr("flowControl: "), form), this->flowControlBox);
	l->addRow(new QLabel(tr("localEcho: "), form), this->localEchoCheckBox);
	l->addWidget(this->monitorRadio);
	l->addWidget(this->plotterRadio);
	form->setLayout(l);
	return form;
}

SettingsWidget::SerialSettings SettingsWidget::getSetting()
{
	SettingsWidget::SerialSettings set;
	set.name = this->serialPortInfoListBox->currentText();

	if (this->baudRateBox->currentIndex() == 4) {
		set.baudRate = this->baudRateBox->currentText().toInt();
	}
	else {
		set.baudRate = static_cast<QSerialPort::BaudRate>(
			this->baudRateBox->itemData(this->baudRateBox->currentIndex()).toInt());
	}
	set.stringBaudRate = QString::number(set.baudRate);

	set.dataBits = static_cast<QSerialPort::DataBits>(
		this->dataBitsBox->itemData(this->dataBitsBox->currentIndex()).toInt());
	set.stringDataBits = this->dataBitsBox->currentText();

	set.parity = static_cast<QSerialPort::Parity>(
		this->parityBox->itemData(this->parityBox->currentIndex()).toInt());
	set.stringParity = this->parityBox->currentText();

	set.stopBits = static_cast<QSerialPort::StopBits>(
		this->stopBitsBox->itemData(this->stopBitsBox->currentIndex()).toInt());
	set.stringStopBits = this->stopBitsBox->currentText();

	set.flowControl = static_cast<QSerialPort::FlowControl>(
		this->flowControlBox->itemData(this->flowControlBox->currentIndex()).toInt());
	set.stringFlowControl = this->flowControlBox->currentText();

	set.localEchoEnabled = this->localEchoCheckBox->isChecked();
	return set;
}

void SettingsWidget::createDisplay()
{
	SettingsWidget::SerialSettings s = this->getSetting();
	this->MainWindow->openDisplay(s);
}
