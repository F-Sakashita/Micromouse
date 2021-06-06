#include "IMU.hpp"
#include "main.h"
#include "DebugQueue.h"

ICM_20602 IMU::Icm20602 = ICM_20602(ICM_20602::EN_COMM_MODE_SPI);

/* private member functions */
IMU::IMU() : 
    bInitialized(false),
    uiSamplingTimeMs(0u),
    bGyroOffsetCalcStart(false),
    uiGyroOffsetSamplingNum(0u),
    uiGyroOffsetSamplingCount(0u)
{
    Calc_SetCoordValue(&stRawGyroDPS,   0.0f,0.0f,0.0f);
    Calc_SetCoordValue(&stRawAccelG,    0.0f,0.0f,0.0f);
    Calc_SetCoordValue(&stGyroDPS,      0.0f,0.0f,0.0f);
    Calc_SetCoordValue(&stGyroRPS,      0.0f,0.0f,0.0f);
    Calc_SetCoordValue(&stAccelG,       0.0f,0.0f,0.0f);
    Calc_SetCoordValue(&stGyroDeg,       0.0f,0.0f,0.0f);
    Calc_SetCoordValue(&stGyroDPSOffsetValue,       0.0f,0.0f,0.0f);
    for(uint8_t ucCount = 0; ucCount < 3; ucCount ++){
        bGyroOffsetCalcStartFlag[ucCount] = false;
        bGyroOffsetCalcCompleted[ucCount] = false;
    }
}

void IMU::CalcGyroDeg()
{
	stGyroDeg.fValueX += (stGyroDPS.fValueX * ((float)uiSamplingTimeMs / 1000.0f));
	stGyroDeg.fValueY += (stGyroDPS.fValueY * ((float)uiSamplingTimeMs / 1000.0f));
	stGyroDeg.fValueZ += (stGyroDPS.fValueZ * ((float)uiSamplingTimeMs / 1000.0f));
}

void IMU::CalcGyroOffset()
{
	/* 一定時間Gyro値をサンプリングし，　その後平均値を計算 */
	if(uiGyroOffsetSamplingCount <= uiGyroOffsetSamplingNum){
		uiGyroOffsetSamplingCount ++;
			
		if(true == bGyroOffsetCalcStartFlag[0]){
			bGyroOffsetCalcCompleted[0] = false;
			stGyroDPSOffsetValue.fValueX = Calc_GetSeqAverage(stGyroDPSOffsetValue.fValueX, stGyroDPS.fValueX, uiGyroOffsetSamplingCount);
		}

		if(true == bGyroOffsetCalcStartFlag[1]){
			bGyroOffsetCalcCompleted[1] = false;
			stGyroDPSOffsetValue.fValueY = Calc_GetSeqAverage(stGyroDPSOffsetValue.fValueY, stGyroDPS.fValueY, uiGyroOffsetSamplingCount);
		}

		if(true == bGyroOffsetCalcStartFlag[2]){
			bGyroOffsetCalcCompleted[2] = false;
			stGyroDPSOffsetValue.fValueZ = Calc_GetSeqAverage(stGyroDPSOffsetValue.fValueZ, stGyroDPS.fValueZ, uiGyroOffsetSamplingCount);
		}

	}else{
		bGyroOffsetCalcStartFlag[0] = false;  /* x */
		bGyroOffsetCalcStartFlag[1] = false;  /* y */
		bGyroOffsetCalcStartFlag[2] = false;  /* z */
		/* 平均値を計算したのち，，オフセット計算終了フラグを立てる */
		bGyroOffsetCalcCompleted[0] = true;
		bGyroOffsetCalcCompleted[1] = true;
		bGyroOffsetCalcCompleted[2] = true;
	}
}

/* public member functions */

void IMU::EnableGyroOffestCalc(uint32_t uiSamplingNum, bool bCalcX, bool bCalcY, bool bCalcZ)
{
	if(bCalcX || bCalcY || bCalcZ){
		bGyroOffsetCalcStart = true;
	}else{
		bGyroOffsetCalcStart = false;
	}
	uiGyroOffsetSamplingNum = uiSamplingNum;
	bGyroOffsetCalcStartFlag[0] = bCalcX;
	bGyroOffsetCalcStartFlag[1] = bCalcY;
	bGyroOffsetCalcStartFlag[2] = bCalcZ;

	for(uint8_t ucCount; ucCount < 3; ucCount ++){
		bGyroOffsetCalcCompleted[ucCount] = !bGyroOffsetCalcStartFlag[ucCount];
	}
    if(0 < uiGyroOffsetSamplingCount && bGyroOffsetCalcStart && !IsGyroOffsetCompleted()){
        uiGyroOffsetSamplingCount = 0;
    }
}



bool IMU::Initialize(uint32_t uiSamplingTimeMs, bool bEnableGyro, bool bEnableAccel)
{
    if(uiSamplingTimeMs <= 0){
        return false;
    }
    this->uiSamplingTimeMs = uiSamplingTimeMs; 

    Icm20602.SetSPIPort(SPI1, IMU_CS_GPIO_Port, IMU_CS_Pin);
    Icm20602.SetINTPort(IMU_INT_GPIO_Port, IMU_INT_Pin);
    if(!Icm20602.Initialize()){
        return false;    
    }

    Icm20602.SetGyroEnable(bEnableGyro);
    Icm20602.SetAccelEnable(bEnableAccel);

    bInitialized = true;
    return true;
}


bool IMU::IsGyroOffsetCompleted()
{
	bool bResult = true;
	for(uint8_t ucCount; ucCount < 3; ucCount ++){
		bResult &= bGyroOffsetCalcCompleted[ucCount];
	}
	return bResult;
}

const Coord_t& IMU::GetGyroDPS()
{
	return (const Coord_t&)stGyroDPS;
}
const Coord_t& IMU::GetAccelG()
{
	return (const Coord_t&)stAccelG;
}
const Coord_t& IMU::GetGyroRPS()
{
	return (const Coord_t&)stGyroRPS;
}
const Coord_t& IMU::GetGyroDeg()
{
	return (const Coord_t&)stGyroDeg;
}

void IMU::Update()
{
    if(bInitialized){
        if(Icm20602.IsConnected()){
            Icm20602.Update();
        }

        //Gyro
        if(Icm20602.IsGyroEnable()){
            stRawGyroDPS = Icm20602.GetGyroDPS();
            if(bGyroOffsetCalcStart){
                //Gyro　オフセット計算
                CalcGyroOffset();
                if(IsGyroOffsetCompleted()){
                    bGyroOffsetCalcStart = false;
                }
                Calc_SetCoordValue(&stGyroDPS, 
                                    stRawGyroDPS.fValueX,
                                    stRawGyroDPS.fValueY,
                                    stRawGyroDPS.fValueZ);
            }else{
                Calc_SetCoordValue(&stGyroDPS,
                                    stRawGyroDPS.fValueX - stGyroDPSOffsetValue.fValueX,
                                    stRawGyroDPS.fValueY - stGyroDPSOffsetValue.fValueY,
                                    stRawGyroDPS.fValueZ - stGyroDPSOffsetValue.fValueZ);
                CalcGyroDeg();
            }
            stGyroRPS.fValueX = Calc_ConvDegToRad(stGyroDPS.fValueX);
            stGyroRPS.fValueY = Calc_ConvDegToRad(stGyroDPS.fValueY);
            stGyroRPS.fValueZ = Calc_ConvDegToRad(stGyroDPS.fValueZ);
        }
        
        //Accel
        if(Icm20602.IsAccelEnable()){
            stRawAccelG = Icm20602.GetAccelG();
            stAccelG = stRawAccelG;

        }
    }
}

