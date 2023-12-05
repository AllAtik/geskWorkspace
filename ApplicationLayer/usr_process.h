#ifndef __USR_PROCESS_H
#define __USR_PROCESS_H

#include "usr_general.h"

void UsrProcess(void);
void UsrProcessDecideFirstState(void);
void UsrProcessLedOpenAnimation(void);

extern S_SENSOR_ALL_VALUES              g_sAllSensorValues;
extern S_HALLEFFECT_PARAMETERS          g_sHalleffectParameters;
extern S_LED_PARAMETERS                 g_sLedParameters;
extern S_GSM_PARAMETERS                 g_sGsmParameters;
extern S_GSM_MODULE_INFO                g_sGsmModuleInfo;
extern S_GSM_MQTT_CONNECTION_PARAMETERS g_sGsmMqttInitialParameters;
extern S_GSM_FTP                        g_sGsmFtpParameters;

#endif
