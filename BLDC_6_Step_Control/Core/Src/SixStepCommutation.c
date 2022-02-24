/*
 * SixStepCommutation.c
 *
 *  Created on: 17 Şub 2022
 *      Author: YusufKahya
 */

#include "main.h"
#include "SixStepCommutation.h"
#include "TimeTask.h"

const uint8_t Trigger_Control_State[6] = {State_A_B,State_A_C,State_B_C,State_B_A,State_C_A,State_C_B};

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

	  Motor_Control.Duty_Cycle = 30;
	  Motor_Control.Pulse_Center = 0;

	  Motor_Control.A_Out = 0;
	  Motor_Control.B_Out = 0;
	  Motor_Control.C_Out = 0;

	  Motor_Control.Rotor_Position = 0;
	  Motor_Control.State = 1;

	  Motor_Control.Signal = 0;
	  Motor_Control.Max_Signal = 0;

	  Start_Up.Duty_Cycle = 50;
	  Start_Up.Delay_Seconds = 0.00001f;
	  Start_Up.Tour = 3;
//	  Start_Up.PWM_Frequency = ( PeriphClkInit.Tim1ClockSelection/(htim1.Init.Period+1)*(htim1.Init.Prescaler+1));
	  Start_Up.State = 1;
	  Start_Up.AlignCoefficient = 2;
	  Start_Up.AlignDutyCycle = 50;

	  Motor_Control.drive_Stage = START_UP;

	  HAL_TIM_Base_Start_IT(&htim4);
}

void Start_Up_Motor()
{
	static int tour_Cnt = 0;

	if(Motor_Control.ControlCnt++ >= MOTOR_CONTROL_TASK_HZ*Start_Up.Delay_Seconds)
	{
		Motor_Control.ControlCnt = 0;

		Set_Motor_State(Trigger_Control_State[Motor_Control.motor_state_index], Start_Up.Duty_Cycle);

		if( (Trigger_Control_State[Motor_Control.motor_state_index] == State_C_B) && tour_Cnt++ >= Start_Up.Tour)
		{
			Motor_Control.drive_Stage = RUN;
			tour_Cnt = 0;
		}

		else
		{
			Motor_Control.motor_state_index = (Motor_Control.motor_state_index + 1) % 6;
		}

		Start_Up_Time_Task = 0;
	}

}

//void Align_Motor()
//{
//	static int j = 0;
//
//	if(Motor_Control.ControlCnt++ >= MOTOR_CONTROL_TASK_HZ*Start_Up.Delay_Seconds)
//	{
//		Motor_Control.ControlCnt = 0;
//
//		if(j < Start_Up.Tour*Start_Up.AlignCoefficient*6)
//		{
//			Motor_Control.A_Out = HAL_COMP_GetOutputLevel(&hcomp1) >> 30;
//		    Motor_Control.B_Out = HAL_COMP_GetOutputLevel(&hcomp3) >> 30;
//		    Motor_Control.C_Out = HAL_COMP_GetOutputLevel(&hcomp5) >> 30;
//
//		    Motor_Control.Rotor_Position = (Motor_Control.C_Out << 2) + (Motor_Control.B_Out << 1) + (Motor_Control.A_Out);
//
//		    if(Trigger_Control_Index[Motor_Control.Rotor_Position] == (Trigger_Control_Index[Motor_Control.Last_Trigger] % 6) + 1)
//		    {
//			    Set_Motor_State(Motor_Control.Rotor_Position, Motor_Control.Duty_Cycle);
//
//				j++;
//		    }
//
//		}
//
//		else if(j == Start_Up.Tour*Start_Up.AlignCoefficient*6)
//		{
//			Motor_Control.drive_Stage = RUN;
//			j = 0;
//		}
//
//		Start_Up_Time_Task = 0;
//	}
//
//}

void Run_Motor()
{

	  static uint8_t Next_State_Index = 0;

	  Motor_Control.A_Out = HAL_COMP_GetOutputLevel(&hcomp1) >> 30;
	  Motor_Control.B_Out = HAL_COMP_GetOutputLevel(&hcomp3) >> 30;
	  Motor_Control.C_Out = HAL_COMP_GetOutputLevel(&hcomp5) >> 30;

	  Motor_Control.Rotor_Position = (Motor_Control.C_Out << 2) + (Motor_Control.B_Out << 1) + (Motor_Control.A_Out);

	  Next_State_Index = (Motor_Control.motor_state_index + 1) % 6;

	  if(Motor_Control.Rotor_Position == Trigger_Control_State[Next_State_Index])
	  {
		  Motor_Control.motor_state_index = Next_State_Index;

		  Set_Motor_State(Trigger_Control_State[Motor_Control.motor_state_index], Motor_Control.Duty_Cycle);
	  }
}

void Set_Motor_State(uint8_t State, uint16_t DutyCycle)
{
	Motor_Control.Signal = (htim1.Init.Period+1)*DutyCycle/100;	// CCR
	Motor_Control.Max_Signal = htim1.Init.Period+1;				// ARR

	switch(State)
	{

		case State_A_B:
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,Motor_Control.Signal);		// A HIGH ACTIVE
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2,Motor_Control.Max_Signal);	// B LOW AVTIVE
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3,0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2,0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3,0);

			Motor_Control.Pulse_Center = htim1.Instance->CCR1 / 2;	//DutyCycle bu State'de iken değişirse kontrol güncellenir

			break;

		case State_A_C:
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,Motor_Control.Signal);		// A HIGH ACTIVE
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3,Motor_Control.Max_Signal);	// C LOW ACTIVE
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2,0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2,0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3,0);

			break;

		case State_B_C:
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2,Motor_Control.Signal);		// B HIGH ACTIVE
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3,Motor_Control.Max_Signal);	// C LOW ACTIVE
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2,0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3,0);

			Motor_Control.Pulse_Center = htim1.Instance->CCR2 / 2;	//DutyCycle bu State'de iken değişirse kontrol güncellenir

			break;

		case State_B_A:
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2,Motor_Control.Signal);	// B HIGH ACTIVE
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1,Motor_Control.Max_Signal);	// A LOW ACTIVE
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2,0);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3,0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3,0);

			break;

		case State_C_A:
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3,Motor_Control.Signal);	// C HIGH ACTIVE
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1,Motor_Control.Max_Signal);	// A LOW ACTIVE
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2,0);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3,0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2,0);

			Motor_Control.Pulse_Center = htim1.Instance->CCR3 / 2;	//DutyCycle bu State'de iken değişirse kontrol güncellenir

			break;

		case State_C_B:
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3,Motor_Control.Signal);	// C HIGH ACTIVE
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2,Motor_Control.Max_Signal);	// B LOW ACTIVE
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3,0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2,0);

			break;

		default:
			break;

	}
}
