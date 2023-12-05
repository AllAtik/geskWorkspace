#include "usr_general.h"

#define _USR_PERCENTAGE_LIMIT 10

_io bool GsmProc(void);
_io void ClearFlagProc(void);
_io char *PreparePublishJsonDataProc(void);
_io void PrepareRequsetJsonDataProc(void);
_io void AddDataToBufProc(void);

_io int16_t m_logRawDataBuf[144];
_io uint8_t m_logRawDataBufCnt;
_io uint32_t m_startTs;

// gsm flags
extern bool g_gsmErrorFlag;
extern bool m_eReceiveGsmDataCameOkFlg;
extern bool m_eReceiveGsmDataCameFlg;
extern uint8_t m_receiveGsmEndBuf[512];

extern bool g_accelometerWakeUpFlag;

// gsm check
// extern bool m_eReceiveGsmDataCameOkFlg;
// extern bool m_eReceiveGsmDataCameFlg;
// extern uint8_t m_receiveGsmEndBuf[512];


// for mqtt payload
char m_JsonDataBuf[256];
char *json;
bool startParser = false; // request parser

// json payload
char *mcu = "STM32L051";
char *version = "8.4.1";
uint8_t lat, lon = 0;


void UsrProcess(void)
{
    _io bool _sendDataFlg = true;
    _io bool _isTankFullState = false;
    _io bool _fullnessLimitExdeed = false;
    _io bool _isCleaned = false;
    _io bool _fireAlarmFlag = false;
    _io uint8_t _coverAlarmFlag = 0;
    

    if (g_dailyResetTimer > _USR_SYSTEM_DAILY_RESET_TIME)
        HAL_NVIC_SystemReset();

    if (_sendDataFlg)                                                  // UsrSystemInitial --> UsrProcessDecideFirstState
    {
        #ifdef __usr_process_log
          __logsw("DEVICE COME FROM RESET");
        #endif
        g_packageEventBits |= _USR_SYSTEM_EVENT_BITS_DEVICE_RESET;
        g_dataSendTs = UL_RtcGetTs();
    }

    if (!g_sensorsReadingFlag)                                           // usr_sensor den false olarak geliyor.
    {
        AddDataToBufProc();                                              // 144 byte lik diziyi doldurmak

        if (_coverAlarmFlag != g_sAllSensorValues.halleffectAlarmStatus)
        {
            _coverAlarmFlag = g_sAllSensorValues.halleffectAlarmStatus;
            #ifdef __usr_process_log
                __logse("Cover Status Changed, last status %02x,", _coverAlarmFlag);
                if( _coverAlarmFlag == 1)                                // sadece top cover acik
                {
                    __logse("Top Cover Open, Battery Cover Closed");
                }
                else if(_coverAlarmFlag == 2)                            // sadece bat cover acik
                {
                    __logse("Top Cover Closed, Battery Cover Open");
                }
                else if(_coverAlarmFlag == 3)                            // ikisi de acik
                {
                    __logse("Top Cover and Battery Cover Open");
                }
                else                                                     // hepsi kapali
                {
                    __logse("Top Cover and Battery Cover Closed");
                }
            #endif
            g_packageEventBits |= _USR_SYSTEM_EVENT_BITS_COVERS_ALARM;

            _sendDataFlg = true;
        }

        if (_fireAlarmFlag != g_fireAlarmFlag)
        {
            _fireAlarmFlag = g_fireAlarmFlag;
            g_packageEventBits |= _USR_SYSTEM_EVENT_BITS_FIRE_ALARM;
            #ifdef __usr_process_log
                __logse("Fire alarm status was changed %d", (int)_fireAlarmFlag);
            #endif
            _sendDataFlg = true;
        }

        if (g_sAllSensorValues.distanceValue != _USR_SENSOR_DISTANCE_ERROR_VALUE)
        {
            if (g_sAllSensorValues.distanceValue <= g_sNvsDeviceInfo.fullAlarmLimit)
            {
                if (!_isTankFullState)
                {
                    #ifdef __usr_process_log
                        __logse("Enter full alarm");
                    #endif
                    g_packageEventBits |= _USR_SYSTEM_EVENT_BITS_FULL_ALARM;
                    _sendDataFlg = true;
                }
                _isTankFullState = true;
            }
            else
            {
                if (_isTankFullState)
                {
                    #ifdef __usr_process_log
                        __logsw("Exit full alarm");
                    #endif
                    g_packageEventBits |= _USR_SYSTEM_EVENT_BITS_FULL_ALARM;
                    _sendDataFlg = true;
                }
                _isTankFullState = false;
            }

            if (g_sAllSensorValues.distanceValue <= g_sNvsDeviceInfo.fullnessAlarmLimit)
            {
                if (!_fullnessLimitExdeed)
                {
                    #ifdef __usr_process_log
                        __logsw("Enter fullness alarm");
                    #endif
                }

                _fullnessLimitExdeed = true;
            }
            else if (_fullnessLimitExdeed && (g_sAllSensorValues.distanceValue >= (g_sNvsDeviceInfo.depthAlarmLimit * ((100 - g_sNvsDeviceInfo.toleranceValue) / 100))))
            {
                #ifdef __usr_process_log
                    __logsi("Tank has been emptied !");
                #endif
                g_packageEventBits |= _USR_SYSTEM_EVENT_BITS_FULLNESS_ALARM;
                _sendDataFlg = true;
                _isCleaned = true;
                _fullnessLimitExdeed = false;
            }
        }
        #ifdef __usr_process_log
            __logsi("Data Process Completed. AlarmCase: %d\n", g_packageEventBits);

            if(_sendDataFlg)
                __logsw("ts: %d, Data Send mission STARTED. Reason for Sending Data: Alarm case detection", UL_RtcGetTs());
        #endif

        if ((UL_RtcGetTs() - g_dataSendTs) >= g_sNvsDeviceInfo.sendingDataInterval)   // periyodik data gönderme zamanı
        {
            g_packageEventBits |= _USR_SYSTEM_EVENT_BITS_PERIODIC_DATA_SEND;
            _sendDataFlg = true;
            #ifdef __usr_process_log
                __logsw("Dummy data send %d", UL_RtcGetTs());
            #endif
        }
    }

    #ifdef __usr_process_log
      if (!_sendDataFlg)
      {
          __logsw("ts: %d, NOT Alarm Case or Dummy Data Interval \n", UL_RtcGetTs());  
      }
    #endif
    
    if (_sendDataFlg)
    {
        g_dataSendTs = UL_RtcGetTs();
        // if (GsmProc())
        if(1)
        {
            m_logRawDataBufCnt = 0;
            g_packageEventBits = 0;
            _isCleaned = false;
        }
        #ifdef __usr_process_log
            __logsw("ts: %d, This Data Sent From Gsm \n",UL_RtcGetTs());
        #endif
        
        _sendDataFlg = false;
    }
    
    #ifdef __usr_process_log
        __logsw("ts: %d, Going to Sleep Now \n", UL_RtcGetTs());
    #endif    

    g_sleepFlag = true;   // En başa gitsin başa sarsın
}


void UsrProcessLedOpenAnimation(void)
{
    if (g_sNvsDeviceInfo.deviceStatus)
        UL_LedOpenAnimation(250); // RGB blink with timeout
    else
        UL_LedPassiveAnimation(250);
}


void UsrProcessDecideFirstState(void)
{
    if (!g_sAllSensorValues.halleffectAlarmStatus)
        g_sleepFlag = false;                 // usr_sleep.c  dosyası

    g_sensorsReadingFlag = true;                                // usr_sensor.c
}


_io char *PreparePublishJsonDataProc(void)
{
    // sprintf((char*)m_JsonDataBuf, "{\"deviceReset\":%d,\"ts\":%d,\"sendType\":%d,\"mcu\":%s,\"module\":%s,\"sigq\":%d,\"ccid\":%s,\"temp\":%f,\"charge\":%f,\"fireAlarm\":%s,\"fullAlarm\":%s,\"coverAlarm\":%s,\"batteryCoverAlarm\":%s,\"fullnessAlarm\":%s,\"version\":%s,\"lat\":%d,\"lon\":%d,\"rawData\":%d}",
    // sprintf((char*)m_JsonDataBuf, "{\"deviceReset\":%d,\"ts\":%d,\"sendType\":%d,\"mcu\":%s,\"module\":%s,\"sigq\":%d,\"ccid\":%s,\"temp\":%.3f,\"charge\":%.3f,\"fireAlarm\":%s,\"fullAlarm\":%s,\"coverAlarm\":%s,\"batteryCoverAlarm\":%s,\"rawData\":%d}",
    sprintf((char *)m_JsonDataBuf, "{\"deviceReset\":%d,\"ts\":%d,\"sendType\":%d,\"mcu\":%s,\"module\":%s,\"sigq\":%d,\"ccid\":%s,\"temp\":%.3f,\"charge\":%.3f,\"fireAlarm\":%s,\"fullAlarm\":%s,\"coverAlarm\":%s,\"batteryCoverAlarm\":%s}",
            g_sNvsDeviceInfo.deviceStatus,
            g_sAllSensorValues.rtc,
            g_packageEventBits,
            mcu,
            g_sGsmModuleInfo.moduleInfoBuf,
            g_sGsmModuleInfo.signal,
            g_sGsmModuleInfo.iccidBuf,
            g_sAllSensorValues.tempValue,
            g_sAllSensorValues.batteryVoltage,
            ((g_sAllSensorValues.alarmEventGroup & 0x01) ? "true" : "false"),
            ((g_sAllSensorValues.alarmEventGroup & 0x02) ? "true" : "false"),
            ((g_sAllSensorValues.halleffectAlarmStatus & 0x01) ? "true" : "false"),
            ((g_sAllSensorValues.halleffectAlarmStatus & 0x02) ? "true" : "false"));
    //((g_sUsrSystemAlarmGroupParameters.fullnessSendFlag & 0x01) ? "true" : "false"),
    // version
    // lat,
    // lon,
    // g_sAllSensorValues.distanceValue);

    return m_JsonDataBuf;
}


_io void PrepareRequsetJsonDataProc(void)
{
    // m_eReceiveGsmDataCameOkFlg = false;
    int timeout = HAL_GetTick();
    while ((HAL_GetTick() - timeout) < 50000) // bu timeout un 5 dk'ya kadar yolu var
    {
        UsrSystemWatchdogRefresh();
        if (m_eReceiveGsmDataCameOkFlg)
        {
            //__logsi("%s", m_receiveGsmEndBuf);
            const char *deviceStatusKey = "#";
            //__logsw("Recevive : %s", m_receiveGsmEndBuf);
            const char *foundStatus = strstr((const char *)m_receiveGsmEndBuf, deviceStatusKey);
            if (foundStatus == NULL)
            {
                __logsi("deviceStatus not found\n");
            }
            else
            {
                __logsi("deviceStatus found\n");
            }
            m_eReceiveGsmDataCameOkFlg = false;
        }
    }
}

  
_io void ClearFlagProc(void)
{
    // g_wakeUpFromRtcCheckDataFlag = false;
    // g_gsmModuleInitialFlag = false; // gsm flag
    g_gsmErrorFlag = false;
}


_io void AddDataToBufProc(void)
{
    _io int32_t _lastdistanceValue = 0;

    if (g_sAllSensorValues.distanceValue != _USR_SENSOR_DISTANCE_ERROR_VALUE && _lastdistanceValue != 0)
    {
        uint8_t tryCount = 0;
        uint8_t tryErrorCount = 0;
        int percentage = 0;
    start_step:;
        if (++tryCount > 3)
        {
            _lastdistanceValue = g_sAllSensorValues.distanceValue;
            goto reading_log_step;
        }
        
        float value = g_sAllSensorValues.distanceValue - _lastdistanceValue;
        if (value < 0)
            value *= -1;
        percentage = (int)((value * 100) / _lastdistanceValue);
        if (percentage >= _USR_PERCENTAGE_LIMIT)
        {
            #ifdef __usr_process_log
                __logsw("Distance Value suddenly changed too much, Going to 1min sleep than try again. ");
            #endif
        sleep_step:;
            #ifdef __usr_process_log
                __logsw("this is %d th attempt", tryErrorCount);
            #endif
            
            UsrSleepEnterSubSleep(6); // every wakeup time is ten second so waiting is 1 minute
            UsrSensorGetDistance();
            if (g_sAllSensorValues.distanceValue == _USR_SENSOR_DISTANCE_ERROR_VALUE)
            {
                if (++tryErrorCount > 3)
                {
                    #ifdef __usr_process_log
                        __logsw("That was 3rd attempt. Data assumed true.");
                    #endif
                    goto reading_log_step;
                }    
                else
                {
                    #ifdef __usr_process_log
                        __logsw("Distance Sensor Error. Going to 1min sleep than try again. ");
                    #endif
                    goto sleep_step;
                }
                    
            }
            goto start_step;
        }
        else
            _lastdistanceValue = g_sAllSensorValues.distanceValue;
    }
    else if(g_sAllSensorValues.distanceValue != _USR_SENSOR_DISTANCE_ERROR_VALUE && !_lastdistanceValue)
            _lastdistanceValue = g_sAllSensorValues.distanceValue;   

reading_log_step:;
    if (m_logRawDataBufCnt == 0)
        m_startTs = UL_RtcGetTs();

    if (m_logRawDataBufCnt >= 144)
    {
        for (uint8_t i = 0; i < (m_logRawDataBufCnt - 1); i++)
            m_logRawDataBuf[i] = m_logRawDataBuf[i + 1];

        m_logRawDataBuf[143] = g_sAllSensorValues.distanceValue;
    }
    else
        m_logRawDataBuf[m_logRawDataBufCnt++] = g_sAllSensorValues.distanceValue;
}


_io bool GsmProc(void)
{
    UL_GsmModulePeripheral(enableGsmPeripheral);
    if (UL_GsmModuleCheck())
    {
        #ifdef __usr_process_log
            __logsi("Gsm module ok*");
        #endif
    }
    else
    {
        #ifdef __usr_process_log
            __logse("Gsm Module error!");
        #endif
    }
    if (UL_GsmModuleGetInfo(&g_sGsmModuleInfo))
    {
        #ifdef __usr_process_log
            __logsi("Gsm : %s", g_sGsmModuleInfo.iccidBuf);
            __logsi("Gsm : %s", g_sGsmModuleInfo.imeiBuf);
            __logsi("Gsm : %s", g_sGsmModuleInfo.moduleInfoBuf);
            __logsi("Gsm : %d", g_sGsmModuleInfo.signal);   
        #endif

        if (UL_GsmModuleMqttInitial((const S_GSM_MQTT_CONNECTION_PARAMETERS *)&g_sGsmMqttInitialParameters))
        {
            #ifdef __usr_process_log
                __logsi("Mqtt initial ok");
            #endif
            if (!UL_GsmModuleMqttSubcribeTopic("topic1eren", 0))
            {
                #ifdef __usr_process_log
                    __logse("Subcribe error");
                #endif    
            }
            UL_GsmModuleMqttGeneral();
            HAL_Delay(100);
            PreparePublishJsonDataProc();
            UL_GsmModuleMqttPublishTopic("topic1eren", "GsmOpened", 0, 0);
            UL_GsmModuleMqttPublishTopic("topic1eren", m_JsonDataBuf, 0, 0);
        }
        else
        {
            #ifdef __usr_process_log
                __logse("Mqtt error");
            #endif
        }
    }    
    #ifdef __usr_process_log
        __logsi("GSM SEND DATA OK!!!!");
    #endif

    UL_GsmModuleMqttPublishTopic("topic1eren", "GsmClosed", 0, 0);
    UL_GsmModuleMqttClosed();
    UL_GsmModulePeripheral(disableGsmPeripheral);
    UL_GsmModuleHardwareReset();

    return true;
}