#include "usr_general.h"

_io void AllPeripheralFirstParametersProc(void);
#ifdef _accModuleCompile
    _io void AccelInitialProc(void);
#endif
_io void AllPeripheralDisableProc(void);
_io void PreProcessorProc(void);

S_GSM_PARAMETERS                 g_sGsmParameters;
S_GSM_MODULE_INFO                g_sGsmModuleInfo;
S_GSM_MQTT_CONNECTION_PARAMETERS g_sGsmMqttInitialParameters;
S_GSM_FTP                        g_sGsmFtpParameters;
S_ADC_PARAMETERS                 g_sAdcParameters;
S_ADC_RAW_PARAMETERS             g_sAdcRawParameters;
S_BATTERY_DATA                   g_sBatteryParameters;
S_NTC_PARAMETERS                 g_sNtcParameters;
S_HALLEFFECT_PARAMETERS          g_sHalleffectParameters;
S_ULTRASONIC_SENSOR_PARAMETERS   g_sUltrasonicParameters;
S_TIMEDATE                       g_sTimeParameters;
S_LED_PARAMETERS                 g_sLedParameters;

#ifdef _accModuleCompile
    S_ACC_PARAMETERS                 g_sAccelParameters;

    // Accelometer flags
    bool g_accelometerWakeUpFlag = false;            // harbiden uyandı flag'i
    bool g_accelometerInterruptDetectedFlag = false;
    bool g_accStatusFlag;
#endif

// Gsm Flags
bool g_gsmModuleInitialFlag = false;
bool g_gsmModuleMqttInitialFlag = false;
bool g_gsmErrorFlag = false;

// Rtc WakeUp Flag
bool g_wakeUpRtcCheckDataFlag = false;
bool g_wakeUpFromRtcCheckDataFlag = false;

uint32_t g_dataSendTs;
bool g_fireAlarmFlag;
uint32_t g_dailyResetTimer;
uint32_t g_packageEventBits;    // enum yerine geçen değişken

void UsrSystemInitial(void)
{
    PreProcessorProc();                                      // log initial
    UsrNvsInitial();                                         // EEProm Initial
    HAL_TIM_Base_Start_IT(&_USR_SYSTEM_BASE_TIMER_CHANNEL);  // start timer
    UsrSystemUpdateTsValues();                               // update timestamp
    AllPeripheralFirstParametersProc();                      // geçerli olacak ilk değerlerin verilmesi
    AllPeripheralDisableProc();                              // tüm peripherallerin disable başlatılması
    UsrProcessLedOpenAnimation();                            // Led Animation
    UsrSensorGetHalleffectStatusDirectly();                  // Halleffect Give Power And Close power   
    UsrProcessDecideFirstState();                            // Decide first state // uyanış sebebi disable olarak ayarlanır, iki tane flag ayarlanır.  g_sleepFlag = false;  ve   g_sensorsReadingFlag = true;

#ifdef _accModuleCompile
    AccelInitialProc();
#endif

}

void UsrSystemGeneral(void)
{
    UsrSystemWatchdogRefresh();      // Clean watchdog
#ifdef _accModuleCompile
    UsrSystemAccelometerGeneral();
#endif
    UsrSleepGeneral(); 
    UsrSensorGetValues(); 
    UsrProcess();
}


void UL_GsmModuleMqttSubcribeDataCallback(const char *f_pTopic, uint16_t f_topicLen, const char *f_pPayload, uint16_t f_payloadLen)
{
    __logsw("Topic: %s Data: %s", f_pTopic, f_pPayload);
}


void UL_GsmModuleMqttConnectionStatusCallback(EGsmMqttConnectionStatus f_eStatus)
{
    __logsw("Connection status : %d", f_eStatus);
}


void UsrSystemWatchdogRefresh(void)
{
    HAL_IWDG_Refresh(&hiwdg);
}


void UsrSystemHardFault(void)
{
    HAL_NVIC_SystemReset();
}


void UsrSystemUpdateTsValues(void)
{
    uint32_t timestamp = UL_RtcGetTs();

    g_sAllSensorValues.rtc = timestamp;
    #ifdef __usr_system_log
        __logse("Start ts: %d", timestamp);
    #endif
}


#ifdef _accModuleCompile
void UsrSystemAccelometerGeneral(void)
{
    _io uint32_t _acctimercount = 0;

    if (!g_accStatusFlag)
    {
        if ((HAL_GetTick() - _acctimercount) > 5000)
        {
            _acctimercount = HAL_GetTick();
            AccelInitialProc();
        }
    }
    UL_AccelometerClearFlag();
}


_io void AccelInitialProc(void)
{
    g_accStatusFlag = UL_AccelCheckChip();
    if (g_accStatusFlag)
        UL_AccelFabrication();
}
#endif


_io void AllPeripheralDisableProc(void)
{
    UL_AdcPeripheral(disableAdcPeripheral);
    UL_BatteryPeripheral(disableBatteryPeripheral);
    UL_NtcPeripheral(disableNtcPeripheral);
    UL_UltrasonicSensorPeripheral(disableUltrasonicSensor);
    UL_GsmModulePeripheral(disableGsmPeripheral);
    // UL_LedPeripheral(disableLedPeripheral);
    UL_LedAllDisable();
    UL_HalleffectPeripheral(disableHalleffectPeripheral);
    
}

_io void AllPeripheralFirstParametersProc(void)
{
    g_sLedParameters.pBluePort                                       = RGB_LED_BLUE_PWM_GPIO_Port;
    g_sLedParameters.pGreenPort                                      = RGB_LED_GREEN_PWM_GPIO_Port;
    g_sLedParameters.pRedPort                                        = RGB_LED_RED_PWM_GPIO_Port;
    g_sLedParameters.bluePin                                         = RGB_LED_BLUE_PWM_Pin;
    g_sLedParameters.greenPin                                        = RGB_LED_GREEN_PWM_Pin;
    g_sLedParameters.redPin                                          = RGB_LED_RED_PWM_Pin;

    g_sUltrasonicParameters.pUart                                    = &_USR_SYSTEM_UART_1_CHANNEL;
    g_sUltrasonicParameters.pDistanceSensorOnOffPort                 = DISTANCE_SENSOR_ON_OFF_GPIO_Port;
    g_sUltrasonicParameters.sensorOnOffPin                           = DISTANCE_SENSOR_ON_OFF_Pin;
    g_sUltrasonicParameters.sensorPowerStatus                        = GPIO_PIN_SET;
    g_sUltrasonicParameters.eSensorType                              = model1Sensor;

    g_sAdcParameters.pAdcforDma                                      = &_USR_SYSTEM_ADC_CHANNEL;
    g_sAdcRawParameters.rawTempValue                                 = 0;
    g_sAdcRawParameters.rawVrefTempValue                             = 0;
    g_sAdcRawParameters.rawBatteryHighValue                          = 0;
    g_sAdcRawParameters.rawBatteryLowValue                           = 0;

    g_sNtcParameters.pNtcPort                                        = NTC_ACTIVE_GPIO_Port;
    g_sNtcParameters.pNtcPin                                         = NTC_ACTIVE_Pin;
    g_sNtcParameters.ntcStatus                                       = GPIO_PIN_SET;

    g_sBatteryParameters.pBatteryPort                                = VBAT_ADC_ON_OFF_GPIO_Port;
    g_sBatteryParameters.pBatteryPin                                 = VBAT_ADC_ON_OFF_Pin;
    g_sBatteryParameters.batteryRawValue                             = 0;
    g_sBatteryParameters.batteryPowerStatus                          = GPIO_PIN_SET;

    g_sHalleffectParameters.batteryHalleffectPowerPort               = BATTERY_COVER_HALL_SWITCH_POWER_GPIO_Port;
    g_sHalleffectParameters.batteryHalleffectPowerPin                = BATTERY_COVER_HALL_SWITCH_POWER_Pin;
    g_sHalleffectParameters.batteryHalleffectInterruptPort           = BATTERY_COVER_HALL_SWITCH_OUT_INT_GPIO_Port;
    g_sHalleffectParameters.batteryHalleffectInterruptPin            = BATTERY_COVER_HALL_SWITCH_OUT_INT_Pin;
    g_sHalleffectParameters.topHalleffectPowerPort                   = TOP_COVER_HALL_SWITCH_POWER_GPIO_Port;
    g_sHalleffectParameters.topHalleffectPowerPin                    = TOP_COVER_HALL_SWITCH_POWER_Pin;
    g_sHalleffectParameters.topHalleffectInterruptPort               = TOP_COVER_HALL_SWITCH_OUT_INT_GPIO_Port;
    g_sHalleffectParameters.topHalleffectInterruptPin                = TOP_COVER_HALL_SWITCH_OUT_INT_Pin;
    g_sHalleffectParameters.topHalleffectStatus                      = GPIO_PIN_SET;
    g_sHalleffectParameters.batteryHalleffectStatus                  = GPIO_PIN_SET;

    g_sGsmParameters.pUart                                           = &_USR_SYSTEM_UART_2_CHANNEL;
    g_sGsmParameters.pPowerkeyPort                                   = PWRKEY_CONTROL_GPIO_Port;
    g_sGsmParameters.powerKeyPin                                     = PWRKEY_CONTROL_Pin;
    g_sGsmParameters.resetPin                                        = GPRS_POWER_ON_OFF_Pin;
    g_sGsmParameters.gsmProcessMcuStatus                             = 1;
    g_sGsmParameters.pPowerPort                                      = DC_DC_POWER_ON_OFF_GPIO_Port;
    g_sGsmParameters.powerPin                                        = DC_DC_POWER_ON_OFF_Pin;
    g_sGsmParameters.powerKeyPinEnableStatus                         = 1;
    g_sGsmParameters.powerPinEnableStatus                            = 1;
    g_sGsmParameters.resetPinEnableStatus                            = 1;
    g_sGsmParameters.gsmProcessMcuStatus                             = 1;
    g_sGsmParameters.eModuleType                                     = quectelM65GsmModule;

    g_sGsmMqttInitialParameters.sMqtt.port                           = 39039;
    g_sGsmMqttInitialParameters.sMqtt.keepAlive                      = 30;
    g_sGsmFtpParameters.port                                         = 21;
    sprintf(g_sGsmMqttInitialParameters.sGsmApn.name,                "internet");
    sprintf(g_sGsmMqttInitialParameters.sMqtt.urlBuf,                "95.70.201.96");
    sprintf(g_sGsmMqttInitialParameters.sMqtt.randomIdBuf, "%s-gesk", g_sGsmModuleInfo.imeiBuf);
    sprintf(g_sGsmFtpParameters.sGsmApn.name,                        "internet");
    sprintf(g_sGsmFtpParameters.userNameBuf,                         "larryftp");
    sprintf(g_sGsmFtpParameters.userPasswordBuf,                     "gesk2017");
    sprintf(g_sGsmFtpParameters.urlBuf,                              "159.65.112.154");
    sprintf(g_sGsmFtpParameters.fileNameBuf,                         "test.txt");
    sprintf(g_sGsmFtpParameters.filePathBuf,                         "/");

#ifdef _accModuleCompile
    g_sAccelParameters.accelPowerPort                                = ACC_POWER_GPIO_Port;
    g_sAccelParameters.accelPin                                      = ACC_POWER_Pin;
    g_sAccelParameters.accelPowerStatus                              = 1;
    g_sAccelParameters.interruptPort                                 = ACC_INT_1_GPIO_Port;
    g_sAccelParameters.interruptPin                                  = ACC_INT_1_Pin;
#endif

    UL_LogInitial();
    UL_HalleffectInitial(&g_sHalleffectParameters);
    UL_UltrasonicSensorInitial(&g_sUltrasonicParameters);
    UL_AdcInitial(&g_sAdcParameters);
    UL_BatteryInitial(&g_sBatteryParameters);
    UL_NtcInitial(&g_sNtcParameters);
    UL_LedInitial(&g_sLedParameters);
    UL_GsmModuleInitial(&g_sGsmParameters);
#ifdef _accModuleCompile
    UL_AccelInitial(&g_sAccelParameters);
#endif
}

_io void PreProcessorProc(void)
{
    HAL_Delay(500);
    #ifdef __usr_system_log 
        __logsi("********* Main app was started version number : %s  ************", _version);
    #endif
}
