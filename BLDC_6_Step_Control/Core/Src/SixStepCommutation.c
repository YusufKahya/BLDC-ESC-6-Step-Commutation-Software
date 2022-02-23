/*
 * SixStepCommutation.c
 *
 *  Created on: 17 Şub 2022
 *      Author: YusufKahya
 */

#include "main.h"
#include "SixStepCommutation.h"
#include "TimeTask.h"

void PeripheralsStart()
{
	  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);	// Phase A High
	  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);	// Phase B High
	  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);	// Phase C High
	  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);	// Phase A Low
	  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);	// Phase B Low
	  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);	// Phase C Low

	  HAL_COMP_Start(&hcomp1);
	  HAL_COMP_Start(&hcomp3);
	  HAL_COMP_Start(&hcomp5);

	  Motor_Control.Duty_Cycle = 50;
	  Motor_Control.Pulse_Center = 0;

	  Motor_Control.A_Out = 0;
	  Motor_Control.B_Out = 0;
	  Motor_Control.C_Out = 0;

	  Motor_Control.Rotor_Position = 0;
	  Motor_Control.Last_Trigger = 0;
	  Motor_Control.State = 1;

	  Motor_Control.Signal = 0;
	  Motor_Control.Max_Signal = 0;

	  Start_Up.Duty_Cycle = 100;
	  Start_Up.Delay_MilliSeconds = 5;
	  Start_Up.Tour = 10;
//	  Start_Up.PWM_Frequency = ( PeriphClkInit.Tim1ClockSelection/(htim1.Init.Period+1)*(htim1.Init.Prescaler+1));
	  Start_Up.State = 1;
	  Start_Up.AlignCoefficient = 2;
	  Start_Up.AlignDutyCycle = 80;

	  trigger_Sequence = 1;
//	  start_Align_Sequence = 1;

	  Trigger_Control_Index[1] = State_B_A_In;
	  Trigger_Control_Index[2] = State_C_B_In;
	  Trigger_Control_Index[3] = State_B_C_In;
	  Trigger_Control_Index[4] = State_C_A_In;
	  Trigger_Control_Index[5] = State_A_C_In;
	  Trigger_Control_Index[6] = State_A_B_In;

	  Start_Align_Index[1] = State_C_A_Out;
	  Start_Align_Index[2] = State_A_B_Out;
	  Start_Align_Index[3] = State_B_C_Out;
	  Start_Align_Index[4] = State_A_C_Out;
	  Start_Align_Index[5] = State_C_B_Out;
	  Start_Align_Index[6] = State_B_A_Out;

	  drive_Stage = START_UP;

	  HAL_TIM_Base_Start_IT(&htim4);
}

void Start_Up_Motor()
{
	static int i = 0;

	if(Start_Up_Time_Task == 1)
	{

		if(i < Start_Up.Tour*6)
		{
			Set_Motor_State(Start_Align_Index[trigger_Sequence], Start_Up.Duty_Cycle);
			trigger_Sequence = (trigger_Sequence % 6) + 1;

			i++;
		}

		else if(i == Start_Up.Tour*6)
		{
			drive_Stage = ALIGN;
			i = 0;
		}

		Start_Up_Time_Task = 0;
	}

}

void Align_Motor()
{
	static int j = 0;

	if(Start_Up_Time_Task == 1)
	{

		if(j < Start_Up.Tour*Start_Up.AlignCoefficient*6)
		{
			Set_Motor_State(Start_Align_Index[trigger_Sequence], Start_Up.AlignDutyCycle);
			trigger_Sequence = (trigger_Sequence % 6) + 1;

			j++;
		}

		else if(j == Start_Up.Tour*Start_Up.AlignCoefficient*6)
		{
			drive_Stage = RUN;
			j = 0;
		}

		Start_Up_Time_Task = 0;
	}

}

void Run_Motor()
{
	  Motor_Control.A_Out = HAL_COMP_GetOutputLevel(&hcomp1) >> 30;
	  Motor_Control.B_Out = HAL_COMP_GetOutputLevel(&hcomp3) >> 30;
	  Motor_Control.C_Out = HAL_COMP_GetOutputLevel(&hcomp5) >> 30;

	  Motor_Control.Rotor_Position = (Motor_Control.C_Out << 2) + (Motor_Control.B_Out << 1) + (Motor_Control.A_Out);

	  if(Trigger_Control_Index[Motor_Control.Rotor_Position] == (Trigger_Control_Index[Motor_Control.Last_Trigger] % 6) + 1)
	  {
		  Set_Motor_State(Motor_Control.Rotor_Position, Motor_Control.Duty_Cycle);
	  }
}

void Set_Motor_State(uint8_t State, uint16_t DutyCycle)
{
	Motor_Control.Signal = (htim1.Init.Period+1)*DutyCycle/100;	// CCR
	Motor_Control.Max_Signal = htim1.Init.Period+1;				// ARR

	switch(State)
	{

		case State_A_B_Out:
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,Motor_Control.Signal);		// A HIGH ACTIVE
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2,Motor_Control.Max_Signal);	// B LOW AVTIVE
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3,0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2,0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3,0);

			Motor_Control.Last_Trigger = State_A_B_Out;
			Motor_Control.Pulse_Center = htim1.Instance->CCR1 / 2;	//DutyCycle bu State'de iken değişirse kontrol güncellenir

			break;

		case State_A_C_Out:
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,Motor_Control.Signal);		// A HIGH ACTIVE
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3,Motor_Control.Max_Signal);	// C LOW ACTIVE
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2,0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2,0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3,0);

			Motor_Control.Last_Trigger = State_A_C_Out;

			break;

		case State_B_C_Out:
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2,Motor_Control.Signal);		// B HIGH ACTIVE
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3,Motor_Control.Max_Signal);	// C LOW ACTIVE
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2,0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3,0);

			Motor_Control.Last_Trigger = State_B_C_Out;
			Motor_Control.Pulse_Center = htim1.Instance->CCR2 / 2;	//DutyCycle bu State'de iken değişirse kontrol güncellenir

			break;

		case State_B_A_Out:
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2,Motor_Control.Signal);	// B HIGH ACTIVE
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1,Motor_Control.Max_Signal);	// A LOW ACTIVE
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2,0);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3,0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3,0);

			Motor_Control.Last_Trigger = State_B_A_Out;

			break;

		case State_C_A_Out:
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3,Motor_Control.Signal);	// C HIGH ACTIVE
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1,Motor_Control.Max_Signal);	// A LOW ACTIVE
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2,0);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3,0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2,0);

			Motor_Control.Last_Trigger = State_C_A_Out;
			Motor_Control.Pulse_Center = htim1.Instance->CCR3 / 2;	//DutyCycle bu State'de iken değişirse kontrol güncellenir

			break;

		case State_C_B_Out:
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3,Motor_Control.Signal);	// C HIGH ACTIVE
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2,Motor_Control.Max_Signal);	// B LOW ACTIVE
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3,0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2,0);

			Motor_Control.Last_Trigger = State_C_B_Out;

			break;

		default:
			break;

	}
}
