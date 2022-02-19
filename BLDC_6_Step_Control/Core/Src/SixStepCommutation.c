/*
 * SixStepCommutation.h
 *
 *  Created on: 17 Şub 2022
 *      Author: ysfkh
 */

#include "main.h"
#include "SixStepCommutation.h"

//void HAL_TIM_PWM_PulseFinishedHalfCpltCallback(TIM_HandleTypeDef *htim)
//{
//
//}

void PeripheralsStart()
{
//	  HAL_TIM_Base_Start_IT(&htim4);	// Time Task için gerekli global interrupt başlatılıyor

	  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);	// Phase A High
	  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);	// Phase B High
	  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);	// Phase C High
	  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);	// Phase A Low
	  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);	// Phase B Low
	  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);	// Phase C Low

	  HAL_COMP_Start(&hcomp1);	// Phase A - Neutral Point
	  HAL_COMP_Start(&hcomp3);  // Phase B - Neutral Point
	  HAL_COMP_Start(&hcomp5);  // Phase C - Neutral Point

	  motorControl.DutyCycle = 50;	// Default %50 Duty Cycle
	  motorControl.PulseCenter = 0;

	  motorControl.A_Out = 0;
	  motorControl.B_Out = 0;
	  motorControl.C_Out = 0;

	  motorControl.RotorPosition = 0;
	  motorControl.LastPosition = 1;

	  motorControl.Signal = 0;
	  motorControl.MaxSignal = 0;

	  State = 1;
}

void Set_Motor_State(uint8_t State, uint16_t DutyCycle)
{
	motorControl.Signal = (htim1.Init.Period+1)*DutyCycle/100;	// CCR
	motorControl.MaxSignal = htim1.Init.Period+1;				// ARR

	switch(State)
	{

		case State_A_B:
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,motorControl.Signal);		// A HIGH ACTIVE
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2,motorControl.MaxSignal);	// B LOW AVTIVE
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3,0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2,0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3,0);

			motorControl.LastPosition = State_A_B;
			motorControl.PulseCenter = htim1.Instance->CCR1 / 2;	//DutyCycle bu State'de iken değişirse kontrol güncellenir

			break;

		case State_A_C:
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,motorControl.Signal);		// A HIGH ACTIVE
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3,motorControl.MaxSignal);	// C LOW ACTIVE
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2,0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2,0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3,0);

			motorControl.LastPosition = State_A_C;

			break;

		case State_B_C:
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2,motorControl.Signal);		// B HIGH ACTIVE
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3,motorControl.MaxSignal);	// C LOW ACTIVE
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2,0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3,0);

			motorControl.LastPosition = State_B_C;
			motorControl.PulseCenter = htim1.Instance->CCR2 / 2;	//DutyCycle bu State'de iken değişirse kontrol güncellenir

			break;

		case State_B_A:
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2,motorControl.Signal);	// B HIGH ACTIVE
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1,motorControl.MaxSignal);	// A LOW ACTIVE
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2,0);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3,0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3,0);

			motorControl.LastPosition = State_B_A;

			break;

		case State_C_A:
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3,motorControl.Signal);	// C HIGH ACTIVE
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1,motorControl.MaxSignal);	// A LOW ACTIVE
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2,0);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3,0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2,0);

			motorControl.LastPosition = State_C_A;
			motorControl.PulseCenter = htim1.Instance->CCR3 / 2;	//DutyCycle bu State'de iken değişirse kontrol güncellenir

			break;

		case State_C_B:
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3,motorControl.Signal);	// C HIGH ACTIVE
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2,motorControl.MaxSignal);	// B LOW ACTIVE
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3,0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2,0);

			motorControl.LastPosition = State_C_B;

			break;

		default:
			break;

	}
}
