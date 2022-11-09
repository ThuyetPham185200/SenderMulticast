// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef SENDER_H
#define SENDER_H

#include <QObject>
#include <QHostAddress>
#include <QUdpSocket>
#include <QTimer>
#include <fstream>
#include "mythread.h"
#include <vector>

class Sender : public QObject
{
    Q_OBJECT

public:
    Sender(QObject* parent = nullptr);

    Q_INVOKABLE void sendData(QString ip, int port);
    Q_INVOKABLE void selectFile(QString pathOfFile);

private slots:
    void parseData();
private:
    QString ip_;
    int port_;
    int count_;
    std::vector<unsigned char> dataFrame_;
    int lengthOfDataFrame_;
    int prevLengthOfDataFrame_;
    QUdpSocket *udpSocket_;
    QHostAddress groupAddress_;
    std::vector<unsigned char> buffer_;
    QTimer *timer;
};

#endif
