#include "usr_lib_halleffect.h"

#define _io static
#define _iov static volatile

#define HALL_EFFECT_DELAY_MS    50

_io S_HALLEFFECT_PARAMETERS m_sHalleffectParameter; 


bool UL_HalleffectInitial(S_HALLEFFECT_PARAMETERS *f_pParameter)
{
    m_sHalleffectParameter = *f_pParameter;
    HAL_Delay(HALL_EFFECT_DELAY_MS);
    _BATTERY_COVER_HALL_POWER(1);
    _TOP_COVER_HALL_POWER(1);
    return true;
}


void UL_HalleffectPeripheral(EHalleffectControl f_eControl)
{
    if(f_eControl == enableHalleffectPeripheral)
    {
        HAL_Delay(HALL_EFFECT_DELAY_MS);
        _BATTERY_COVER_HALL_POWER(1);
        _TOP_COVER_HALL_POWER(1);
    }
    else
    {
        HAL_Delay(HALL_EFFECT_DELAY_MS);
        _BATTERY_COVER_HALL_POWER(0);
        _TOP_COVER_HALL_POWER(0);
    }
}

