#include "BatteryMonitor.hpp"
#include "adc.h"

const BatteryMonitor::Threshold_t BatteryMonitor::stThreshold = {
    8.50f,  /* Upper Threshold */
    7.20f   /* Lower Threshold */
};

const float BatteryMonitor::fGain = 3.3f * 3.0f;    /* Vref * (10k+20k)/10k */


BatteryMonitor::BatteryMonitor(float fFullVoltage)
{
    fVoltage = 0.0;
    fOffset = 0.0;
    fPercentage = 0.0;
    bIsUpperError = false;
    bIsLowerError = false;
    this->fFullVoltage = fFullVoltage;

    if(!Adc_IsStartConvert(EN_ADC_NUM_1)){
        Adc_StartConvert(EN_ADC_NUM_1);
    }
}

BatteryMonitor::~BatteryMonitor()
{

}

void BatteryMonitor::Update()
{
    if(fFullVoltage < 0.0){
        return;
    }

    fVoltage = fGain * Adc_GetRateAdcChannelValue(EN_ADC_NUM_1, DEF_ADC_CHANNEL_RANK) - fOffset;
    fPercentage = fVoltage / fFullVoltage;

    if(fVoltage < stThreshold.fLower){
        bIsUpperError = false;
        bIsLowerError = true;
    }else if(fVoltage > stThreshold.fUpper){
        bIsUpperError = true;
        bIsLowerError = false;
    }else{
        bIsUpperError = false;
        bIsLowerError = false;   
    }
}