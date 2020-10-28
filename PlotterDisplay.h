#pragma once
#include "AbstractDisplay.h"
#include <QtCharts\qsplineseries.h>
#include <QtCharts/qchartview.h>

class Sonion;
class PlotterDisplay : public AbstractDisplay
{
public:
    PlotterDisplay(QWidget* parent = Q_NULLPTR, QSerialPortInfo* info = Q_NULLPTR);
    ~PlotterDisplay();

public slots:
    void update(const QByteArray& data) override;

private:
    Q_OBJECT

    QtCharts::QChartView* chartView;
    QtCharts::QChart* chart;
    quint64 clock = 0;
    QtCharts::QSplineSeries* series;

    void initLayout();
};