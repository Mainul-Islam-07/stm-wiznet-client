
#ifndef INC_TCP_HANDLER_H_
#define INC_TCP_HANDLER_H_

#include "app_main.h" // NEW
#include "stm32f1xx_hal.h"
#include <stdint.h>
#include "socket.h"
#include "w5500.h"
#include "w5500_spi_handler.h"

#define MAX_SOCK_NUM 1
#define SERVER_PORT 8080


#define MAX_TCP_BUF 64

#define Socket_0 0

typedef struct {
    uint8_t socket;
    uint8_t connected;
    uint8_t disconnected;
    uint8_t received;
    uint8_t timeout;
    uint8_t sent;
} W5500_EventFlags;

typedef enum {
	SOCK_STATUS_CLOSED      = 0x00,
	SOCK_STATUS_INIT        = 0x13,
	SOCK_STATUS_LISTEN      = 0x14,
	SOCK_STATUS_ESTABLISHED = 0x17,
	SOCK_STATUS_CLOSE_WAIT  = 0x1C,
	SOCK_STATUS_FIN_WAIT    = 0x18,
	SOCK_STATUS_TIME_WAIT   = 0x1B,
	SOCK_STATUS_ERROR       = 0xFF
} sock_state_t;


extern W5500_EventFlags w5500_event_flags[];
extern uint8_t sn;
extern sock_state_t sock_status[MAX_SOCK_NUM];


//void W5500_Enable_Interrupts(void);
void W5500_Handle_Events(void);
void W5500_Init_Sockets(void);
void W5500_InterruptHandler(void);
void handle_connection(uint8_t sn);
void handle_disconnection(uint8_t sn);
void handle_received(uint8_t sn);
void handle_timeout(uint8_t sn);
void handle_sent(uint8_t sn);
int8_t SendToSocket(uint8_t sn, const char *msg, uint16_t len);
void W5500_Close_Socket(void);

#endif /* INC_TCP_HANDLER_H_ */
