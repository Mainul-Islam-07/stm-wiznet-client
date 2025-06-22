#include "uart_manager.h"
#include <stdio.h>
#include "tcp_handler.h" // NEW
#include "main.h"
#include "stdlib.h"



// UART DMA Buffer
char uartRxBuffer[UART_BUFFER_SIZE];
//uint8_t uartRxBuffer[UART_BUFFER_SIZE]; //NEW

// Start UART DMA Reception
void startUartDMA(UART_HandleTypeDef *huart) {
	HAL_UARTEx_ReceiveToIdle_DMA(huart, (uint8_t*)uartRxBuffer, UART_BUFFER_SIZE); //TO DO dynamic RX data

}

//// UART RX Complete Callback
//void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
//    if (huart->Instance == USART2) {
//
//
//
//
//
//    	SendToSocket(0, (const char *)uartRxBuffer); // NEW
//
////        memset(uartRxBuffer, 0, Size);  // Clear Buffer
//        startUartDMA(global.comm_uart); // Restart DMA Reception
//
//    }
//}

// UART RX Complete Callback with dynamic socket send
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart->Instance == USART2 && Size > 0 && Size <= UART_BUFFER_SIZE)
    {
        // 1. Allocate exact-size dynamic buffer for received data
        char* tempBuffer = (char*)malloc(Size + 1);  // +1 for null-terminator (if needed)
        if (tempBuffer != NULL)
        {
            // 2. Copy only the received bytes
            memcpy(tempBuffer, uartRxBuffer, Size);
            tempBuffer[Size] = '\0';  // Optional: null-terminate

            // 3. Send to socket
            SendToSocket(0, tempBuffer, Size);

            // 4. Free the buffer
            free(tempBuffer);
        }

        // 5. Restart DMA reception
        startUartDMA(global.comm_uart);
    }
}

void uartTransmitDMA(UART_HandleTypeDef *huart, const char* data){
	HAL_UART_Transmit_DMA(huart, (uint8_t*) data, strlen(data));
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    if (huart->ErrorCode & HAL_UART_ERROR_PE)
    {
        printf("UART Parity Error!\n");
        __HAL_UART_CLEAR_PEFLAG(huart);
    }
    if (huart->ErrorCode & HAL_UART_ERROR_NE)
    {
        printf("UART Noise Error!\n");
        __HAL_UART_CLEAR_NEFLAG(huart);
    }
    if (huart->ErrorCode & HAL_UART_ERROR_FE)
    {
        printf("UART Framing Error!\n");
        __HAL_UART_CLEAR_FEFLAG(huart);
    }
    if (huart->ErrorCode & HAL_UART_ERROR_ORE)
    {
        printf("UART Overrun Error!\n");
        __HAL_UART_CLEAR_OREFLAG(huart);
    }

    // Abort and restart DMA
    HAL_UART_Abort(huart);
    startUartDMA(huart); // Restart your UART DMA
}






