
#include "tcp_handler.h"
#include "wizchip_conf.h"
#include "socket.h"
#include <string.h>
#include <stdio.h>
#include "main.h"
#include <inttypes.h>


W5500_EventFlags w5500_event_flags[MAX_SOCK_NUM] = {0};
sock_state_t sock_status[MAX_SOCK_NUM];

uint8_t recv_buf[MAX_SOCK_NUM][MAX_TCP_BUF] = {0};
uint8_t client_ip[MAX_SOCK_NUM][4] = {0};
uint32_t count_tcp_output = 0;
int32_t tcp_recv_len = 0;

int32_t SN_RX_RSR_Size = 0 ;



void W5500_Handle_Events(void)
{

	if (w5500_event_flags[Socket_0].connected)
		handle_connection(Socket_0);

	if (w5500_event_flags[Socket_0].disconnected)
		handle_disconnection(Socket_0);

	if (w5500_event_flags[Socket_0].received)
		handle_received(Socket_0);

	if (w5500_event_flags[Socket_0].timeout)
		handle_timeout(Socket_0);

	if (w5500_event_flags[Socket_0].sent)
		handle_sent(Socket_0);

	sock_status[Socket_0] = getSn_SR(Socket_0);


	if (sock_status[Socket_0] == SOCK_STATUS_CLOSE_WAIT || sock_status[Socket_0] == SOCK_STATUS_CLOSED){
		W5500_Close_Socket();
	}


}

void W5500_Close_Socket(void){
	//printf("Connection in closing state, restarting...\r\n");
	disconnect(Socket_0);
	closesock(Socket_0);
	socket(Socket_0, Sn_MR_UDP, SERVER_PORT, 0);
	sock_status[Socket_0] = getSn_SR(Socket_0);
	HAL_Delay(500);
}

void W5500_Init_Sockets(void) {
	if (socket(Socket_0, Sn_MR_UDP, SERVER_PORT, 0) == Socket_0) {
		setSn_MR(Socket_0, Sn_MR_UDP);  // Make sure it is in UDP mode
		HAL_Delay(1000);
		clear_receive_buffer(Socket_0);
	}
}


void handle_connection(uint8_t sn) {

	//printf("Connected on socket %d\n", sn);
	memset(&w5500_event_flags[sn], 0, sizeof(W5500_EventFlags));
}


void handle_disconnection(uint8_t sn) {
	//printf("Socket %d disconnected ", sn);
	disconnect(sn);
	closesock(sn);
	socket(sn, Sn_MR_UDP, SERVER_PORT, 0);
	memset(&w5500_event_flags[sn], 0, sizeof(W5500_EventFlags));
	HAL_Delay(2000);
}


void handle_received(uint8_t sn) {
	SN_RX_RSR_Size = getSn_RX_RSR(sn);
	tcp_recv_len = recvfrom(sn, (uint8_t *)recv_buf[sn], sizeof(recv_buf[sn]), client_ip[sn], &remotePort);
	count_tcp_output = count_tcp_output + tcp_recv_len;
	if (count_tcp_output > 100000){
		count_tcp_output = 0;
	}

	if (tcp_recv_len > 0) {
		recv_buf[sn][tcp_recv_len] = '\0';
		uartTransmitDMA(global.comm_uart, (char *)recv_buf[sn], tcp_recv_len);//NEW
	}
	memset(&w5500_event_flags[sn], 0, sizeof(W5500_EventFlags));
}

void handle_timeout(uint8_t sn) {
	//printf("Timeout on socket %d\n", sn);
	disconnect(sn);
	closesock(sn);
	socket(sn, Sn_MR_UDP, SERVER_PORT, 0);
	memset(&w5500_event_flags[sn], 0, sizeof(W5500_EventFlags));
	sock_status[sn] = getSn_SR(sn);
	HAL_Delay(2000);
}

void handle_sent(uint8_t sn) {
	//printf("Data sent on socket %d\n", sn);
	memset(&w5500_event_flags[sn], 0, sizeof(W5500_EventFlags));
}


void W5500_InterruptHandler(void) {
	uint8_t ir = getSn_IR(Socket_0);
	if (ir) {
		w5500_event_flags[Socket_0].socket = Socket_0;
		w5500_event_flags[Socket_0].connected = (ir & Sn_IR_CON) ? 1 : 0;
		w5500_event_flags[Socket_0].disconnected = (ir & Sn_IR_DISCON) ? 1 : 0;
		w5500_event_flags[Socket_0].received = (ir & Sn_IR_RECV) ? 1 : 0;
		w5500_event_flags[Socket_0].timeout = (ir & Sn_IR_TIMEOUT) ? 1 : 0;
		w5500_event_flags[Socket_0].sent = (ir & Sn_IR_SENDOK) ? 1 : 0;
		setSn_IR(Socket_0, ir);  // Clear all flags that are set
	}
}




void SendToSocket(uint8_t sn, const char *msg, uint16_t len)
{

}

void clear_receive_buffer(uint8_t sn) {
	int32_t SN_RX_RSR_Size = getSn_RX_RSR(sn);  // Get the size of the received data
	char temp_buff[64];
	// Loop to read and discard all data in the buffer
	while (SN_RX_RSR_Size > 0) {
		uint16_t remotePort = 0;
		  // Temporary buffer for discarding data
		// Read the available data into the temporary buffer (we don't need to process it, just discard it)
		recvfrom(sn, (uint8_t *)temp_buff, SN_RX_RSR_Size, client_ip[sn], &remotePort);
		// After reading, check for remaining data in the buffer
		SN_RX_RSR_Size = getSn_RX_RSR(sn);  // Get the remaining size of data
	}

	printf("Old data discarded successfully.\n");
}


