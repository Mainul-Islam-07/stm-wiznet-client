
#include "tcp_handler.h"
#include "wizchip_conf.h"
#include "socket.h"
#include <string.h>
#include <stdio.h>
#include "main.h"
#include <inttypes.h>


//uint8_t sock_status;
W5500_EventFlags w5500_event_flags[MAX_SOCK_NUM] = {0};
sock_state_t sock_status[MAX_SOCK_NUM];
uint8_t buf[32];

uint8_t recv_buf[MAX_SOCK_NUM][32] = {0};
uint8_t client_ip[MAX_SOCK_NUM][4] = {0};

uint8_t sn;


uint8_t socket_result;

void W5500_Handle_Events(void)
{

    for (sn = 0; sn < MAX_SOCK_NUM; sn++) {
        if (w5500_event_flags[sn].connected)
            handle_connection(sn);

        if (w5500_event_flags[sn].disconnected)
            handle_disconnection(sn);

        if (w5500_event_flags[sn].received)
            handle_received(sn);

        if (w5500_event_flags[sn].timeout)
            handle_timeout(sn);

        if (w5500_event_flags[sn].sent)
            handle_sent(sn);

        sock_status[sn] = getSn_SR(sn);


        if (sock_status[sn] == SOCK_STATUS_CLOSE_WAIT || sock_status[sn] == SOCK_STATUS_CLOSED){
                    W5500_Close_Socket();

            }
        }




}

void W5500_Close_Socket(void){
				printf("Connection in closing state, restarting...\r\n");
				disconnect(sn);
				closesock(sn);
//				listen(sn);


}

void W5500_Init_Sockets(void) {

    	socket_result = socket(Socket_0, Sn_MR_TCP, SERVER_PORT, 0);
    	HAL_Delay(500);
    	sock_status[Socket_0] = getSn_SR(Socket_0);
    	HAL_Delay(100);


}


void handle_connection(uint8_t sn) {
    // Wait for established state before reading DIPR
//    if (getSn_SR(sn) == SOCK_ESTABLISHED) {
//        getSn_DIPR(sn, client_ip[sn]);
//        printf("Socket %d connected from %d.%d.%d.%d", sn,
//               client_ip[sn][0], client_ip[sn][1], client_ip[sn][2], client_ip[sn][3]);
//    } else {
//        memset(client_ip[sn], 0, 4);
//        printf("Socket %d connection event, but not established ", sn);
//    }

    memset(&w5500_event_flags[sn], 0, sizeof(W5500_EventFlags));
}


void handle_disconnection(uint8_t sn) {
    printf("Socket %d disconnected ", sn);
    memset(client_ip[sn], 0, 4); // Reset client IP on disconnection
    disconnect(sn);
    closesock(sn);
    socket(sn, Sn_MR_TCP, SERVER_PORT, 0);
//    listen(sn);

    if (sock_status[sn] == SOCK_STATUS_INIT ){
   	connect(sn, server.ip, server.port);
   	HAL_Delay(2000);
   	}

    memset(&w5500_event_flags[sn], 0, sizeof(W5500_EventFlags));
}

void handle_received(uint8_t sn) {
    int32_t size = getSn_RX_RSR(sn);
    if (size > 0 && size < sizeof(recv_buf[sn])) {
        int32_t len = recv(sn, recv_buf[sn], size);
        if (len > 0) {
            recv_buf[sn][len] = '\0';
            uartTransmitDMA(global.comm_uart, recv_buf[sn]); //NEW
        }
    }
    memset(&w5500_event_flags[sn], 0, sizeof(W5500_EventFlags));
}

void handle_timeout(uint8_t sn) {
    printf("Timeout on socket %d\n", sn);
    disconnect(sn);
    closesock(sn);
    socket(sn, Sn_MR_TCP, SERVER_PORT, 0);
//    listen(sn);

    memset(&w5500_event_flags[sn], 0, sizeof(W5500_EventFlags));
}

void handle_sent(uint8_t sn) {
    printf("Data sent on socket %d\n", sn);

    memset(&w5500_event_flags[sn], 0, sizeof(W5500_EventFlags));
}


void W5500_InterruptHandler(void) {
	for (uint8_t sn = 0; sn < MAX_SOCK_NUM; sn++) {
		uint8_t ir = getSn_IR(sn);
		if (ir) {


			w5500_event_flags[sn].socket = sn;
			w5500_event_flags[sn].connected = (ir & Sn_IR_CON) ? 1 : 0;
			w5500_event_flags[sn].disconnected = (ir & Sn_IR_DISCON) ? 1 : 0;
			w5500_event_flags[sn].received = (ir & Sn_IR_RECV) ? 1 : 0;
			w5500_event_flags[sn].timeout = (ir & Sn_IR_TIMEOUT) ? 1 : 0;
			w5500_event_flags[sn].sent = (ir & Sn_IR_SENDOK) ? 1 : 0;

			setSn_IR(sn, ir);  // Clear all flags that are set
		}
	}
}

uint8_t socket_test[UART_BUFFER_SIZE];


int8_t SendToSocket(uint8_t sn, const char *msg, uint16_t len)
{
    if (getSn_SR(sn) == SOCK_ESTABLISHED) {
    	memcpy (socket_test, msg, UART_BUFFER_SIZE);
//    	int32_t result = send(sn, (uint8_t *)msg, strlen(msg));
    	int32_t result = send(sn, (uint8_t *)msg, len); //NEW
        if (result > 0) {
//            printf("Sent on socket %d: %s\n", sn, msg);
            return 0; // Success
        } else {
//            printf("Failed to send on socket %d\n ", sn);
            return -2; // Send error
        }
    } else {
//        printf("Socket %d not connected\n", sn);
        return -1; // Socket not established
    }
}


