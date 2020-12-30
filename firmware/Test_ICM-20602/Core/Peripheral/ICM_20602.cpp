/*
 * ICM_20602.cpp
 *
 *  Created on: Dec 29, 2020
 *      Author: fumihiko-mbp
 */


#include "ICM_20602.hpp"
#include "SystickTimer.h"

#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_spi.h"

#ifdef DEBUG
#include <stdio.h>
#include <main.h>
#endif

/*
 * Public member function
 */

//Constructor
ICM_20602::ICM_20602(ICM_20602::CommMode_t enCommMode)
{
	this->enCommMode = enCommMode;
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
	this->SPI_CS_PINx = SPI_CS_PINx;

	LL_GPIO_SetOutputPin(this->pSPI_CS_GPIOx, this->SPI_CS_PINx);
	LL_SPI_Disable(pSPIx);

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

	return true;
}

bool ICM_20602::SetINTPort(GPIO_TypeDef *pINT_GPIOx, uint32_t INT_PINx)
{
	return true;
}

void ICM_20602::Setup()
{
	SystickTimer_DelayMS(500);
	LL_SPI_Enable(pSPIx);

#ifdef DEBUG
	printf("Who am I : 0x%x\n", ReadRegister(EN_REG_ADDR_WHO_AM_I));

#endif
	WriteRegister(EN_REG_ADDR_PWR_MGMT_1, 0x80);	//b10000000 = Device Reset
	SystickTimer_DelayMS(50);

	WriteRegister(EN_REG_ADDR_PWR_MGMT_1, 0x01);
	SystickTimer_DelayMS(50);

	WriteRegister(EN_REG_ADDR_PWR_MGMT_2, 0x00);
	SystickTimer_DelayMS(50);

	WriteRegister(EN_REG_ADDR_I2C_IF, 0x40);
	SystickTimer_DelayMS(50);

	//printf("Device Reset : 0x%x\n", ReadRegister(EN_REG_ADDR_PWR_MGMT_1));
	SystickTimer_DelayMS(500);
}
bool ICM_20602::IsConnected()
{
	return true;
}
void ICM_20602::Update()
{
#ifdef DEBUG
	//printf("Who am I : 0x%x\n", ReadRegister(EN_REG_ADDR_WHO_AM_I));
	printf("GyroZ:0x%x, 0x%x\n", ReadRegister(EN_REG_ADDR_GYRO_ZOUT_H), ReadRegister(EN_REG_ADDR_GYRO_ZOUT_L));
	//uint8_t u8ReadData = ReadRegister(EN_REG_ADDR_PWR_MGMT_1);
	//printf("Device Reset : %d,", (u8ReadData & 0x80) >> 7);
	//printf("Sleep : %d,", (u8ReadData & 0x40) >> 6);
	//printf("Cycle : %d,", (u8ReadData & 0x20) >> 5);
	//printf("Gyro_standby : %d,", (u8ReadData & 0x10) >> 4);
	//printf("Temp_dis : %d,", (u8ReadData & 0x08) >> 3);
	//printf("clksel : %d\n,", (u8ReadData & 0x07));
#endif
}

//Private member function
void ICM_20602::WriteRegister(ICM_20602::RegisterAddress_t enAddr, uint8_t u8WriteData)
{
	//SPI Mode
	uint8_t u8TxData[2] = {0x00, 0x00};
	uint8_t u8RxData[2] = {0x00, 0x00};

	switch(enCommMode){
	case EN_COMM_MODE_SPI:
		//ref : ICM020602 Datasheet, SPI Interface
		u8TxData[0] = (uint8_t)enAddr & 0x7F;	//書き込みレジスタをセットし，7bit目を0にセット
		u8TxData[1] = u8WriteData;		//書き込みデータをセット
		CommunicateSPIMode(u8TxData, u8RxData, sizeof(u8TxData)/sizeof(uint8_t));
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
	uint8_t u8ReadData = 0x00;
	uint8_t u8TxData[2] = {0x00, 0x00};
	uint8_t u8RxData[2] = {0x00, 0x00};

	switch(enCommMode){
	case EN_COMM_MODE_SPI:
		//SPI Mode
		//ref : ICM020602 Datasheet, SPI Interface
		u8TxData[0] = (uint8_t)enAddr | 0x80;	//読み出しレジスタをセットし，7bit目を1にセット

		//CommunicateSPIMode(&u8TxData[0], &u8RxData[0], 1);
		//u8ReadData = u8RxData[0];
		CommunicateSPIMode(u8TxData, u8RxData, 2);
		u8ReadData = u8RxData[1];
		break;
	case EN_COMM_MODE_I2C:
		//TODO : 追加実装
		//CommunicateI2CMode(...);
		break;
	default:
		//do nothing
		break;
	}

	return u8ReadData;
}


//
//ref : https://garberas.com/archives/1542
void ICM_20602::CommunicateSPIMode(uint8_t *pTxData, uint8_t *pRxData, uint16_t u16DataLength)
{
	uint16_t u16Count = u16DataLength;

	LL_GPIO_ResetOutputPin(pSPI_CS_GPIOx, SPI_CS_PINx);
	//LL_GPIO_ResetOutputPin(SPI2_CS_GPIO_Port, SPI2_CS_Pin);


	//if ( LL_SPI_IsActiveFlag_RXNE(pSPIx) == SET ) LL_SPI_ReceiveData8(pSPIx);
	//if ( LL_SPI_IsEnabled(pSPIx) == RESET ) LL_SPI_Enable(pSPIx);

	while(0 < u16Count){
		//__disable_irq();
		//送信
		LL_SPI_TransmitData8(pSPIx, *pTxData++);
		while(RESET == LL_SPI_IsActiveFlag_TXE(pSPIx)){
			//送信完了待ち（必要ならTimeout処理）
		}

		while(RESET == LL_SPI_IsActiveFlag_RXNE(pSPIx)){
			//受信完了待ち（必要ならTimeout処理）
		}
		//受信
		*pRxData++ = LL_SPI_ReceiveData8(pSPIx);
		//__enable_irq();
		u16Count --;
	}

	LL_GPIO_SetOutputPin(pSPI_CS_GPIOx, SPI_CS_PINx);
	//LL_GPIO_SetOutputPin(SPI2_CS_GPIO_Port, SPI2_CS_Pin);
}

void ICM_20602::CommunicateI2CMode(uint8_t *pTxData, uint8_t *pRxData, uint16_t u16DataLength)
{

}
