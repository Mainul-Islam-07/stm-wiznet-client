

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



  RemoteHost server = {
      .ip = {192, 168, 10, 45},
      .port = 8000
  }; //Mainul

static int8_t get_result;

const char *msg = "Hello";
int8_t GetSN_SR = 0;
uint8_t ir = 0;


static inline uint16_t htons(uint16_t x) {
    return (x << 8) | (x >> 8);
}

void init(void){
	global.debug_uart 	 = &huart3;
	global.comm_uart  	 = &huart2;

	  HAL_Delay(500);
	  startUartDMA(global.comm_uart);
	  HAL_Delay(100);
	  //Initialize Wiznet w5500 with GPIOs, SPI and NetInfo
	  W5500Init(&w5500_config,&netInfo);
	  HAL_Delay(100);
	  W5500_Init_Sockets();

}


void loop(void){
//	W5500_Handle_Events();
//	GetSN_SR = getSn_SR(0);
//	ir = getSn_IR(0);
//	HAL_Delay(10);
	if (get_result == 0){
		get_result = connect(0, server.ip, server.port);
		HAL_Delay(2000);
	}

//	GetSN_SR = getSn_SR(0);
//	HAL_Delay(10);
//	ir = getSn_IR(0);
//	HAL_Delay(10);
	if (get_result < 0 ){
		get_result = 0;
		disconnect(0);
		closesock(0);
		HAL_Delay(10);
		socket(0, Sn_MR_TCP, SERVER_PORT, 0);
		HAL_Delay(10);
	}
//	sock_status[sn] = getSn_SR(sn);
//	HAL_Delay(10);
	send(0, (uint8_t *)msg, strlen(msg));
//	HAL_Delay(10);


//	link = getPHYCFGR();
//	if (!(link & PHYCFGR_LNK_ON)) {
//		plink = 1;
//	}
//	for (uint8_t i = 1; i <= 254; i++) {
//	    uint8_t ip[4] = {192, 168, 20, i};
//	    socket(sn, Sn_MR_TCP, 5000 + i, 0);  // use varying local ports
//	    if (connect(sn, ip, htons(8000)) == SOCK_OK) {
//	        	plink = 1;
//	        disconnect(sn);
//	    }
//	    closesock(sn);
//	}



}








