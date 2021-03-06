/*
 * TimeTask.h
 *
 *  Created on: Feb 17, 2022
 *      Author: YusufKahya
 */

#include "main.h"

#ifndef SRC_TIMETASK_H_
#define SRC_TIMETASK_H_

 TIM_HandleTypeDef htim15;

 uint32_t timerCounter;

typedef union
{
	struct
	{
		uint8_t Hz_1000 : 1;
		uint8_t Hz_200 : 1;
		uint8_t Hz_100 : 1;
		uint8_t Hz_50 : 1;
		uint8_t Hz_5 : 1;
	}Task;

}time_task_union;
uint32_t Start_Up_Time_Task;

 time_task_union Time;

 int elapsedtime,saniye,saniye2;
#endif /* SRC_TIMETASK_H_ */
