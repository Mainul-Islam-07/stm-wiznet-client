/*
 * ISR_handler.h
 *
 *  Created on: Apr 8, 2025
 *      Author: User
 */

#ifndef INC_ISR_HANDLER_H_
#define INC_ISR_HANDLER_H_

#include "stm32f1xx_hal.h"
#include <stdbool.h>

#define DEBOUNCE_DELAY 30

extern volatile bool oneCycleIgnore;

void handle_exti_pin(uint16_t pin, GPIO_TypeDef *port, IRQn_Type irq, void (*handler)(void));

#endif /* INC_ISR_HANDLER_H_ */
