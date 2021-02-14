/*
 * Button.hpp
 *
 *  Created on: Jan 10, 2021
 *      Author: fumihiko-mbp
 */

#ifndef PERIPHERAL_BUTTON_HPP_
#define PERIPHERAL_BUTTON_HPP_

#include "stm32f4xx_ll_gpio.h"

class Button {
private:
	GPIO_TypeDef *pGPIOx;
	uint32_t	uiInputPin;
	uint32_t	uiPushCount;
	uint32_t 	uiReleaseCount;
	bool		bPushEdge;
	bool		bReleaseEdge;
	uint32_t 	uiPushFilterTimeMs;
	uint32_t	uiReleaseFilterTimeMs;
	bool 		bRawState;
	bool		bRawPushState;
	bool 		bPushState;
	bool		bReverseEnable;
	bool 		bPushStateOld;
	bool 		bFirstFlag;
	bool 		bPushFilterStartFlag;
	bool 		bReleaseFilterStartFlag;
	uint64_t	ullPushFilterStartTimeMs;
	uint64_t	ullReleaseFilterStartTimeMs;

	void ExecuteEdgeFilter();

public:
	Button(GPIO_TypeDef *pGPIOx, uint32_t uiInputPin);
	~Button();
	Button(const Button &other);

	void SetPushEdgeFilter(uint32_t uiFilterTimeMs = 10){
		uiPushFilterTimeMs = uiFilterTimeMs;
	}

	void SetReleaseEdgeFilter(uint32_t uiFilterTimeMs = 10){
		uiReleaseFilterTimeMs = uiFilterTimeMs;
	}

	void SetEdgeFilter(uint32_t uiPushFilterTimeMs = 10, uint32_t uiReleaseFilterTimeMs = 10){
		SetPushEdgeFilter(uiPushFilterTimeMs);
		SetReleaseEdgeFilter(uiReleaseFilterTimeMs);
	}

	void SetPushReverse(bool bReverseEnable = true){
		this->bReverseEnable = bReverseEnable;
	}

	bool IsRawState(){
		return bRawState;
	}

	bool IsRawPushState(){
		return bRawPushState;
	}

	bool IsPush(){
		return bPushState;
	}

	uint32_t IsPushCount(){
		return uiPushCount;
	}
	void ResetPushCount(){
		uiPushCount = 0;
	}
	uint32_t IsReleaseCount(){
		return uiReleaseCount;
	}
	void ResetReleaseCount(){
		uiReleaseCount = 0;
	}
	bool IsPushEdge(){
		return bPushEdge;
	}
	bool IsReleaseEdge(){
		return bReleaseEdge;
	}

	void ResetCount(){
		uiPushCount = 0;
		uiReleaseCount = 0;
	}

	void Update();
};

#endif /* PERIPHERAL_BUTTON_HPP_ */
