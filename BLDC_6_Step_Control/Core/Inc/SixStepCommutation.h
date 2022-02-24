/*
 * SixStepCommutation.h
 *
 *  Created on: 17 Şub 2022
 *      Author: YusufKahya
 */

#include <stdbool.h>

#ifndef INC_SIXSTEPCOMMUTATION_H_
#define INC_SIXSTEPCOMMUTATION_H_

#define MOTOR_CONTROL_TASK_HZ 20000.0f

COMP_HandleTypeDef hcomp1;
COMP_HandleTypeDef hcomp3;
COMP_HandleTypeDef hcomp5;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

RCC_PeriphCLKInitTypeDef PeriphClkInit;

	typedef enum
	{
		NullStart,
		State_A_C,
		State_B_A,
		State_B_C,
		State_C_B,
		State_A_B,
		State_C_A,
	}Trigger_Sequence;

	typedef enum
	{
		START_UP,
		ALIGN,
		RUN
	}Drive_Stage;

	typedef struct
	{
		uint8_t State;

		uint8_t Duty_Cycle;
		uint8_t Rotor_Position;

		uint32_t A_Out;
		uint32_t B_Out;
		uint32_t C_Out;

		uint32_t Signal;
		uint32_t Max_Signal;

		uint32_t Pulse_Center;

		uint8_t System_Enable;

		uint32_t ControlCnt;

		uint8_t motor_state_index;

		Drive_Stage drive_Stage;

	}MotorControl;

	typedef struct
	{
		uint8_t Duty_Cycle;
		uint16_t Tour;
		float Delay_Seconds;

//		uint32_t PWM_Frequency;
		uint8_t  State;
		uint32_t Counter;

		uint16_t AlignCoefficient;
		uint16_t AlignDutyCycle;
	}StartUp;

	const extern uint8_t Trigger_Control_State[6];

	MotorControl Motor_Control;
	StartUp Start_Up;
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
