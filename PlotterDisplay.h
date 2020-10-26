#pragma once
#include "AbstractDisplay.h"
#include <QtCharts/qchartglobal.h>

class PlotterDisplay : public AbstractDisplay
{
public:
    PlotterDisplay(QWidget* parent = Q_NULLPTR, QSerialPortInfo* info = Q_NULLPTR);
    ~PlotterDisplay();

public slots:
    void update(const QByteArray& data) override;

private:
    Q_OBJECT;
    QScriptLine* series;

    void initLayout();
};