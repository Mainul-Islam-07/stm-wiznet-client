

#include "app_main.h"
#include "tcp_handler.h"

GlobalParam global;

W5500_GPIO_Config_t w5500_config = {
      .cs_port = W5500_CS_GPIO_Port,         // Replace with your CS GPIO port
      .cs_pin = W5500_CS_Pin,    // Replace with your CS GPIO pin
      .reset_port = W5500_RESET_GPIO_Port,      // Replace with your Reset GPIO port
      .reset_pin = W5500_RESET_Pin, // Replace with your Reset GPIO pin
      .int_port = W5500_INT_GPIO_Port,        // Replace with your Interrupt GPIO port
      .int_pin = W5500_INT_Pin,   // Replace with your Interrupt GPIO pin
      .spi_handle = &hspi1      // Replace with your SPI handle
  };

  wiz_NetInfo netInfo = {
      .mac = { 0x00, 0x08, 0xdc, 0xab, 0xcd, 0xef },
      .ip = { 192, 168, 20, 14 },
      .sn = { 255, 255, 255, 0 },
      .gw = { 192, 168, 20, 1 },
      .dns = { 8, 8, 8, 8 },
      .dhcp = NETINFO_STATIC
  };


void init(void){
	global.debug_uart 	 = &huart3;
	global.comm_uart  	 = &huart2;

	  HAL_Delay(500);
	  startUartDMA(global.comm_uart);

	  //Initialize Wiznet w5500 with GPIOs, SPI and NetInfo
	  W5500Init(&w5500_config,&netInfo);
	  W5500_Init_Sockets();

}


void loop(void){
	W5500_Handle_Events();
}








