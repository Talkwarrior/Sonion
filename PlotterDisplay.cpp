#include "PlotterDisplay.h"
#include "Sonion.h"
#include "fit_generator.h"

#include <fstream>
#include <QtCharts/qchart.h>
#include <QtCharts/qchartview.h>
#include <qlayout.h>
#include <QtWidgets\qmessagebox.h>

PlotterDisplay::PlotterDisplay(QWidget* parent, QSerialPortInfo* info) : AbstractDisplay(parent, info)
{
	this->saveButton = new QPushButton(this);
	this->saveButton->setText("Save");
	connect(this->saveButton, &QPushButton::clicked, this, &PlotterDisplay::save);

	this->setWindowTitle(info->portName().append(" Plotter"));
	this->setMinimumSize(600, 400);
	QWidget* mainWidget = new QWidget(this);
	QVBoxLayout* layout = new QVBoxLayout(mainWidget);
	this->chartView = new QtCharts::QChartView(this);
	this->chart = new QtCharts::QChart();
	this->series = new QtCharts::QLineSeries();

	this->chart->legend()->hide();
	this->chart->addSeries(this->series);
	this->chart->createDefaultAxes();
	this->chartView->setChart(this->chart);

	this->chartView->setRenderHint(QPainter::Antialiasing);
	layout->addWidget(this->chartView);
	layout->addWidget(this->saveButton);

	mainWidget->setLayout(layout);

	this->setWidget(mainWidget);
}

PlotterDisplay::~PlotterDisplay()
{
	this->MainWindow->unsubscribe(this);
}

void PlotterDisplay::save()
{
	auto points = this->series->points();
	std::ofstream out;
	out.open("time_points.txt");
	for (const QPointF& point : points) {
		out << point.x() << "\t" << point.y() << "\n";
	}
	out.close();

	out.open("time_funcs.txt");
	fit_generator(points.mid(1, -1), out);
	out.close();
}

void PlotterDisplay::update(const QByteArray& data)
{
	static QString buffer;
	buffer.append(data);
	if (buffer.endsWith('\n')) {	// last packet.
		double c;
		buffer.chop(1);
		try
		{
			c = buffer.toDouble();
			if (c > maxY) { maxY = c; }
			if (c < minY) { minY = c; }
		}
		catch (const std::exception&)
		{
			buffer.clear();
			return;
		}
		this->series->append(clock, c);
		this->chart->axisY()->setRange(minY-1, maxY+5);
		this->chart->axisX()->setRange(0, clock);
		this->clock += 1;
		buffer.clear();
	}
	else {
		return;
	}
}
