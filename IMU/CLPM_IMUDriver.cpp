#include "CLPM_IMUDriver.h"
#include <QElapsedTimer>
#include <QDebug>
CLPM_IMUDriver::CLPM_IMUDriver()
{
    m_portNum = "com4";
    isconnected = false;
    isRunning = false;
    m_manager = LpmsSensorManagerFactory();
}


bool CLPM_IMUDriver::isConnected()
{
    return isconnected;
}
bool CLPM_IMUDriver::connect()
{
    if(isconnected||isRunning){return false;}
    qDebug()<<"LpmsSensorManagerI begin";
    m_lpms = m_manager->addSensor(DEVICE_LPMS_RS232,m_portNum.c_str());
    int connectTime = 0;
    while (m_lpms->getConnectionStatus() != SENSOR_CONNECTION_CONNECTED )
    {
        connectTime++;
        QElapsedTimer timer;
        timer.start();
        while(timer.elapsed()<2){}
        if(connectTime>7500)
        {
            isconnected = false;
            return false;
            qDebug()<<"LpmsSensorManagerI connect overtime";
        }
    }
    isconnected=true;
    return true;
}

void CLPM_IMUDriver::disconnect()
{
    stopMeasurement();
    if(isconnected==true)
    {
        m_lpms->close();
        isconnected = false;
        m_manager->removeSensor(m_lpms);
    }
    else
    {
        qDebug()<<"CLPM_IMUDriver::disconnect() failed";
    }
}


bool CLPM_IMUDriver::startMeasurement()
{
    if(isconnected == true && isRunning == false )
    {
        m_lpms->run();
        int connectTime = 0;
        while (!(m_lpms->getSensorStatus() == SENSOR_STATUS_RUNNING))
        {
            connectTime++;
            QElapsedTimer timer;
            timer.start();
            while(timer.elapsed()<2){}
            if(connectTime>50){
                isRunning = false;
                return false;
                qDebug()<<"LpmsSensorManagerI connect overtime";
            }
            qDebug()<<"LpmsSensorManagerI connect m_lpms->getSensorStatus()!= SENSOR_STATUS_RUNNING";
        }
        isRunning = true;
        qDebug()<<"m_lpms->run();";
        return true;
    }
    return false;
}

bool CLPM_IMUDriver::getMeasurement(ImuData &imuData)
{
    if((m_lpms->getConnectionStatus() == SENSOR_CONNECTION_CONNECTED) && (m_lpms->hasImuData() > 0)) {
        imuData = m_lpms->getCurrentData();
        qDebug()<<"LpmsSensorManagerI getMeasurement(ImuData &imuData) imuData = m_lpms->getCurrentData();";
        return true;
    }
    return false;
}

bool CLPM_IMUDriver::stopMeasurement()
{
    if(isconnected == true){
        m_lpms->pause();
        isRunning = false;
        qDebug()<<"m_lpms->pause();";
        return true;
    }
    else
    {
        qDebug()<<"CLPM_IMUDriver::stopMeasurement() failded";
        return false;
    }
}

void CLPM_IMUDriver::reset_time_stamp()
{
    //    m_lpms->armTimestampReset();
    //    m_lpms->pollData();
    //    m_lpms->assertConnected();
//    m_lpms->clearFlashLog();
    m_lpms->setTimestamp(0);
    qDebug()<<" m_lpms->resetTimestamp();";
}

