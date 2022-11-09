#include <QtWidgets>
#include <QtNetwork>
#include <QtCore>
#include <sender.h>


Sender::Sender(QObject *parent)
    : QObject(parent)
{
    this->dataFrame_ = {};
    this->lengthOfDataFrame_ = 0;
    this->prevLengthOfDataFrame_ = 0;
    this->count_ = 0;
}
/////////////////////////////////////////////////////////////////////////////////////
void Sender::sendData(QString ip, int port)
{
    this->ip_ = ip;
    this->port_ = port;
    groupAddress_ = QHostAddress(ip);
    udpSocket_ = new QUdpSocket(this);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(parseData()));
    timer->start(10);
    //parseData(port);
    //MyThread myThread_(std::thread(&Sender::parseData, this, port));
}
/////////////////////////////////////////////////////////////////////////////////////
void Sender::selectFile(QString pathOfFile)
{
    std::string pathOfFile_ = pathOfFile.toLocal8Bit().constData();
    std::ifstream file(pathOfFile_, std::ios::binary);
    std::vector<unsigned char> initBuffer_(std::istreambuf_iterator<char>(file), {});
    this->buffer_ = initBuffer_;
}
/////////////////////////////////////////////////////////////////////////////////////
void Sender::parseData()
{

//    int count_ = 0;
//    std::vector<unsigned char> dataFrame_;
//    int lengthOfDataFrame_ = 0;
//    int prevLengthOfDataFrame_ = 0;
    for (int i = prevLengthOfDataFrame_; i < buffer_.size() ; i++)
    {
        if(buffer_[i] == 0x0a && buffer_[i+1] == 0x0d)
        {
            this->count_ += 1;
            lengthOfDataFrame_ = i + 2 - prevLengthOfDataFrame_;
            dataFrame_.resize(lengthOfDataFrame_);
            for(int j = prevLengthOfDataFrame_ ; j < i + 2 ; j++)
            {
                dataFrame_[j - prevLengthOfDataFrame_] = buffer_[j];
            }
            prevLengthOfDataFrame_ = i + 2;

            QByteArray* dataGram_ = new QByteArray(reinterpret_cast<const char*>(dataFrame_.data()), dataFrame_.size());
            udpSocket_->writeDatagram(dataGram_->data(), dataGram_->size(),
                                     groupAddress_, this->port_);
            printf("sender: %d\n", this->count_);
            break;
        }

    }

}












//std::vector<unsigned char> dataFrame_ = { 0x01, 0x03, 0x5c, 0x0c, 0x1d, 0x5c, 0x03, 0x0a,
//                                       0x0d, 0x05, 0x1f, 0xae, 0x45, 0x86, 0x0a,
//                                       0x0d, 0x05, 0x1f, 0xae, 0x45, 0x86, 0x02,
//                                       0x0a, 0x0d, 0x01, 0x03, 0x5c, 0x0c, 0x1d, 0x5c, 0x03, 0x0a,
//                                       0x0d, 0x05, 0x1f, 0xae, 0x45, 0x86, 0x0a,
//                                       0x0d};
//QByteArray* dataGram_ = new QByteArray(reinterpret_cast<const char*>(dataFrame_.data()), dataFrame_.size());
//udpSocket_->writeDatagram(dataGram_->data(), dataGram_->size(),
//                         groupAddress_, port);


