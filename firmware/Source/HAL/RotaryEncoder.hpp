#ifndef ROTARY_ENCODER_HPP
#define ROTARY_ENCODER_HPP

#include "stdint.h"
#include "Encoder.h"

class RotaryEncoder{
    public:
        typedef enum{
            EN_ENC_FIRST = 0,
            EN_ENC_LEFT = 0,
            EN_ENC_RIGHT,
            EN_ENC_LAST,
        }EN_ENC_NAME;


        typedef enum{
            EN_DEG_MODE_FIRST = 0,
            EN_DEG_MODE_0_360 = 0,     //0~360 deg
            EN_DEG_MODE_M180_180,      //-180~180 deg
            EN_DEG_MODE_UNLIMITED,     //Unlimited
            EN_DEG_MODE_LAST,
        }EN_DEG_MODE;

        typedef enum{
            EN_RAD_MODE_FIRST = 0,
            EN_RAD_MODE_0_2PI = 0,
            EN_RAD_MODE_MPI_PI,
            EN_RAD_MODE_UNLIMITED,
            EN_RAD_MODE_LAST,
        }EN_RAD_MODE;


        static RotaryEncoder &GetInstance(EN_ENC_NAME enEncName){
            static RotaryEncoder self[EN_ENC_LAST];
            self[enEncName].SetEncoderName(enEncName);
            return self[enEncName];
        }

        bool Initialize(uint32_t uiSamplingTimeMs, bool bReverse, bool bEnableAngleUpdate=false,float fStartDeg=0.0f);
        void Update();

        int64_t GetCount(){
            return llNowCount;
        }
        float GetDegree(EN_DEG_MODE enMode=EN_DEG_MODE_M180_180);
        float GetRadian(EN_RAD_MODE enMode=EN_RAD_MODE_MPI_PI);
         //deg / sec
        float GetDPS(){
            return fNowDps;
        }
        //rad / sec
        float GetRPS(){
            return fNowRps;
        } 

    private:
        typedef struct{
            EN_ENCODER_NUM  enEncNum;
            float        fCountPerCircle;
            uint32_t        uiResolution;
            TIM_TypeDef     *pTIMx;
        }EncoderConfig_t;

        RotaryEncoder();
        ~RotaryEncoder(){;}
        RotaryEncoder(const RotaryEncoder &other);
        
        void SetEncoderName(EN_ENC_NAME enEncName){
            enName = enEncName;
        }
        
        void UpdateDegree();
        void UpdateRadian();
        void UpdateDps();
        void UpdateRps();

        const EncoderConfig_t *pEncConfig;
        bool bReverse;
        uint32_t uiSamplingTimeMs;
        bool bInitialized;
        EN_ENC_NAME enName;
        float fStartDegree;
        bool bEnableAngleUpdate;

        int64_t llNowCount;
        int64_t llOldCount;
        int32_t iDiffCount;
        float fDiffRate;
        float fNowDegree;
        float fOldDegree;
        float fNowRadian;
        float fOldRadian;
        float fNowDps;
        float fNowRps;

        static const EncoderConfig_t stEncConfig[EN_ENC_LAST];  

};

#endif
