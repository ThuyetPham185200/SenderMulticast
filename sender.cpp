#include <QtWidgets>
#include <QtNetwork>
#include <QtCore>
#include <sender.h>


MulticastSender::MulticastSender(QObject *parent)
    : QObject(parent)
{
    this->dataFrame_ = {};
    this->lengthOfDataFrame_ = 0;
    this->prevLengthOfDataFrame_ = 0;
    this->timeStampCurrent_ = 0;
    this->timeStampPrevious_ = 0;
    this->count_ = 0;
    this->frequency_ = 1000.0;
}
/////////////////////////////////////////////////////////////////////////////////////
MulticastSender::~MulticastSender()
{
    delete this->udpSocket_;
    this->udpSocket_ = nullptr;
    delete this->timer_;
    this->timer_ = nullptr;
}
/////////////////////////////////////////////////////////////////////////////////////
void MulticastSender::sendData(QString ip, int port)
{
    this->ip_ = ip;
    this->port_ = port;
    groupAddress_ = QHostAddress(ip);
    udpSocket_ = new QUdpSocket(this);
    timer_ = new QTimer(this);
    connect(timer_, SIGNAL(timeout()), this, SLOT(parseData()));
    timer_->start(1000);
}
/////////////////////////////////////////////////////////////////////////////////////
void MulticastSender::selectFile(QString pathOfFile)
{
    std::string pathOfFile_ = pathOfFile.toLocal8Bit().constData();
    std::ifstream file(pathOfFile_, std::ios::binary);
    std::vector<unsigned char> initBuffer_(std::istreambuf_iterator<char>(file), {});
    this->buffer_ = initBuffer_;
}
/////////////////////////////////////////////////////////////////////////////////////
void MulticastSender::parseData()
{
    for (int i = prevLengthOfDataFrame_; i < buffer_.size() ; i++)
    {
        if((i % 2 == 0) && buffer_[i] == 0x0a && buffer_[i+1] == 0x0d)
        {
            this->count_ += 1;
            lengthOfDataFrame_ = i + 2 - prevLengthOfDataFrame_;
            dataFrame_.resize(lengthOfDataFrame_);
            for(int j = prevLengthOfDataFrame_ ; j < i + 2; j++)
            {
                dataFrame_[j - prevLengthOfDataFrame_] = buffer_[j];
                if(j - prevLengthOfDataFrame_ < 8) arrTimeStamp[j - prevLengthOfDataFrame_] = buffer_[j];
            }
            //this->timeStampPrevious_ = this->timeStampCurrent_;
            //this->timeStampCurrent_ = hexToUint64(arrTimeStamp);
            prevLengthOfDataFrame_ = i + 2;
            float countFrequency_ = 0.0;

            if(this->count_ == 1)
            {
                this->timeStampPrevious_ = hexToUint64(arrTimeStamp);
                this->timeStampCurrent_ = hexToUint64(arrTimeStamp);
                countFrequency_ = 1000;
                 setFrequency(countFrequency_);
                timer_->start(1);
            }
            else
            {
                this->timeStampPrevious_ = this->timeStampCurrent_;
                this->timeStampCurrent_ = hexToUint64(arrTimeStamp);
                countFrequency_ = (float)1000/(this->timeStampCurrent_ - this->timeStampPrevious_);
                setFrequency(countFrequency_);
                timer_->start(this->timeStampCurrent_ - this->timeStampPrevious_);
            }
            QByteArray* dataGram_ = new QByteArray(reinterpret_cast<const char*>(dataFrame_.data()), dataFrame_.size());
            udpSocket_->writeDatagram(dataGram_->data(), dataGram_->size(),
                                     groupAddress_, this->port_);
            printf("sender: %d\n", this->count_);
            break;
        }

    }

}
/////////////////////////////////////////////////////////////////////////////////////
uint64_t MulticastSender::hexToUint64(unsigned char *data)
{
    uint64_t num = 0;
    for (int32_t i = 0; i < 8; i++) {
        num = (num << 8) + (data[i] & 0xFF);
    }
    return num;
}
/////////////////////////////////////////////////////////////////////////////////////
float MulticastSender::frequency() const
{
    return frequency_;
}
/////////////////////////////////////////////////////////////////////////////////////
void MulticastSender::setFrequency(const float &frequency)
{
    if(std::abs(frequency - frequency_) < std::numeric_limits<float>::epsilon()) return;
    frequency_ = frequency;
    Q_EMIT frequencyChanged();
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


