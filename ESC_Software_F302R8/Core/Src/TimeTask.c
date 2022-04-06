/*
 * TimeTask.c
 *
 *  Created on: Feb 17, 2022
 *      Author: YusufKahya
 */

#include "TimeTask.h"
#include "SixStepCommutation.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &htim15)
	{
		elapsedtime++;
		if(elapsedtime % 1 == 0)
		{
			saniye2++;
			rpmvalue=(RpmFlag-RpmFlagOld)*6000;
			RpmFlagOld=RpmFlag;
		}


//		timerCounter++;


//		if(timerCounter % 200 == 0)
//		{
//			Time.Task.Hz_100 = 1;
//		}

	}
}

