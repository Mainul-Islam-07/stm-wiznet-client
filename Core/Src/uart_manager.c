#include "uart_manager.h"
#include <stdio.h>
#include "tcp_handler.h" // NEW
#include "main.h"
#include "stdlib.h"
#include <stddef.h>


char uartRxBuffer [MAX_UART_BUF];
uint32_t count_uart_input = 0;
uint32_t count_uart_output = 0;

// Start UART DMA Reception
void startUartDMA(UART_HandleTypeDef *huart) {
	HAL_UARTEx_ReceiveToIdle_DMA(huart, (uint8_t*)uartRxBuffer, MAX_UART_BUF); //TO DO dynamic RX data

}



void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	count_uart_input = count_uart_input + Size;

	if (huart->Instance == USART2 && Size > 0 && Size <= MAX_UART_BUF)
	{
		uartRxBuffer[Size] = '\0';
		SendToSocket(Socket_0, uartRxBuffer, Size+1);
		startUartDMA(global.comm_uart);
	}
}




void uartTransmitDMA(UART_HandleTypeDef *huart, const char* data, size_t length ){

	count_uart_output = count_uart_output + length;
	if (count_uart_output > 100000){
		count_uart_output = 0;
	}

	HAL_UART_Transmit_DMA(huart, (uint8_t*) data, length-1);
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	if (huart->ErrorCode & HAL_UART_ERROR_PE)
	{
		//printf("UART Parity Error!\n");
		__HAL_UART_CLEAR_PEFLAG(huart);
	}
	if (huart->ErrorCode & HAL_UART_ERROR_NE)
	{
		//printf("UART Noise Error!\n");
		__HAL_UART_CLEAR_NEFLAG(huart);
	}
	if (huart->ErrorCode & HAL_UART_ERROR_FE)
	{
		//printf("UART Framing Error!\n");
		__HAL_UART_CLEAR_FEFLAG(huart);
	}
	if (huart->ErrorCode & HAL_UART_ERROR_ORE)
	{
		//printf("UART Overrun Error!\n");
		__HAL_UART_CLEAR_OREFLAG(huart);
	}

	// Abort and restart DMA
	HAL_UART_Abort(huart);
	startUartDMA(huart); // Restart your UART DMA
}






