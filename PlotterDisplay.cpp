#include "PlotterDisplay.h"
#include <QtCharts/qchart.h>
#include <QtCharts/qchartview.h>
#include <qgridlayout.h>
#include <QtWidgets\qmessagebox.h>

PlotterDisplay::PlotterDisplay(QWidget* parent, QSerialPortInfo* info) : AbstractDisplay(parent, info)
{
	this->setWindowTitle(info->portName().append(" Plotter"));
	this->initLayout();
}

PlotterDisplay::~PlotterDisplay()
{
}

void PlotterDisplay::initLayout()
{
	QWidget* mainWidget = new QWidget(this);
	QGridLayout* layout = new QGridLayout(mainWidget);
	this->chartView = new QtCharts::QChartView(this);
	this->chart = new QtCharts::QChart();
	this->series = new QtCharts::QSplineSeries();

	this->chart->legend()->hide();
	this->chart->addSeries(this->series);
	this->chart->createDefaultAxes();
	this->chartView->setChart(this->chart);

	this->chartView->setRenderHint(QPainter::Antialiasing);
	layout->addWidget(this->chartView);
	mainWidget->setLayout(layout);

	this->setWidget(mainWidget);
}

void PlotterDisplay::update(const QByteArray& data)
{
	static QByteArray buffer;
	buffer.append(data);
	if (data.endsWith('\n')) {
		double c;
		try
		{
			c = buffer.toDouble();
		}
		catch (const std::exception&)
		{
			return;
		}
		this->series->append(clock, c);
		this->clock += 1;
		buffer.clear();
	}
	else {
		return;
	}
}
