
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
				printf("Connection in closing state, restarting...\r\n");
				disconnect(Socket_0);
				closesock(Socket_0);
				socket(Socket_0, Sn_MR_TCP, SERVER_PORT, 0);
				HAL_Delay(500);


}

void W5500_Init_Sockets(void) {
	if (socket(Socket_0, Sn_MR_TCP, SERVER_PORT, 0) == Socket_0) {
    	HAL_Delay(1000);
    	connect(Socket_0, server.ip, server.port);
    	HAL_Delay(2000);
	}
}


void handle_connection(uint8_t sn) {

	setSn_DIPR(sn, server.ip);     // Set destination IP
	setSn_DPORT(sn, server.port);  // Set destination port
	printf("Connected on socket %d\n", sn);
    memset(&w5500_event_flags[sn], 0, sizeof(W5500_EventFlags));
}


void handle_disconnection(uint8_t sn) {
    printf("Socket %d disconnected ", sn);
    memset(client_ip[sn], 0, 4); // Reset client IP on disconnection
    disconnect(sn);
    closesock(sn);
    socket(sn, Sn_MR_TCP, SERVER_PORT, 0);

    memset(&w5500_event_flags[sn], 0, sizeof(W5500_EventFlags));
}

void handle_received(uint8_t sn) {
	int32_t SN_RX_RSR_Size = getSn_RX_RSR(sn);
    if (SN_RX_RSR_Size > 0 && SN_RX_RSR_Size <= sizeof(recv_buf[sn])) {
    	int32_t tcp_recv_len = recv(sn, recv_buf[sn], SN_RX_RSR_Size);
        if (tcp_recv_len > 0) {
            recv_buf[sn][tcp_recv_len] = '\0';
            uartTransmitDMA(global.comm_uart, (char *)recv_buf[sn]);//NEW
        }
    }
    memset(&w5500_event_flags[sn], 0, sizeof(W5500_EventFlags));
}

void handle_timeout(uint8_t sn) {
    printf("Timeout on socket %d\n", sn);
    disconnect(sn);
    closesock(sn);
    socket(sn, Sn_MR_TCP, SERVER_PORT, 0);
    HAL_Delay(2000);


    memset(&w5500_event_flags[sn], 0, sizeof(W5500_EventFlags));
}

void handle_sent(uint8_t sn) {
    printf("Data sent on socket %d\n", sn);
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




int8_t SendToSocket(uint8_t sn, const char *msg, uint16_t len)
{
    if (getSn_SR(sn) == SOCK_ESTABLISHED) {
    	int32_t tcp_send_len = send(sn, (uint8_t *)msg, len); //NEW
        if (tcp_send_len > 0) {
            printf("Sent on socket %d: %s\n", sn, msg);
            return 0; // Success
        } else {
            printf("Failed to send on socket %d\n ", sn);
            return -2; // Send error
        }
    } else {
        printf("Socket %d not connected\n", sn);
        return -1; // Socket not established
    }
}


