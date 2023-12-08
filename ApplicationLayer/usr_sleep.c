#include "usr_general.h"

#define _fire_alarm_log_count 6

_io void BaseTimerControlProc(ETimerControl f_eControl);
_io bool FireAlarmProc(void);
_io bool CoverAlarmProc(void);

bool g_sleepFlag = false;
_io uint8_t m_lastHallAlarm;
extern bool g_gsmModuleMqttInitialFlag;
extern bool g_gsmErrorFlag;
extern bool g_wakeUpRtcCheckDataFlag;

#ifdef _accModuleCompile
    extern bool g_accelometerWakeUpFlag;          // harbiden uyandı flag'i
    extern bool g_accelometerInterruptDetectedFlag;
    uint8_t controlPeriod_Min = 5;
    uint8_t controlPeriod_Max = 10;
    uint8_t controlInterval = 0;
    uint32_t accInterruptTime = 0;
    uint32_t lastAccInterruptTime = 0;
    uint32_t controlPeriod_Min_Time = 0;
#endif


void UsrSleepGeneral(void)
{
    if (g_sleepFlag)
    {
        bool alarmCheckFlg = false;
        bool _wakeUpRequest = false;
        g_sleepFlag = false;
        #ifdef _accModuleCompile
        g_accelometerWakeUpFlag = false;
        #endif
        m_lastHallAlarm = g_sAllSensorValues.halleffectAlarmStatus;
        int lastTs = UL_RtcGetTs();
    main_sleep:;
        MX_DMA_Deinit();
        BaseTimerControlProc(disableTimer);
    sleep_label:;
        #ifdef __usr_sleep_log
        __logse("ts: %d, Device Just Enter the Sleep !\n",UL_RtcGetTs());
        #endif

        // son koydugum, ayberk, digerleri yerine yerlestirildi, kullanildiktan sonra kapatiliyolar
        HAL_UART_DeInit(&hlpuart1);             
        BaseTimerControlProc(disableTimer);
        HAL_I2C_DeInit(&hi2c2);
        HAL_TIM_Base_DeInit(&htim2);
        HAL_TIM_Base_DeInit(&htim6);

        
        HAL_SuspendTick();
        __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
        PWR->CR |= 0x00000600;
        HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);

        g_dailyResetTimer += 10000;
        
        UsrSystemWatchdogRefresh();

        #ifdef __usr_sleep_log
        SystemClock_Config();       /// LOG GONDEREBILMEK ICIN !!! 
        __logse("ts: %d, Device Just Wake Up !!!\n",UL_RtcGetTs());
        #endif

        int ts = UL_RtcGetTs();
        if (g_sNvsDeviceInfo.deviceStatus)
        {
            if ((ts - lastTs) > 30)
            {
                #ifdef __usr_sleep_log
                __logsi("ts: %d, Wake up Reason: Cover and Fire alarms check per 30 seconds.\n",UL_RtcGetTs(), 30);
                #endif
                lastTs = ts;
                alarmCheckFlg = true;
            }
            if ((ts - g_dataSendTs) >= g_sNvsDeviceInfo.sendingDataInterval)
            {
                #ifdef __usr_sleep_log
                __logsi("ts: %d, Wake up Reason: Data Send ! There is no data sent since (%dsec). So sensor read started and dummy data will send.\n",UL_RtcGetTs(),g_sNvsDeviceInfo.sendingDataInterval);
                #endif
                if(alarmCheckFlg)
                    _wakeUpRequest = 1;
                goto wakeup_label;
            }    
            else if ((ts - g_sensorReadTs) >= g_sNvsDeviceInfo.sensorWakeUpTime)
            {
                #ifdef __usr_sleep_log
                __logsi("ts: %d, Wake up Reason: Sensor Read ! %dsec passed from last sensor read. If alarm case detected, data will be sent.\n",UL_RtcGetTs(),g_sNvsDeviceInfo.sensorWakeUpTime);
                #endif
                if(alarmCheckFlg)
                    _wakeUpRequest = 1;
                goto wakeup_label;
            }
            else if(g_dailyResetTimer > _USR_SYSTEM_DAILY_RESET_TIME)
            {
                #ifdef __usr_sleep_log
                __logsi("ts: %d, Daily Reset !.  \n",UL_RtcGetTs());
                #endif
                HAL_NVIC_SystemReset();
            }
            else
                if(!alarmCheckFlg)
                    goto sleep_label;
        }
        else
        {
            if ((ts - lastTs) >= g_sNvsDeviceInfo.deviceStatusCheckTime)
            {
                #ifdef __usr_sleep_log
                __logsi("ts: %d, deviceStatus is 0, check period hasn't come yet. Back to sleep again. Time until next check: %d  \n",UL_RtcGetTs(), 00000 );
                #endif
                goto sleep_label;
            }
            else
            {
                #ifdef __usr_sleep_log
                __logsi("ts: %d, deviceStatus is 0, wake up for deviceStatus Check Mission !.  \n",UL_RtcGetTs());
                #endif
                goto wakeup_label;
            }
        }

    wakeup_label:;
        HAL_ResumeTick();
        SystemClock_Config();
        MX_GPIO_Init();
        MX_DMA_Init();
        BaseTimerControlProc(enableTimer);

        if (alarmCheckFlg)
        {
            alarmCheckFlg = false;
            UsrSensorGetAdcAndHalleffectValues();
            #ifdef __usr_sleep_log
                __logsi("cover:%02x, temp: %.3f \n", g_sAllSensorValues.halleffectAlarmStatus, g_sAllSensorValues.tempValue);
            #endif
            if (!FireAlarmProc() && !CoverAlarmProc())
            {
                if(!_wakeUpRequest)
                {
                   #ifdef __usr_sleep_log
                   __logsi("ts: %d, Fire and Cover alarms Checked. No Alarm Condition. Back to sleep again.\n",UL_RtcGetTs());
                   #endif
                   goto main_sleep;
                }
                #ifdef __usr_sleep_log
                else 
                {
                   __logsi("ts: %d, Fire and Cover alarms Checked. No Alarm Condition. But still woken up for Sensor Read or Data Send Intervals !.\n",UL_RtcGetTs());
                }
                #endif        
            }
            #ifdef __usr_sleep_log
            else
            {
                __logsi("ts: %d, Fire and Cover alarms Checked. ALARM DETECTED. Going to SensorGetValues for Sensor Readings.\n",UL_RtcGetTs());
            }
            #endif
        }
        g_sensorsReadingFlag = true;
    }
}


void UsrSleepEnterSubSleep(uint32_t f_time)
{
    UsrSystemWatchdogRefresh();
    MX_DMA_Deinit();
    BaseTimerControlProc(disableTimer);
    int waitCount = 0;
    while (waitCount <= f_time)
    {
        HAL_SuspendTick();
        __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
        PWR->CR |= 0x00000600;
        HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
        UsrSystemWatchdogRefresh();
        waitCount++;
    }
    HAL_ResumeTick();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_DMA_Init();
    BaseTimerControlProc(enableTimer);
}


_io bool FireAlarmProc(void)
{
    _io uint8_t _temperatureArrayCnt = 0;
    _io float _temperatureBuf[_fire_alarm_log_count] = {0};

    if (_temperatureArrayCnt < _fire_alarm_log_count)
        _temperatureBuf[_temperatureArrayCnt++] = g_sAllSensorValues.tempValue;
    else
    {
        for (int i = 0; i < (_fire_alarm_log_count - 1); i++)
            _temperatureBuf[i] = _temperatureBuf[i + 1];
        _temperatureBuf[_fire_alarm_log_count - 1] = g_sAllSensorValues.tempValue;
        float diff = _temperatureBuf[_fire_alarm_log_count - 1] - _temperatureBuf[0];
        if (diff >= g_sNvsDeviceInfo.enterFireAlarmValue)
        {
            if (!g_fireAlarmFlag)
            {
                #ifdef __usr_sleep_log
                    __logse("Fire alarm detected");
                #endif
                g_fireAlarmFlag = true;
                return true;
            }
        }
        else if (diff < g_sNvsDeviceInfo.enterFireAlarmValue)
        {
            if (g_fireAlarmFlag)
            {
                #ifdef __usr_sleep_log
                    __logse("Fire alarm exit");
                #endif
                g_fireAlarmFlag = false;
                return true;
            }
        }
    }
    return false;
}

_io bool CoverAlarmProc(void)
{
    if (m_lastHallAlarm != g_sAllSensorValues.halleffectAlarmStatus)
    {
        m_lastHallAlarm = g_sAllSensorValues.halleffectAlarmStatus;
        return true;
    }

    return false;
}

_io void BaseTimerControlProc(ETimerControl f_eControl)
{
    if (f_eControl == enableTimer)
    {
        MX_TIM6_Init();
        HAL_TIM_Base_Start_IT(&_USR_SYSTEM_BASE_TIMER_CHANNEL);
    }
    else
    {
        HAL_TIM_Base_DeInit(&_USR_SYSTEM_BASE_TIMER_CHANNEL);
    }
}

void UsrSleepAdcPins(GPIO_TypeDef *f_pGpio, uint16_t f_pinGroup, GPIO_PinState f_ePinstate)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    HAL_GPIO_WritePin(f_pGpio, f_pinGroup, f_ePinstate);
    GPIO_InitStruct.Pin = f_pinGroup;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(f_pGpio, &GPIO_InitStruct);
}

void UsrSleepGpioOutPins(GPIO_TypeDef *f_pGpio, uint16_t f_pinGroup, GPIO_PinState f_ePinstate)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    HAL_GPIO_WritePin(f_pGpio, f_pinGroup, f_ePinstate);
    GPIO_InitStruct.Pin = f_pinGroup;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(f_pGpio, &GPIO_InitStruct);
}

void UsrSleepGpioInputPins(GPIO_TypeDef *f_pGpio, uint16_t f_pinGroup)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = f_pinGroup;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(f_pGpio, &GPIO_InitStruct);
}
