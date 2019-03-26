#ifndef CLPM_IMUDriver_H
#define CLPM_IMUDriver_H
#include "LpmsSensorI.h"
#include "LpmsSensorManagerI.h"
#include <QString>

using namespace std;

class CLPM_IMUDriver
{
public:
    CLPM_IMUDriver();
    CLPM_IMUDriver::~CLPM_IMUDriver()
    {
        delete m_manager;
    }
    bool connect();
    bool startMeasurement() ;
    bool getMeasurement(ImuData &imuData);
    void reset_time_stamp();
    bool stopMeasurement();
    void disconnect();
    bool isConnected();
    void setPortNum(QString m_portNum);
private:
    bool isconnected;
    bool isRunning;
    std::string m_portNum;
    LpmsSensorI* m_lpms;
    LpmsSensorManagerI* m_manager;
};

#endif // CLPM_IMUDriver_H
