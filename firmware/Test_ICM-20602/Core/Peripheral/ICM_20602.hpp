/*
 * ICM_20602.hpp
 *
 *  Created on: Dec 29, 2020
 *      Author: fumihiko-mbp
 */

#ifndef PERIPHERAL_ICM_20602_HPP_
#define PERIPHERAL_ICM_20602_HPP_

#include <stdbool.h>
#include <stdint.h>

//STM32 Driver
#include "stm32f4xx.h"	//SPI_TypeDef, I2C_TypeDef, ...

class ICM_20602{
public:
	typedef enum{
		EN_COMM_MODE_SPI = 0,
		EN_COMM_MODE_I2C
	}CommMode_t;

	typedef enum{
		EN_DATA_WHO_AM_I = 0,

		EN_DATA_GYRO_X,
		EN_DATA_GYRO_Y,
		EN_DATA_GYRO_Z,

		EN_DATA_ACC_X,
		EN_DATA_ACC_Y,
		EN_DATA_ACC_Z

	}Data_t;

	ICM_20602(CommMode_t enCommMode = EN_COMM_MODE_SPI);
	~ICM_20602();

	bool SetSPIPort(SPI_TypeDef *pSPIx, GPIO_TypeDef *pSPI_CS_GPIOx, uint32_t SPI_CS_PINx);
	bool SetI2CPort(I2C_TypeDef *pI2Cx);
	bool SetINTPort(GPIO_TypeDef *pINT_GPIOx, uint32_t INT_PINx);
	bool IsINTActive();

	void Setup();

	bool IsConnected();
	void Update();


private:

	typedef enum{
		EN_REG_ADDR_XG_OFFS_TC_H		= 0x04,
		EN_REG_ADDR_XG_OFFS_TC_L		= 0x05,

		EN_REG_ADDR_YG_OFFS_TC_H		= 0x07,
		EN_REG_ADDR_YG_OFFS_TC_L		= 0x08,

		EN_REG_ADDR_ZG_OFFS_TC_H		= 0x0A,
		EN_REG_ADDR_ZG_OFFS_TC_L		= 0x0B,

		EN_REG_ADDR_SELF_TEST_X_ACCEL	= 0x0D,
		EN_REG_ADDR_SELF_TEST_Y_ACCEL	= 0x0E,
		EN_REG_ADDR_SELF_TEST_Z_ACCEL	= 0x0F,

		EN_REG_ADDR_XG_OFFS_USRH		= 0x13,
		EN_REG_ADDR_XG_OFFS_USRL		= 0x14,
		EN_REG_ADDR_YG_OFFS_USRH		= 0x15,
		EN_REG_ADDR_YG_OFFS_USRL		= 0x16,
		EN_REG_ADDR_ZG_OFFS_USRH		= 0x17,
		EN_REG_ADDR_ZG_OFFS_USRL		= 0x18,
		EN_REG_ADDR_SMPLRT_DIV			= 0x19,
		EN_REG_ADDR_CONFIG				= 0x1A,
		EN_REG_ADDR_GYRO_CONFIG			= 0x1B,
		EN_REG_ADDR_ACCEL_CONFIG		= 0x1C,
		EN_REG_ADDR_ACCEL_CONFIG_2		= 0x1D,
		EN_REG_ADDR_LP_MODE_CFG			= 0x1E,

		EN_REG_ADDR_ACCEL_WOM_X_THR		= 0x20,
		EN_REG_ADDR_ACCEL_WOM_Y_THR		= 0x21,
		EN_REG_ADDR_ACCEL_WOM_Z_THR		= 0x22,
		EN_REG_ADDR_FIFO_EN				= 0x23,

		EN_REG_ADDR_FSYNC_INT			= 0x36,
		EN_REG_ADDR_INT_PIN_CFG			= 0x37,
		EN_REG_ADDR_INT_ENABLE			= 0x38,
		EN_REG_ADDR_FIRO_WM_INT_STATUS	= 0x39,
		EN_REG_ADDR_INT_STATUS			= 0x3A,
		EN_REG_ADDR_ACCEL_XOUT_H		= 0x3B,
		EN_REG_ADDR_ACCEL_XOUT_L		= 0x3C,
		EN_REG_ADDR_ACCEL_YOUT_H		= 0x3D,
		EN_REG_ADDR_ACCEL_YOUT_L		= 0x3E,
		EN_REG_ADDR_ACCEL_ZOUT_H		= 0x3F,
		EN_REG_ADDR_ACCEL_ZOUT_L		= 0x40,
		EN_REG_ADDR_TEMP_OUT_H			= 0x41,
		EN_REG_ADDR_TEMP_OUT_L			= 0x42,
		EN_REG_ADDR_GYRO_XOUT_H			= 0x43,
		EN_REG_ADDR_GYRO_XOUT_L			= 0x44,
		EN_REG_ADDR_GYRO_YOUT_H			= 0x45,
		EN_REG_ADDR_GYRO_YOUT_L			= 0x46,
		EN_REG_ADDR_GYRO_ZOUT_H			= 0x47,
		EN_REG_ADDR_GYRO_ZOUT_L			= 0x48,

		EN_REG_ADDR_SELF_TEST_X_GYRO	= 0x50,
		EN_REG_ADDR_SELF_TEST_Y_GYRO	= 0x51,
		EN_REG_ADDR_SELF_TEST_Z_GYRO	= 0x52,

		EN_REG_ADDR_FIFO_WM_TH1			= 0x60,
		EN_REG_ADDR_FIFO_WM_TH2			= 0x61,

		EN_REG_ADDR_SIGNAL_PATH_RESET	= 0x68,
		EN_REG_ADDR_ACCEL_INTEL_CTRL	= 0x69,
		EN_REG_ADDR_USER_CTRL			= 0x6A,
		EN_REG_ADDR_PWR_MGMT_1			= 0x6B,
		EN_REG_ADDR_PWR_MGMT_2			= 0x6C,

		EN_REG_ADDR_I2C_IF				= 0x70,

		EN_REG_ADDR_FIFO_COUNTH			= 0x72,
		EN_REG_ADDR_FIFO_COUNTL			= 0x73,
		EN_REG_ADDR_FIFO_R_W			= 0x74,
		EN_REG_ADDR_WHO_AM_I			= 0x75,

		EN_REG_ADDR_XA_OFFSET_H			= 0x77,
		EN_REG_ADDR_XA_OFFSET_L			= 0x78,

		EN_REG_ADDR_YA_OFFSET_H			= 0x7A,
		EN_REG_ADDR_YA_OFFSET_L			= 0x7B,

		EN_REG_ADDR_ZA_OFFSET_H			= 0x7D,
		EN_REG_ADDR_ZA_OFFSET_L			= 0x7E,

	}RegisterAddress_t;

	void WriteRegister(RegisterAddress_t enAddr, uint8_t u8WriteData);
	uint8_t ReadRegister(RegisterAddress_t enAddr);


	void Communicate(uint8_t *pTxData, uint8_t *pRxData, uint16_t u16DataLength);
	void CommunicateSPIMode(uint8_t *pTxData, uint8_t *pRxData, uint16_t u16DataLength);
	void CommunicateI2CMode(uint8_t *pTxData, uint8_t *pRxData, uint16_t u16DataLength);


	CommMode_t enCommMode;
	bool bIsConnected;

	//STM32 Configuration
	SPI_TypeDef *pSPIx;
	GPIO_TypeDef *pSPI_CS_GPIOx;
	uint32_t 	SPI_CS_PINx;
	I2C_TypeDef *pI2Cx;

	GPIO_TypeDef *pINT_GPIOx;
	uint32_t pINT_PINx;

};	//class ICM_20602



#endif /* PERIPHERAL_ICM_20602_HPP_ */