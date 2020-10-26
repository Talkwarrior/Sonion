#include "PlotterDisplay.h"

PlotterDisplay::PlotterDisplay(QWidget* parent, QSerialPortInfo* info) : AbstractDisplay(parent, info)
{
	this->initLayout();
}

PlotterDisplay::~PlotterDisplay()
{
}

void PlotterDisplay::initLayout()
{
	this->series = new QSpline
}
