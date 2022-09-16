#pragma once

#include <QtWidgets/QWidget>
#include "ui_NetworkMonitor.h"
#include <string>

class NetworkMonitor : public QWidget
{
    Q_OBJECT

public:
    NetworkMonitor(QWidget *parent = nullptr);

    std::string ip;
    int portNumber;

public slots:
    void on_closeButton_clicked();
    void on_scanButton_clicked();
    void updateProgress(int flag);
    void updateMessageList(std::string message);


private:
    Ui::NetworkMonitorClass ui;
};
