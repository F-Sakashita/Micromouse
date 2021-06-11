#ifndef PID_CONTROLLER_HPP
#define PID_CONTROLLER_HPP

#include <cstdint>

class PidController{
    public:

        typedef enum{
            EN_MODE_NORMAL = 0,     //通常モード（不完全微分)
            EN_MODE_PI_D,           //PI-Dモード（微分先行）
            EN_MODE_I_PD            //I-PDモード（比例微分先行)
        }PidMode_t;
        
        typedef enum{
            EN_PID_TYPE_VELOCITY = 0,   //速度型PID
            EN_PID_TYPE_POSITION        //位置型PID
        }PidType_t;

        typedef enum{
            EN_GAIN_P = 0,
            EN_GAIN_I,
            EN_GAIN_D,
            EN_GAIN_LAST
        }Gain_t;

        PidController();
        ~PidController();
        
        bool Initialize(uint32_t uiSamplingTimeMs, float fCutoffFreqD, PidMode_t enMode=EN_MODE_NORMAL,  PidType_t enType = EN_PID_TYPE_VELOCITY);
        void SetGain(Gain_t enGain, float fValue);
        void SetAllGain(float fPGain, float fIGain, float fDGain);
        void SetOutputLimit(float fMLimit, float fPLimit);
        float GetOutput(float fTarget, float fFeedback);
        bool IsEnableOutput(){
            return bEnableOutput; 
        }
        
    private:
        PidController(const PidController &other);
        
        float CalcOutput_Nomal();   //通常PID出力計算
        float CalcOutput_PI_D();    //微分先行型PID出力計算
        float CalcOutput_I_PD();    //比例微分先行型PID出力計算
        float CalcOutput_D_PosType();       //微分項計算（不完全微分, 位置型)
        float CalcOutput_D_VelType();       //微分項計算(不完全微分, 速度型)

        bool bInitialized;          //初期化実行済フラグ
        uint32_t uiSamplingTimeMs;  //サンプリング時間[ms]
        float fSamplingTimeSec;     //サンプリング時間[sec]
        PidMode_t enMode;           //PIDモード
        PidType_t enType;           //PIDタイプ（位置型 or 速度型)
        bool bEnableOutput;         //出力有効
        float fGain[EN_GAIN_LAST];  //ゲイン
        float fPlusOutputLimit;     //正側最大出力
        float fMinusOutputLimit;    //負側最大出力
        float fNowOutput;           //現在の出力
        float fOldOutput;           //1step前の出力
        float fCutoffFreqD;         //不完全微分項のカットオフ周波数[Hz]
        float fTimeConstantD;       //不完全微分項の時定数
        bool bEnableLimited;        //出力制限(True:有効, False:無効)
        bool bFirstFlag;            //初回動作フラグ（True:初回, False:2回目以降)
        float fNowDiff;             //現在の目標値との差分
        float fOld1Diff;            //1step前の差分
        float fOld2Diff;            //2step前の差分
        float fNowFeedback;         //現在のフィードバック値
        float fOldFeedback;         //1step前のフィードバック値
        float fNowOutput_D;         //現在の微分項の出力値
        float fOldOutput_D;         //1step前微分項の出力値
        float fOutput_I_Sum;
        float fNowTarget;
};

#endif //PID_CONTROLLER_HPP