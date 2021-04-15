/*
 * Button.cpp
 *
 *  Created on: Jan 10, 2021
 *      Author: fumihiko-mbp
 */

#include <Button.hpp>
#include "SystickTimer.h"

/*
 * Public member functions
 */

Button::Button(GPIO_TypeDef *pGPIOx, uint32_t uiInputPin) {
	// TODO Auto-generated constructor stub
	this->pGPIOx = pGPIOx;
	this->uiInputPin = uiInputPin;

	uiPushCount = 0;
	uiReleaseCount = 0;
	uiPushFilterTimeMs = 0;
	uiReleaseFilterTimeMs = 0;
	bPushEdge = false;
	bReleaseEdge = false;
	bRawState = false;
	bRawPushState = false;
	bPushState = false;
	bReverseEnable = false;
	bPushStateOld = false;
	bFirstFlag = false;
	ullPushFilterStartTimeMs = 0;
	ullReleaseFilterStartTimeMs = 0;
}

Button::~Button() {
	// TODO Auto-generated destructor stub
}

Button::Button(const Button &other) {
	// TODO Auto-generated constructor stub

}


void Button::Update()
{
	bRawState = (bool)LL_GPIO_IsInputPinSet(pGPIOx, uiInputPin);

	if(bReverseEnable){
		bRawPushState  = !bRawState;
		if(!bFirstFlag){
			bFirstFlag = true;
			bPushStateOld = bRawPushState;
		}
	}else{
		bRawPushState = bRawState;
	}

	if((0u < uiPushFilterTimeMs) || (0u < uiReleaseFilterTimeMs)){
		ExecuteEdgeFilter();
	}else{
		bPushState = bRawPushState;
	}

	if(bPushStateOld != bPushState){
		if(		bPushStateOld		//bPushStateOld = High
			&& !bPushState){		//bPushState = Low
			bReleaseEdge = true;
		}else{
			bPushEdge = true;
		}
	}else{
		bPushEdge = false;
		bReleaseEdge = false;
	}

	if(bPushEdge){
		uiPushCount ++;
	}
	if(bReleaseEdge){
		uiReleaseCount ++;
	}

	bPushStateOld = bPushState;
}


/*
 * Private member functions
 */

void Button::ExecuteEdgeFilter()
{
	if(bRawPushState && !bPushStateOld && !bPushFilterStartFlag){
		bPushFilterStartFlag = true;
		ullPushFilterStartTimeMs = SystickTimer_GetTimeMS();
	}else if(!bRawPushState && bPushStateOld && !bReleaseFilterStartFlag){
		bReleaseFilterStartFlag = true;
		ullReleaseFilterStartTimeMs = SystickTimer_GetTimeMS();
	}else{
		//do nothing
	}

	if(bPushFilterStartFlag){
		if(SystickTimer_IsTimeElapsed(ullPushFilterStartTimeMs, uiPushFilterTimeMs)){
			bPushFilterStartFlag = false;
			bPushState = bRawPushState;
		}else{
			bPushState = false;
		}
	}else{
		//do nothing
	}

	if(bReleaseFilterStartFlag){
		if(SystickTimer_IsTimeElapsed(ullReleaseFilterStartTimeMs, uiReleaseFilterTimeMs)){
			bReleaseFilterStartFlag = false;
			bPushState = bRawPushState;
		}else{
			bPushState = true;
		}

	}else{
		//do nothing
	}
}


