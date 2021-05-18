/*
 * ICM_20602.cpp
 *
 *  Created on: Dec 29, 2020
 *      Author: fumihiko-mbp
 */


#include "ICM_20602.hpp"
#include "SystickTimer.h"
#include "Calculation.h"

#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_spi.h"
#include "arm_math.h"
#include <string.h>
#include "DebugConsole.h"
#include "DebugQueue.h"

#ifdef DEBUG
#include <stdio.h>
#include <main.h>

#endif
/*
 * Static member variable
 */
const ICM_20602::GyroFullScale_t ICM_20602::stGyroScale[4] =
{
	/*  Full-Scale Range,				Scale Factor [LSB/dps]	*/
	{	EN_GYRO_FULLSCALE_PM250DPS,		131.0f	},
	{	EN_GYRO_FULLSCALE_PM500DPS,		65.5f	},
	{	EN_GYRO_FULLSCALE_PM1000DPS,	32.8f	},
	{	EN_GYRO_FULLSCALE_PM2000DPS,	16.4f	},
};
const ICM_20602::AccelFullScale_t ICM_20602::stAccelScale[4] =
{
	/*  Full-Scale Range,				Scale Factor [LSB/g]	*/
	{	EN_ACCEL_FULLSCALE_PM2G,		16484.0f	},
	{	EN_ACCEL_FULLSCALE_PM4G,		8192.0f		},
	{	EN_ACCEL_FULLSCALE_PM8G,		4096.0f		},
	{	EN_ACCEL_FULLSCALE_PM16G,		2048.0f		},
};

/*
 * Public member function
 */

//Constructor
ICM_20602::ICM_20602(ICM_20602::CommMode_t enCommMode)
{
	this->enCommMode = enCommMode;

	enGyroScaleMode = EN_GYRO_FULLSCALE_PM250DPS;
	enAccelScaleMode = EN_ACCEL_FULLSCALE_PM2G;
	bIsConnected = false;
    bSetCommPort = false;
	ucGyroConfigValue		= 0x00;
	ucAccelConfigValue		= 0x00;
	ucAccelConfig2Value		= 0x00;
	ucGyroConfigValueOld	= 0x00;
	ucAccelConfigValueOld	= 0x00;
	ucAccelConfig2ValueOld	= 0x00;

	stGyroRawData.sValueX = 0x0000;
	stGyroRawData.sValueY = 0x0000;
	stGyroRawData.sValueZ = 0x0000;
	stAccelRawData.sValueX = 0x0000;
	stAccelRawData.sValueY = 0x0000;
	stAccelRawData.sValueZ = 0x0000;
	stGyroDPS.fValueX = 0.0f;
	stGyroDPS.fValueY = 0.0f;
	stGyroDPS.fValueZ = 0.0f;
	stAccelG.fValueX = 0.0f;
	stAccelG.fValueY = 0.0f;
	stAccelG.fValueZ = 0.0f;
}

//Destructor
ICM_20602::~ICM_20602()
{
	//do nothing
}

bool ICM_20602::SetSPIPort(SPI_TypeDef *pSPIx, GPIO_TypeDef *pSPI_CS_GPIOx, uint32_t SPI_CS_PINx)
{
	if(ICM_20602::EN_COMM_MODE_SPI != enCommMode){
		return false;
	}

	if(NULL == pSPIx){
		return false;
	}
	this->pSPIx = pSPIx;

	if(NULL == pSPI_CS_GPIOx){
		return false;
	}
	this->pSPI_CS_GPIOx = pSPI_CS_GPIOx;
	this->ui_SPI_CS_PINx = SPI_CS_PINx;

	LL_GPIO_SetOutputPin(this->pSPI_CS_GPIOx, this->ui_SPI_CS_PINx);
	LL_SPI_Disable(pSPIx);

    bSetCommPort = true;
	return true;
}

bool ICM_20602::SetI2CPort(I2C_TypeDef *pI2Cx)
{
	if(ICM_20602::EN_COMM_MODE_I2C != enCommMode){
		return false;
	}

	if(NULL == pI2Cx){
		return false;
	}
	this->pI2Cx = pI2Cx;

    bSetCommPort = true;
	return true;
}

bool ICM_20602::SetINTPort(GPIO_TypeDef *pINT_GPIOx, uint32_t INT_PINx)
{
	return true;
}

bool ICM_20602::Initialize()
{
    if(!bSetCommPort){
        return false;
    }
    SystickTimer_DelayMS(500);

    if(enCommMode == EN_COMM_MODE_SPI){
        LL_SPI_Enable(pSPIx);
    }else{
        /* I2C mode */
    }

#if 1
	if(WHO_AM_I_VALUE == ReadRegister(EN_REG_ADDR_WHO_AM_I)){
		bIsConnected = true;
	}else{
		bIsConnected = false;
	}

#ifdef DEBUG
	//printf("Who am I : 0x%x\n", ReadRegister(EN_REG_ADDR_WHO_AM_I));

#endif
	WriteRegister(EN_REG_ADDR_PWR_MGMT_1, 0x80);	//b10000000 = Device Reset
	SystickTimer_DelayMS(50);

	WriteRegister(EN_REG_ADDR_PWR_MGMT_1, 0x01);	//b00000001 = Auto Selects the best available clock source
	SystickTimer_DelayMS(50);

	WriteRegister(EN_REG_ADDR_PWR_MGMT_2, 0x00);	//b00000000 = Gyro and Accelerometer are all enabled
	SystickTimer_DelayMS(50);

	//WriteRegister(EN_REG_ADDR_I2C_IF, 0x40);		//SPI mode only
	//SystickTimer_DelayMS(50);

	SetGyroFullScale(EN_GYRO_FULLSCALE_PM2000DPS);	//Set Gyro Full Scale : ±2000 dps
	SetAccelFullScale(EN_ACCEL_FULLSCALE_PM16G);	//Set Accel Full Scale : ±16 G
	SetGyroConfig();	//Gyro FullScale
	SetAccelConfig();	//Accle FullScale
#endif
    return true;
}

bool ICM_20602::IsConnected()
{
	return bIsConnected;
}
void ICM_20602::Update()
{
	uint8_t ucGyroRawDataHL[6] = {0x00};
	uint8_t ucAccelRawDataHL[6] = {0x00};

	if(ucGyroConfigValue != ucGyroConfigValueOld){
		SetGyroConfig();
	}
	if(ucAccelConfigValue != ucAccelConfigValueOld){
		SetAccelConfig();
	}
	if(ucAccelConfig2Value != ucAccelConfig2ValueOld){
		SetAccelConfig2();
	}

	//Read Gyro Raw Data
	ReadRegister(EN_REG_ADDR_GYRO_XOUT_H, ucGyroRawDataHL, 6);

	//Read Accel Raw Data
	ReadRegister(EN_REG_ADDR_ACCEL_XOUT_H, ucAccelRawDataHL, 6);

	//Convert HL data to 16bits data
	stGyroRawData.sValueX	= ConvertHLDataTo16Bits(ucGyroRawDataHL[0], ucGyroRawDataHL[1]);
	stGyroRawData.sValueY	= ConvertHLDataTo16Bits(ucGyroRawDataHL[2], ucGyroRawDataHL[3]);
	stGyroRawData.sValueZ	= ConvertHLDataTo16Bits(ucGyroRawDataHL[4], ucGyroRawDataHL[5]);

	stAccelRawData.sValueX	= ConvertHLDataTo16Bits(ucAccelRawDataHL[0], ucAccelRawDataHL[1]);
	stAccelRawData.sValueY	= ConvertHLDataTo16Bits(ucAccelRawDataHL[2], ucAccelRawDataHL[3]);
	stAccelRawData.sValueZ	= ConvertHLDataTo16Bits(ucAccelRawDataHL[4], ucAccelRawDataHL[5]);

	ScaleConvert();

	//Old Config Value update
	ucGyroConfigValueOld = ucGyroConfigValue;
	ucAccelConfigValueOld = ucAccelConfigValue;
	ucAccelConfig2ValueOld = ucAccelConfig2Value;

#ifdef DEBUG
	//printf("Who am I : 0x%x\n", ReadRegister(EN_REG_ADDR_WHO_AM_I));
#endif
}

const ICM_20602::RawData_t& ICM_20602::GetGyroRawData()
{
	return (const RawData_t&)stGyroRawData;
}
const ICM_20602::RawData_t& ICM_20602::GetAccelRawData()
{
	return (const RawData_t&)stAccelRawData;
}
const Coord_t& ICM_20602::GetGyroDPS()
{
	return (const Coord_t&)stGyroDPS;
}
const Coord_t& ICM_20602::GetAccelG()
{
	return (const Coord_t&)stAccelG;
}

//
void ICM_20602::SetGyroFullScale(GyroFullScaleMode_t enScaleMode)
{
	if(enScaleMode < EN_GYRO_FULLSCALE_PM250DPS || enScaleMode > EN_GYRO_FULLSCALE_PM2000DPS){
		return;
	}

	enGyroScaleMode = enScaleMode;

	if(EN_GYRO_FULLSCALE_PM250DPS == enGyroScaleMode){
		ucGyroConfigValue = ucGyroConfigValue & 0xE7;	//b11100111 : 3,4bitのみ0をAND
	}else{
		ucGyroConfigValue = ucGyroConfigValue | ((uint8_t)enGyroScaleMode << 3);	//3bitシフトし，OR
	}
}

//
void ICM_20602::SetAccelFullScale(AccelFullScaleMode_t enScaleMode)
{
	if(enScaleMode < EN_ACCEL_FULLSCALE_PM2G || enScaleMode > EN_ACCEL_FULLSCALE_PM16G){
		return;
	}

	enAccelScaleMode = enScaleMode;

	if(EN_ACCEL_FULLSCALE_PM2G == enAccelScaleMode){
		ucAccelConfigValue = ucAccelConfigValue & 0xE7;	//b11100111 : 3,4bitのみ0をAND
	}else{
		ucAccelConfigValue = ucAccelConfigValue | ((uint8_t)enAccelScaleMode << 3);	//3bitシフトし，OR
	}
}

/*
 * Private member function
 */


//
void ICM_20602::ScaleConvert()
{
	stGyroDPS.fValueX = ConvertRawDataToFloat(stGyroRawData.sValueX, stGyroScale[enGyroScaleMode].fLSB_per_dps);
	stGyroDPS.fValueY = ConvertRawDataToFloat(stGyroRawData.sValueY, stGyroScale[enGyroScaleMode].fLSB_per_dps);
	stGyroDPS.fValueZ = ConvertRawDataToFloat(stGyroRawData.sValueZ, stGyroScale[enGyroScaleMode].fLSB_per_dps);

	stAccelG.fValueX = ConvertRawDataToFloat(stAccelRawData.sValueX, stAccelScale[enAccelScaleMode].fLSB_per_G);
	stAccelG.fValueY = ConvertRawDataToFloat(stAccelRawData.sValueY, stAccelScale[enAccelScaleMode].fLSB_per_G);
	stAccelG.fValueZ = ConvertRawDataToFloat(stAccelRawData.sValueZ, stAccelScale[enAccelScaleMode].fLSB_per_G);
}

//
void ICM_20602::SetGyroConfig()
{
	WriteRegister(EN_REG_ADDR_GYRO_CONFIG, ucGyroConfigValue);
}

//
void ICM_20602::SetAccelConfig()
{
	WriteRegister(EN_REG_ADDR_ACCEL_CONFIG, ucAccelConfigValue);
}

//
void ICM_20602::SetAccelConfig2()
{
	WriteRegister(EN_REG_ADDR_ACCEL_CONFIG_2, ucAccelConfig2Value);
}

//レジスタ書き込み（1バイト）
void ICM_20602::WriteRegister(ICM_20602::RegisterAddress_t enAddr, uint8_t ucWriteData)
{
	switch(enCommMode){
	case EN_COMM_MODE_SPI:
		CommunicateSPIMode(EN_RW_MODE_WRITE, enAddr, &ucWriteData, NULL, 1);
		break;
	case EN_COMM_MODE_I2C:
		//TODO : 追加実装
		//CommunicateI2CMode(...);
		break;
	default:
		//do nothing
		break;
	}
}
//レジスタ書き込み（複数バイト）
void ICM_20602::WriteRegister(RegisterAddress_t enStartAddr, uint8_t *pWriteData, uint16_t usLength)
{
	switch(enCommMode){
	case EN_COMM_MODE_SPI:
		CommunicateSPIMode(EN_RW_MODE_WRITE, enStartAddr, pWriteData, NULL, usLength);
		break;
	case EN_COMM_MODE_I2C:
		//TODO : 追加実装
		//CommunicateI2CMode(...);
		break;
	default:
		//do nothing
		break;
	}
}


uint8_t ICM_20602::ReadRegister(ICM_20602::RegisterAddress_t enAddr)
{
	uint8_t ucReadData = 0x00;
	switch(enCommMode){
	case EN_COMM_MODE_SPI:
		CommunicateSPIMode(EN_RW_MODE_READ, enAddr, NULL, &ucReadData, 1);
		break;
	case EN_COMM_MODE_I2C:
		//TODO : 追加実装
		//CommunicateI2CMode(...);
		break;
	default:
		//do nothing
		break;
	}
	return ucReadData;
}

void ICM_20602::ReadRegister(RegisterAddress_t enStartAddr, uint8_t *pReadData, uint16_t usLength)
{
	switch(enCommMode){
	case EN_COMM_MODE_SPI:
		CommunicateSPIMode(EN_RW_MODE_READ, enStartAddr, NULL, pReadData, usLength);
		break;
	case EN_COMM_MODE_I2C:
		//TODO : 追加実装
		//CommunicateI2CMode(...);
		break;
	default:
		//do nothing
		break;
	}
}


//
//ref : https://garberas.com/archives/1542
void ICM_20602::CommunicateSPIMode(RW_Mode_t mode, RegisterAddress_t enStartAddr, uint8_t *pTxData, uint8_t *pRxData, uint16_t usDataLength)
{
	uint16_t usCount = usDataLength;
	uint8_t ucAddress = 0x00;

	//Chip Select
	LL_GPIO_ResetOutputPin(pSPI_CS_GPIOx, ui_SPI_CS_PINx);

	//MSB Setting
	switch(mode){
	case EN_RW_MODE_READ:
		ucAddress = enStartAddr | 0x80;		//7bit目を1に設定
		break;
	case EN_RW_MODE_WRITE:
		ucAddress = enStartAddr & 0x7F;		//7bit目を0に設定
		break;
	default:
		return;		//設定外のため，即Return
		break;
	}

	//Address送信
	LL_SPI_TransmitData8(pSPIx, ucAddress);
	while(RESET == LL_SPI_IsActiveFlag_TXE(pSPIx));
	while(RESET == LL_SPI_IsActiveFlag_RXNE(pSPIx));
	LL_SPI_ReceiveData8(pSPIx); //Dummy Received

	//Data送受信
	while(0 < usCount){
		//送信
		if(NULL != pTxData){
			LL_SPI_TransmitData8(pSPIx, *pTxData++);
		}else{
			LL_SPI_TransmitData8(pSPIx, 0x00);
		}
		while(RESET == LL_SPI_IsActiveFlag_TXE(pSPIx)){
			//送信完了待ち（送信バッファが空でない時）
			//※必要ならTimeout処理
		}
		while(RESET == LL_SPI_IsActiveFlag_RXNE(pSPIx)){
			//受信完了待ち（受信バッファが空の時）
			//※必要ならTimeout処理
		}
		//受信
		if(NULL != pRxData){
			*pRxData++ = LL_SPI_ReceiveData8(pSPIx);
		}else{
			LL_SPI_ReceiveData8(pSPIx);
		}
		usCount --;
	}
	//Chip Deselect
	LL_GPIO_SetOutputPin(pSPI_CS_GPIOx, ui_SPI_CS_PINx);
}

void ICM_20602::CommunicateI2CMode(RW_Mode_t mode, RegisterAddress_t enStartAddr, uint8_t *pTxData, uint8_t *pRxData, uint16_t usDataLength)
{

}
