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
	if(htim == &htim4)
	{
		timerCounter++;

//		if(timerCounter % 10 == 1 )
//		{
//			Time.Task.Hz_100 = 1;
//		}

		if(timerCounter % 1 == 0)
		{
			Time.Task.Hz_1000 = 1;
		}

//		if(timerCounter % 1 == 0)
//		{
//			Time.Task.Hz_20000 = 1;
//		}
//		if(timerCounter % 2 == 0)
//		{
//			Time.Task.Hz_10000 = 1;
//		}
//		if(timerCounter % 10 == 0)
//		{
//			Time.Task.Hz_2000 = 1;
//		}
//		if(timerCounter % 20 == 0)
//		{
//			Time.Task.Hz_1000 = 1;
//		}
//		if(timerCounter % 100 == 0)
//		{
//			Time.Task.Hz_200 = 1;
//		}
//		if(timerCounter % 200 == 0)
//		{
//			Time.Task.Hz_100 = 1;
//		}

	}
}

