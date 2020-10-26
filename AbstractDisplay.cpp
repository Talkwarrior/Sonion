#include "AbstractDisplay.h"
#include <qserialportinfo.h>
#include "Sonion.h"
#include <qmessagebox.h>

AbstractDisplay::AbstractDisplay(QWidget* parent, QSerialPortInfo* info)
{
	if (typeid(*parent) == typeid(Sonion))
	{
		this->MainWindow = static_cast<Sonion*>(parent);
	}
	else {
		QMessageBox::critical(this, "Error", "parent is not Sonion");
		exit(0);
	}
	this->info = *info;
}

AbstractDisplay::~AbstractDisplay()
{

}
