#include "app_main.h"
#include "tcp_handler.h"

GlobalParam global;

uint8_t physical_conn = 0;

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
      .mac = { 0x02, 0x12, 0x34, 0x56, 0x78, 0x02 },
      .ip = { 192, 168, 1, 101 },
      .sn = { 255, 255, 255, 0 },
      .gw = { 0, 0, 0, 0 },
      .dns = { 0, 0, 0, 0 },
      .dhcp = NETINFO_STATIC
  };

  RemoteHost server = {
      .ip = {192, 168, 1, 100},
      .port = 8000
  };




void init(void){
	global.debug_uart 	 = &huart3;
	global.comm_uart  	 = &huart2;
	global.dma_uart_rx   = &hdma_usart2_rx;
	global.dma_uart_tx   = &hdma_usart2_rx;

	  HAL_Delay(500);
	  startUartDMA(global.comm_uart);
//	  HAL_Delay(1000);
	  //Initialize Wiznet w5500 with GPIOs, SPI and NetInfo
	  W5500Init(&w5500_config,&netInfo);
	  HAL_Delay(1000);
	  W5500_Init_Sockets();

}


void loop(void){
//	HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
	W5500_Handle_Events();



	sock_status[Socket_0] = getSn_SR(Socket_0);
	physical_conn = getPHYCFGR() & 0x01;
	if((getPHYCFGR() & 0x01) == 0){
		HAL_Delay(5000);
		if((getPHYCFGR() & 0x01) == 0){
			W5500_Close_Socket();
		}
	}

	if ( HAL_GetTick() - Wireless_timeout > 5000 )
	{
		HAL_GPIO_WritePin(NETWORK_MODE_GPIO_Port, NETWORK_MODE_Pin, SET);
	}
	else
	{
		HAL_GPIO_WritePin(NETWORK_MODE_GPIO_Port, NETWORK_MODE_Pin, RESET);
	}

//	if(sock_status[Socket_0] == SOCK_STATUS_INIT ){
//			connect(Socket_0, server.ip, server.port);
//			HAL_Delay(1000);
//	}


}








