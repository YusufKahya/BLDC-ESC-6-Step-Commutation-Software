/*
 * SixStepCommutation.c
 *
 *  Created on: 17 Şub 2022
 *      Author: YusufKahya
 */

#include "main.h"
#include "SixStepCommutation.h"
#include "TimeTask.h"

extern volatile uint32_t *DWT_CYCCNT;
const uint8_t Trigger_Control_State[6] = {State_A_B,State_A_C,State_B_C,State_B_A,State_C_A,State_C_B};

void PeripheralsStart()
{
	  Motor_Control.Duty_Cycle = 25;

	  Start_Up.Duty_Cycle = 50;
	  Start_Up.Delay_Seconds = 0.00005f; // 50 mikro saniye (20k task'ta yapılabilecek minimum süre)
	  Start_Up.Tour = 5;

	  Start_Up.Align_Coefficient = 2;
	  Start_Up.Align_DutyCycle = 50;

	  Motor_Control.Drive_Stage = START_UP;

	  Motor_Control.Blinde_Mode_RPM = 20;

	  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);	// Phase A High
	  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);	// Phase B High
	  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);	// Phase C High
	  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);	// Phase A Low
	  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);	// Phase B Low
	  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);	// Phase C Low

	  HAL_COMP_Start(&hcomp2);	// Phase A Out
	  HAL_COMP_Start(&hcomp4);	// Phase B Out
	  HAL_COMP_Start(&hcomp6);	// Phase C Out

	  HAL_TIM_Base_Start_IT(&htim15);	// for Time Tasks
}

void Stop_Motor()
{
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,0);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2,0);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3,0);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1,0);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2,0);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3,0);

	Motor_Control.Drive_Stage = START_UP;
}

void Start_Up_Motor()
{
	static int Tour_Counter = 0;

	if(Motor_Control.Control_Counter++ >= MOTOR_CONTROL_TASK_HZ*Start_Up.Delay_Seconds)
	{
		Motor_Control.Control_Counter = 0;

		Set_Motor_State(Trigger_Control_State[Motor_Control.Motor_State_Index], Start_Up.Duty_Cycle);

		if( (Trigger_Control_State[Motor_Control.Motor_State_Index] == State_C_B) && Tour_Counter++ >= Start_Up.Tour)
		{
			Motor_Control.Drive_Stage = ALIGN;
			Tour_Counter = 0;
		}
		else
		{
			Motor_Control.Motor_State_Index = (Motor_Control.Motor_State_Index + 1) % 6;
		}
	}
}

void Align_Motor()
{
	static int Tour_Counter = 0;

	if(Motor_Control.Control_Counter++ >= MOTOR_CONTROL_TASK_HZ*Start_Up.Delay_Seconds)
	{
		Motor_Control.Control_Counter = 0;

		Set_Motor_State(Trigger_Control_State[Motor_Control.Motor_State_Index], Start_Up.Align_DutyCycle);

		if( (Trigger_Control_State[Motor_Control.Motor_State_Index] == State_C_B) && Tour_Counter++ >= Start_Up.Tour*Start_Up.Align_Coefficient)
		{
			Motor_Control.Drive_Stage = RUN;
			Tour_Counter = 0;
		}
		else
		{
			Motor_Control.Motor_State_Index = (Motor_Control.Motor_State_Index + 1) % 6;
		}
	}
}

void Run_Motor()
{

	  static uint8_t Next_State_Index = 0;

	  Motor_Control.A_Out = HAL_GPIO_ReadPin(COMP_A_OUT_GPIO_Port, COMP_A_OUT_Pin);
	  Motor_Control.B_Out = HAL_GPIO_ReadPin(COMP_B_OUT_GPIO_Port, COMP_B_OUT_Pin);
	  Motor_Control.C_Out = HAL_GPIO_ReadPin(COMP_C_OUT_GPIO_Port, COMP_C_OUT_Pin);

	  Motor_Control.Rotor_Position = (Motor_Control.C_Out << 2) + (Motor_Control.B_Out << 1) + (Motor_Control.A_Out);

	  Next_State_Index = (Motor_Control.Motor_State_Index + 1) % 6;

	  if(Motor_Control.Rotor_Position == Trigger_Control_State[Next_State_Index])
	  {
		  Motor_Control.Motor_State_Index = Next_State_Index;

		  Set_Motor_State(Trigger_Control_State[Motor_Control.Motor_State_Index], Motor_Control.Duty_Cycle);

		  if(Motor_Control.Rotor_Position == 1)
		  {
			  RpmFlag++;
		  }

	  }

}

void RPM_Calculate()
{
	  if(Motor_Control.Rotor_Position == 1)
	  {
		  Motor_Control.RPM = 432000000/(*DWT_CYCCNT);
		  *DWT_CYCCNT = 0;
//			  Motor_Control.RPM++;	// Buraya beklediğimizden hızlı giriyor		0,008 saniyede bir giriyo.
// 			  wait a minute 0,008 saniye 7200 rpm yapıyo yani normal. o zaman sıkıntı başka bir yerde, burda değil
	  }
}

void Blinde_Mode()
{
	  if(Motor_Control.Blinde_Mode_Counter++ >= MOTOR_CONTROL_TASK_HZ*(float)(1.0f/(Motor_Control.Blinde_Mode_RPM*6)))
	  {
		  Motor_Control.Blinde_Mode_Counter = 0;

		  Motor_Control.A_Out = HAL_GPIO_ReadPin(COMP_A_OUT_GPIO_Port, COMP_A_OUT_Pin);
		  Motor_Control.B_Out = HAL_GPIO_ReadPin(COMP_B_OUT_GPIO_Port, COMP_B_OUT_Pin);
		  Motor_Control.C_Out = HAL_GPIO_ReadPin(COMP_C_OUT_GPIO_Port, COMP_C_OUT_Pin);

		  Motor_Control.Rotor_Position = (Motor_Control.C_Out << 2) + (Motor_Control.B_Out << 1) + (Motor_Control.A_Out);

		  static int h = 0;
		  Set_Motor_State(Trigger_Control_State[h], Motor_Control.Duty_Cycle);
		  h = (h+1) % 6;

		  Motor_Control.Drive_Stage = START_UP;
	  }
}

void Set_Motor_State(uint8_t State, uint16_t DutyCycle)
{
	Motor_Control.Signal = (htim1.Init.Period+1)*DutyCycle/100;	// CCR
	Motor_Control.Max_Signal = htim1.Init.Period+1;				// ARR

	switch(State)
	{

		case State_A_B:

			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3,0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2,0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3,0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,Motor_Control.Signal);		// A HIGH ACTIVE
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2,Motor_Control.Max_Signal);	// B LOW AVTIVE

			Motor_Control.Pulse_Center = htim1.Instance->CCR1 / 2;

			break;

		case State_A_C:

			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2,0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2,0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3,0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,Motor_Control.Signal);		// A HIGH ACTIVE
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3,Motor_Control.Max_Signal);	// C LOW ACTIVE

			Motor_Control.Pulse_Center = htim1.Instance->CCR1 / 2;

			break;

		case State_B_C:

			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2,0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3,0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2,Motor_Control.Signal);		// B HIGH ACTIVE
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3,Motor_Control.Max_Signal);	// C LOW ACTIVE

			Motor_Control.Pulse_Center = htim1.Instance->CCR2 / 2;

			break;

		case State_B_A:

			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2,0);
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3,0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3,0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2,Motor_Control.Signal);	// B HIGH ACTIVE
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1,Motor_Control.Max_Signal);	// A LOW ACTIVE

			Motor_Control.Pulse_Center = htim1.Instance->CCR2 / 2;

			break;

		case State_C_A:

			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2,0);
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3,0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2,0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3,Motor_Control.Signal);	// C HIGH ACTIVE
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1,Motor_Control.Max_Signal);	// A LOW ACTIVE

			Motor_Control.Pulse_Center = htim1.Instance->CCR3 / 2;

			break;

		case State_C_B:

			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3,0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1,0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2,0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3,Motor_Control.Signal);	// C HIGH ACTIVE
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2,Motor_Control.Max_Signal);	// B LOW ACTIVE

			Motor_Control.Pulse_Center = htim1.Instance->CCR3 / 2;

			break;

		default:
			break;

	}
}
