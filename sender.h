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

class MulticastSender : public QObject
{
    Q_OBJECT
    Q_PROPERTY (float frequency READ frequency WRITE setFrequency NOTIFY frequencyChanged)

public:
    MulticastSender(QObject* parent = nullptr);
    ~MulticastSender();
    uint64_t hexToUint64(unsigned char *data);
    Q_INVOKABLE void sendData(QString ip, int port);
    Q_INVOKABLE void selectFile(QString pathOfFile);
    float frequency() const;
    void setFrequency(const float &frequency);

private slots:
    void parseData();
Q_SIGNALS:
     void frequencyChanged();
private:
    QString ip_;
    int port_;
    int count_;
    float frequency_;
    unsigned char arrTimeStamp[8];
    uint64_t timeStampCurrent_;
    uint64_t timeStampPrevious_;
    std::vector<unsigned char> dataFrame_;
    int lengthOfDataFrame_;
    int prevLengthOfDataFrame_;
    QUdpSocket *udpSocket_;
    QHostAddress groupAddress_;
    std::vector<unsigned char> buffer_;
    QTimer *timer_;
};

#endif
