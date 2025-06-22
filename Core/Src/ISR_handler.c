
#include "ISR_handler.h"
#include "uart_manager.h"
#include "tcp_handler.h"
#include "stm32f1xx_hal.h"

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == W5500_INT_Pin) {
        W5500_InterruptHandler();
        return;
    }
}

