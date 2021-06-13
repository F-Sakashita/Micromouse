#include "PidController.hpp"
#include <cfloat>
#include <cmath>
#include <arm_math.h>

/* Public */
PidController::PidController()
{
    bInitialized = false;
    uiSamplingTimeMs = 0u;
    fSamplingTimeSec = 0.0f;     
    enMode = EN_MODE_NORMAL;            
    enType = EN_PID_TYPE_VELOCITY;           
    bEnableOutput = false;         
    fGain[EN_GAIN_P] = 0.0f;
    fGain[EN_GAIN_I] = 0.0f;
    fGain[EN_GAIN_D] = 0.0f;  
    fPlusOutputLimit = 0.0f;
    fMinusOutputLimit = 0.0f;
    fNowOutput = 0.0f;           
    fOldOutput = 0.0f;          
    fCutoffFreqD = 0.0f;        
    fTimeConstantD = 0.0f;       
    bEnableLimited = false;        
    bFirstFlag = false;            
    fNowDiff = 0.0f;             
    fOld1Diff = 0.0f;            
    fOld2Diff = 0.0f;
    fOutput_I_Sum = 0.0f;
    fNowTarget = 0.0f;
}

PidController::~PidController()
{
}

bool PidController::Initialize(uint32_t uiSamplingTimeMs, float fCutoffFreqD, PidMode_t enMode, PidType_t enType)
{
    if(uiSamplingTimeMs <= 0 || fCutoffFreqD <= 0.0f){
        return false;
    }
    this->uiSamplingTimeMs = uiSamplingTimeMs;
    fSamplingTimeSec = static_cast<float>(this->uiSamplingTimeMs) / 1000.0f;

    this->fCutoffFreqD = fCutoffFreqD;
    this->fTimeConstantD = 1.0f / (2.0f * M_PI * this->fCutoffFreqD);
    this->enMode = enMode;
    this->enType = enType;
    bInitialized = true;
    bFirstFlag = true;
    return true;
 }

void PidController::SetGain(Gain_t enGain, float fValue)
{
    if(enGain >= EN_GAIN_LAST || fValue < 0.0f){
        bEnableOutput = false;
        return;
    }
    
    //速度型PID時のIゲインは必ず0より大きい値に設定すること
    //0にすると出力が加算されない
    if(EN_GAIN_I == enGain && fValue <= 0.0f && EN_PID_TYPE_VELOCITY == enType){
        bEnableOutput = false;
        return;
    }

    fGain[enGain] = fValue;
    bEnableOutput = true;
}
void PidController::SetAllGain(float fPGain, float fIGain, float fDGain)
{
    if(fPGain < 0.0f || fIGain < 0.0f || fDGain < 0.0f){
        bEnableOutput = false;
        return;
    }

    fGain[EN_GAIN_P] = fPGain;
    fGain[EN_GAIN_I] = fIGain;
    fGain[EN_GAIN_D] = fDGain;

    bEnableOutput = true;
}
void PidController::SetOutputLimit(float fMLimit, float fPLimit)
{
    if(fPLimit < fMLimit){
        bEnableOutput = false;
        return;
    }
    
    fPlusOutputLimit = fPLimit;
    fMinusOutputLimit = fMLimit;

    //両制限値がほぼ0なら出力制限を無効化
    if(FLT_EPSILON >= fabs(fPlusOutputLimit) && FLT_EPSILON >= fabs(fMinusOutputLimit)){
        bEnableLimited= false;
    }else{
        bEnableLimited = true;
    }
}

float PidController::GetOutput(float fTarget, float fFeedback)
{
    float fResult = 0.0f;

    fNowDiff = fTarget - fFeedback;
    fNowTarget = fTarget;
    fNowFeedback = fFeedback;

    if(bFirstFlag){
        Reset();
        bFirstFlag = false;
    }

    //出力計算
    if(bEnableOutput && bInitialized){
        switch(enMode){
        case EN_MODE_NORMAL:
            fResult = CalcOutput_Nomal();
            break;
        case EN_MODE_PI_D:
            fResult = CalcOutput_PI_D();
            break;
        case EN_MODE_I_PD:
            fResult = CalcOutput_I_PD();
            break;
        default:
            break;
        }
        
        //出力制限
        if(bEnableLimited){
            if(fResult > fPlusOutputLimit){
                fResult = fPlusOutputLimit;
            }else if(fResult < fMinusOutputLimit){
                fResult = fMinusOutputLimit;
            }
        }
    }else{
        fResult = 0.0f;
    }

    //前回値更新
    fOldFeedback = fNowFeedback;
    fOld2Diff = fOld1Diff;  //更新順番注意
    fOld1Diff = fNowDiff;   //更新順番注意
    fOldOutput = fNowOutput;
    fOldOutput_D = fNowOutput_D;

    fNowOutput = fResult;
    
    return fResult;
}

void PidController::Reset()
{
    fOldFeedback = fNowFeedback;    //現在値を採用
    fOld1Diff = fNowDiff;           //現在値を採用
    fOld2Diff = fNowDiff;           //現在値を採用
    fOldOutput_D = 0.0f;            //出力0
    fOldOutput = 0.0f;              //出力0
}

/* Private */
float PidController::CalcOutput_Nomal()
{
    float fResult = 0.0f;
    float fDeltaOutput = 0.0f;
    switch(enType){
    case EN_PID_TYPE_VELOCITY:
        fDeltaOutput = fGain[EN_GAIN_P] * (fNowDiff - fOld1Diff);               //P Control
        fDeltaOutput += fGain[EN_GAIN_I] * (fNowDiff) * (fSamplingTimeSec);     //I Control
        fDeltaOutput += CalcOutput_D_VelType();                //D Control
        fResult = fDeltaOutput + fOldOutput;
        break;
    case EN_PID_TYPE_POSITION:
        fResult = fGain[EN_GAIN_P] * fNowDiff;
        //制御量が目標値を超えたらIゲインの出力を0にする
        if(fabs(fNowTarget) < fabsf(fNowFeedback)){
            fOutput_I_Sum = 0.0f;
        }
        fOutput_I_Sum += fGain[EN_GAIN_I] * fNowDiff * fSamplingTimeSec;
        
        fResult += fOutput_I_Sum;
        fResult += CalcOutput_D_PosType();
        break;
    default:
        break;
    }
    return fResult;
}
float PidController::CalcOutput_PI_D()
{
    float fResult = 0.0f;
    float fDeltaOutput = 0.0f;
    switch(enType){
    case EN_PID_TYPE_VELOCITY:

        fResult = fDeltaOutput + fOldOutput;
        break;
    case EN_PID_TYPE_POSITION:
        break;
    default:
        break;
    }
    return fResult;
}
float PidController::CalcOutput_I_PD()
{
    float fResult = 0.0f;
    float fDeltaOutput = 0.0f;

    switch(enType){
    case EN_PID_TYPE_VELOCITY:
        fResult = fDeltaOutput + fOldOutput;
        break;
    case EN_PID_TYPE_POSITION:
        break;
    default:
        break;
    }
    return fResult;
}

float PidController::CalcOutput_D_VelType()
{
    float fDeltaOutput_d = 0.0f;
    switch(enMode){
    case EN_MODE_NORMAL:
        //ΔYd = (Kd * (e[k] - e[k-1]) - dt * Yd[k-1]) / (dt + Tc)
        fDeltaOutput_d  = (fGain[EN_GAIN_D] * (fNowDiff - fOld1Diff) - fSamplingTimeSec * fOldOutput_D) / (fSamplingTimeSec + fTimeConstantD);
        break;
    case EN_MODE_PI_D:
    case EN_MODE_I_PD:
        //ΔYd = (Kd * (F[k-1] - F[k]) - dt * Yd[k-1]) / (dt + Tc)
        //e[k] = T[k] - F[k] 
        //T:目標値, F:フィードバック値
        fDeltaOutput_d = (fGain[EN_GAIN_D] * (fOldFeedback - fNowFeedback) - fSamplingTimeSec * fOldOutput_D) / (fSamplingTimeSec + fTimeConstantD);
        break;
    default:
        break;
    }
    
    //差分値を加算
    //Yd[k] = Yd[k-1] + ΔYd
    fNowOutput_D = fOldOutput_D + fDeltaOutput_d;

    return fNowOutput_D;
}


float PidController::CalcOutput_D_PosType()
{
    switch(enMode){
    case EN_MODE_NORMAL:
        //Yd[k] = (Kd * (e[k] - e[k-1]) + Yd[k-1] * Tc) / (dt + Tc)
        fNowOutput_D  = (fGain[EN_GAIN_D] * (fNowDiff - fOld1Diff) + fTimeConstantD * fOldOutput_D) / (fSamplingTimeSec + fTimeConstantD);
        break;
    case EN_MODE_PI_D:
    case EN_MODE_I_PD:
        //ΔYd = (Kd * (F[k-1] - F[k]) - dt * Yd[k-1]) / (dt + Tc)
        //e[k] = T[k] - F[k] 
        //T:目標値, F:フィードバック値
        //fNowOutput_D = (fGain[EN_GAIN_D] * (fOldFeedback - fNowFeedback) - fSamplingTimeSec * fOldOutput_D) / (fSamplingTimeSec + fTimeConstantD);
        break;
    default:
        break; 
    }

    return fNowOutput_D;
}
