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

COMP_HandleTypeDef hcomp2;
COMP_HandleTypeDef hcomp4;
COMP_HandleTypeDef hcomp6;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;

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
	}DriveStage;

	typedef struct
	{
		uint8_t System_Enable;

		uint8_t Blinde_Mode;

		uint8_t Duty_Cycle;
		uint8_t Rotor_Position;

		uint32_t A_Out;
		uint32_t B_Out;
		uint32_t C_Out;

		uint32_t Signal;
		uint32_t Max_Signal;
		uint32_t Pulse_Center;
		uint32_t Control_Counter;
		uint32_t Blinde_Mode_Counter;
		uint8_t  Motor_State_Index;
		uint32_t Blinde_Mode_RPM;

		DriveStage Drive_Stage;

	}MotorControl;

	typedef struct
	{
		uint8_t Duty_Cycle;
		uint16_t Tour;
		float Delay_Seconds;
		uint32_t Counter;
		uint16_t Align_Coefficient;
		uint16_t Align_DutyCycle;
	}StartUp;

	const extern uint8_t Trigger_Control_State[6];

	MotorControl Motor_Control;
	StartUp Start_Up;

	// *********** Function Prototypes Start ************ //
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

	// Stops Motor
	void Stop_Motor();
	// *********** Function Prototypes Finish ************ //

#endif /* INC_SIXSTEPCOMMUTATION_H_ */
