

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
  }; //Mainul


const char *msg = "Hello";

int8_t get_result = 0;

void init(void){
	global.debug_uart 	 = &huart3;
	global.comm_uart  	 = &huart2;

	  HAL_Delay(500);
	  startUartDMA(global.comm_uart);
	  HAL_Delay(100);
	  //Initialize Wiznet w5500 with GPIOs, SPI and NetInfo
	  W5500Init(&w5500_config,&netInfo);
	  HAL_Delay(1000);
	  W5500_Init_Sockets();

}


void loop(void){
	W5500_Handle_Events();

//	if (get_result == 0){
//		get_result = connect(0, server.ip, server.port);
//		HAL_Delay(2000);
//	}
//
//	if (get_result < 0 ){
//		get_result = 0;
//		disconnect(0);
//		closesock(0);
//		HAL_Delay(10);
//		socket(0, Sn_MR_TCP, SERVER_PORT, 0);
//		HAL_Delay(10);
//	}

if(sock_status[Socket_0] == SOCK_STATUS_INIT ){
			HAL_Delay(2000);
	get_result = connect(Socket_0, server.ip, server.port);
			HAL_Delay(2000);
			}



 if (sock_status[Socket_0] == SOCK_STATUS_ESTABLISHED){
	send(Socket_0, (uint8_t *)msg, strlen(msg));
    HAL_Delay(500);
 }

}








