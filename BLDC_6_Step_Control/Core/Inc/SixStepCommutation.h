/*
 * SixStepCommutation.h
 *
 *  Created on: 17 Şub 2022
 *      Author: ysfkh
 */

#include <stdbool.h>

#ifndef INC_SIXSTEPCOMMUTATION_H_
#define INC_SIXSTEPCOMMUTATION_H_

COMP_HandleTypeDef hcomp1;
COMP_HandleTypeDef hcomp3;
COMP_HandleTypeDef hcomp5;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

	typedef struct
	{

		uint8_t DutyCycle;
		uint8_t RotorPosition;
		uint8_t LastPosition;

		uint32_t A_Out;
		uint32_t B_Out;
		uint32_t C_Out;

		uint32_t Signal;
		uint32_t MaxSignal;

		uint32_t PulseCenter;

	}MotorControl;

	enum Control
	{
		State_A_B = 1,
		State_A_C,
		State_B_C,
		State_B_A,
		State_C_A,
		State_C_B,
	};

	MotorControl motorControl;
	enum Control State;

	// Fonksiyon Prototipleri//
	//***********************//
	void Set_Motor_State(uint8_t, uint16_t);
	void PeripheralsStart();
	//***********************//

#endif /* INC_SIXSTEPCOMMUTATION_H_ */
