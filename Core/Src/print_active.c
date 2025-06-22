/*
 * print_active.c
 *
 *  Created on: Mar 9, 2025
 *      Author: User
 */

#include "print_active.h"
#include "app_main.h"

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#define GETCHAR_PROTOTYPE int __io_getchar(void)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#endif

PUTCHAR_PROTOTYPE
{
  HAL_UART_Transmit(global.debug_uart, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
  return ch;
}

GETCHAR_PROTOTYPE
{
  uint8_t ch = 0;

  /* Clear the Overrun flag just before receiving the first character */
  __HAL_UART_CLEAR_OREFLAG(global.debug_uart);

  /* Wait for reception of a character on the USART RX line and echo this
   * character on console */
  HAL_UART_Receive(global.debug_uart, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
  HAL_UART_Transmit(global.debug_uart, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
  return ch;
}

