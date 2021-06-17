#ifndef BATTERY_MONITOR_HPP
#define BATTERY_MONITOR_HPP

#include <cstdint>

class BatteryMonitor{
private:
    BatteryMonitor();
    ~BatteryMonitor();

    float fOffset;
    float fVoltage;
    float fFullVoltage;
    float fPercentage;
    bool bIsUpperError;
    bool bIsLowerError;

    bool bInitialized;
    uint32_t uiUpperErrorCount;
    uint32_t uiLowerErrorCount;

    typedef struct{
        float fUpper;
        float fLower;
    }Threshold_t;
    static const Threshold_t stThreshold;
    static const float fGain;
    #define DEF_ADC_CHANNEL_RANK    (1)
    #define DEF_ERR_COUNT_MAX       (1000)

public:
    static BatteryMonitor& GetInstance(){
        static BatteryMonitor self;
        return self;
    }

    bool Initialize(float fFullVoltage);
    void SetOffset(float fOffset){
        this->fOffset = fOffset;
    }
    float GetVoltage(){
        return fVoltage;
    }
    float GetPercentage(){
        return fPercentage;
    }
    bool IsError(){
        return bIsUpperError | bIsLowerError;
    }
    bool IsLowerError(){
        return bIsLowerError;
    }
    bool IsUpperError(){
        return bIsUpperError;
    }
    void ClearError(){
        bIsLowerError = false;
        bIsUpperError = false;
        uiUpperErrorCount = 0u;
        uiLowerErrorCount = 0u;
    }

    void Update();
};

#endif /* BATTERY_MONITOR_HPP */
