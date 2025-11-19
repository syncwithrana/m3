/* comms_drv.h
 *
 * Public COMMS driver API (renamed from uart_drv.h). Provides simple
 * initialization, transmit and receive helpers used by tests.
 */

#include <stdint.h>
#ifndef __COMMS_DRV_H__
#define __COMMS_DRV_H__

typedef enum{
    COMMS_OK = 0,
    COMMS_NO_DATA,
    COMMS_RX_ERR,
} comms_status;

#define COMMSLCRH_EXAMPLE    0x00000060u
#define COMMSLCRH_FEN        0x00000010

#define COMMSCTL_UARTEN      0x00000001u

#define COMMSFR_TXFF         0x00000020u
#define COMMSFR_RXFE         0x00000010u
#define COMMSFR_BUSY         0x00000008u

#define COMMSDR_DATA_MASK    0x000000FFu
#define COMMSRSRECR_ERR_MASK 0x0000000Fu

#define COMMSIFLS_TX_1_8     0x00000000 
#define COMMSIFLS_RX_1_8     0x00000000

#define COMMS_RX_IRQ         0x00000010u
#define COMMS_RT_IRQ         0x00000040u

#define COMMS_BAUD_9600      9600u
#define COMMS_BAUD_19200     19200u
#define COMMS_BAUD_38400     38400u
#define COMMS_BAUD_57600     57600u
#define COMMS_BAUD_115200    115200u

void comms_init(uint32_t baudrate);
void comms_send_byte(uint8_t byte);
comms_status comms_recv_byte(uint8_t* byte);

#endif /* __COMMS_DRV_H__ */
