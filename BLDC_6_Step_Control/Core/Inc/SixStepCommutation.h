/*
 * SixStepCommutation.h
 *
 *  Created on: 17 Şub 2022
 *      Author: YusufKahya
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

RCC_PeriphCLKInitTypeDef PeriphClkInit;

	typedef struct
	{
		uint8_t State;

		uint8_t Duty_Cycle;
		uint8_t Rotor_Position;
		uint8_t Last_Trigger;

		uint32_t A_Out;
		uint32_t B_Out;
		uint32_t C_Out;

		uint32_t Signal;
		uint32_t Max_Signal;

		uint32_t Pulse_Center;
	}MotorControl;

	typedef struct
	{
		uint8_t Duty_Cycle;
		uint8_t Tour;
		uint8_t Delay_MilliSeconds;

//		uint32_t PWM_Frequency;
		uint8_t  State;
		uint32_t Counter;

		uint16_t AlignCoefficient;
		uint16_t AlignDutyCycle;
	}StartUp;

	enum Trigger_Sequence
	{
		NullStart,
		State_A_B_Out,
		State_A_C_Out,
		State_B_C_Out,
		State_B_A_Out,
		State_C_A_Out,
		State_C_B_Out,
	};

	enum Rotor_Sequence
	{
		StartNull,
		State_A_C_In,	// 2
		State_B_A_In,	// 4
		State_B_C_In,	// 3
		State_C_B_In,	// 6
		State_A_B_In,	// 1
		State_C_A_In,	// 5
		StopNull
	};
//
//	enum Start_Align_Sequence
//	{
//		null,
//		State_C_A_Str_Alg,	// 5
//		State_A_B_Str_Alg,	// 1
//		State_B_C_Str_Alg,	// 3
//		State_A_C_Str_Alg,	// 2
//		State_C_B_Str_Alg,	// 6
//		State_B_A_Str_Alg,	// 4
//	};

	enum Drive_Stage
	{
		START_UP,
		ALIGN,
		RUN
	};

	uint8_t Trigger_Control_Index[7];

	MotorControl Motor_Control;
	StartUp Start_Up;
	enum Trigger_Sequence trigger_Sequence;
	enum Rotor_Sequence rotor_Sequence;
	enum Drive_Stage drive_Stage;
//	enum Start_Align_Sequence start_Align_Sequence;

	// Function Prototypes//
	//***********************//
	//Triggers Motor State
	void Set_Motor_State(uint8_t, uint16_t);

	// Initilaize Start Parameters
	void PeripheralsStart();

	// First Stage
	void Start_Up_Motor();

	// Second Stage
	void Align_Motor();

	// Last Stage
	void Run_Motor();
	//***********************//

#endif /* INC_SIXSTEPCOMMUTATION_H_ */
