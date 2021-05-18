#include "DCMotor.hpp"

const DCMotor::MotorConfig_t DCMotor::stConfig[EN_MOTOR_LAST] = {
    /*  enMotorCh,                  bDirReverse     */
    {   TB6612FNG::EN_MOTOR_CH_A,  true   },
    {   TB6612FNG::EN_MOTOR_CH_B,  true   },
};

TB6612FNG& DCMotor::rTB6612FNG = TB6612FNG::GetInstance();

/* private */
DCMotor::DCMotor()
{
	enName = EN_MOTOR_FIRST;
    bInitialized = false;
}

/* public */
bool DCMotor::Initialize(float fMaxDuty)
{
    if(EN_MOTOR_LAST <= enName){
        return false;
    }

    const MotorConfig_t *pConfig = &stConfig[enName];
    rTB6612FNG.Initialize(pConfig->enMotorCh, fMaxDuty);
    rTB6612FNG.SetDirReverse(pConfig->enMotorCh, pConfig->bDirReverse);
    rTB6612FNG.EnableStandby();
    bInitialized = true;
    return true;
}

void DCMotor::SetDuty(float fDuty)
{
    const MotorConfig_t *pConfig = &stConfig[enName];
    rTB6612FNG.SetDuty(pConfig->enMotorCh, fDuty);
}

void DCMotor::Stop()
{
    const MotorConfig_t *pConfig = &stConfig[enName];
    rTB6612FNG.Stop(pConfig->enMotorCh);
}

void DCMotor::EnableBrake()
{
    const MotorConfig_t *pConfig = &stConfig[enName];
    rTB6612FNG.SetBrake(pConfig->enMotorCh, true);
}

void DCMotor::DisableBrake()
{
    const MotorConfig_t *pConfig = &stConfig[enName];
    rTB6612FNG.SetBrake(pConfig->enMotorCh, false);
}

bool DCMotor::IsEnableBrake()
{
    const MotorConfig_t *pConfig = &stConfig[enName];
    return rTB6612FNG.IsBrake(pConfig->enMotorCh);
}

float DCMotor::IsNowDuty()
{
    const MotorConfig_t *pConfig = &stConfig[enName];
    return rTB6612FNG.IsDuty(pConfig->enMotorCh);
}

void DCMotor::Update()
{
    const MotorConfig_t *pConfig = &stConfig[enName];
    if(bInitialized){
        rTB6612FNG.Update(pConfig->enMotorCh);
    }

}


