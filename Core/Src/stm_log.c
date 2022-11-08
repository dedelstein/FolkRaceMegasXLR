#include "stm_log.h"
#include "stdint.h"
#include "string.h"
#include "stdio.h"
#include "usart.h"

void stm_log(char* text){
  char str_buf [200];
	uint8_t buf_len = snprintf(str_buf, 200, text, 10);
	HAL_UART_Transmit(&huart2, (uint8_t*)str_buf, buf_len, 100);
}
