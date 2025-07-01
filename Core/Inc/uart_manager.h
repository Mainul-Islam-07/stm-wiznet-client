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

#define MAX_UART_BUF 64
extern char uartRxBuffer[MAX_UART_BUF];

void startUartDMA(UART_HandleTypeDef *huart) ;
void uartTransmitDMA(UART_HandleTypeDef *huart, const char* data, size_t length);
#endif /* INC_UART_MANAGER_H_ */
