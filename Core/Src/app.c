#include "app.h"
#include "stdint.h"
#include "motors.h"
#include "VL53L1X.h"
#include "main.h"

void app_init(){
  VL53L1X sensor;                                                                        
	TOF_InitStruct(&sensor, &hi2c1, 0x52, MOTOR_DIR_GPIO_Port, MOTOR_DIR_Pin);
	TOF_BootSensor(&sensor);
	uint16_t distance = TOF_GetDistance(&sensor);

  motors_init();
}

void app_task(){
  motors_task();
}
