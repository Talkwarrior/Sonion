#pragma once
#include "AbstractDisplay.h"
#include <qtextbrowser.h>
#include <qlineedit.h>
#include <qpushbutton.h>

class MonitorDisplay :
    public AbstractDisplay
{
public:
    MonitorDisplay(QWidget* parent = Q_NULLPTR, QSerialPortInfo* info = Q_NULLPTR);
    ~MonitorDisplay();

public slots:
    void update(const QByteArray &data) override;
    void submit();
private:
    Q_OBJECT

    QTextBrowser* monitorWidget;
    QLineEdit* inputLine;
    QPushButton* submitButton;
};

