/*
 * uart_manager.h
 *
 *  Created on: Mar 9, 2025
 *      Author: User
 */

#ifndef INC_UART_MANAGER_H_
#define INC_UART_MANAGER_H_

#include "usart.h"
#include "app_main.h"
#include <string.h>
#include <stdbool.h>

#define UART_BUFFER_SIZE 30

extern char uartRxBuffer[UART_BUFFER_SIZE];

//extern uint8_t uartRxBuffer[UART_BUFFER_SIZE]; //NEW

void startUartDMA(UART_HandleTypeDef *huart) ;
void uartTransmitDMA(UART_HandleTypeDef *huart, const char* data);
#endif /* INC_UART_MANAGER_H_ */
