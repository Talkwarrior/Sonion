#pragma once
#include "AbstractDisplay.h"
#include "qpushbutton.h"
#include <QtCharts\qlineseries.h>
#include <QtCharts/qchartview.h>

class Sonion;
class PlotterDisplay : public AbstractDisplay
{
public:
    PlotterDisplay(QWidget* parent = Q_NULLPTR, QSerialPortInfo* info = Q_NULLPTR);
    ~PlotterDisplay();

public slots:
    void update(const QByteArray& data) override;
    void save();
private:
    Q_OBJECT

    QPushButton* saveButton;
    QtCharts::QChartView* chartView;
    QtCharts::QChart* chart;
    double minY = 0;
    double maxY = 10;
    quint64 clock = 0;
    QtCharts::QLineSeries* series;
};