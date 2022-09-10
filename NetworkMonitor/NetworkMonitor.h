#pragma once

#include <QtWidgets/QWidget>
#include "ui_NetworkMonitor.h"

class NetworkMonitor : public QWidget
{
    Q_OBJECT

public:
    NetworkMonitor(QWidget *parent = nullptr);
    ~NetworkMonitor();

private:
    Ui::NetworkMonitorClass ui;
};
