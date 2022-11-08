#include "motors.h"
#include "tim.h"

void motors_init(){
  HAL_TIM_Base_Start(&htim3);                                                                                                                                        
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
}

void motors_task(){
  // Set pwm width
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 75);
	// Set motor direction
	HAL_GPIO_WritePin(MOTOR_DIR_GPIO_Port, MOTOR_DIR_Pin, 1);
}
