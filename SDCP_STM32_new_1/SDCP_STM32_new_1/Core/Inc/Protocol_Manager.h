
#ifndef INC_PROTOCOL_MANAGER_H_
#define INC_PROTOCOL_MANAGER_H_

#include "main.h"
#include "UART.h"
#include "SDCP.h"
#include "SMFs.h"
#include "Application.h"
#include "LED.h"

typedef enum
{
	LOW = 0,
	HIGH = 1

}PM_SMF_Priority;

void PM_Init();
void PM_SMF_Tx_Handler();
void PM_SMF_Rx_Handler(uint8_t* buffer);
void PM_Rx_Handler();
void Protocol_Manager();
void PM_FTOR_Handler(uint8_t* buffer);
void PM_setMsgFramePriority(uint8_t frame_ID, PM_SMF_Priority priority);
void PM_TMC_Rx_Handler(uint8_t* buffer);
void PM_BTR_Rx_Handler(uint8_t* buffer);
PM_SMF_Priority PM_getMsgFramePriority(uint8_t frame_ID);

//static uint8_t verifyChecksum(uint8_t* buffer);
//static uint8_t obtainChecksum(uint8_t* buffer)

#endif /* INC_PROTOCOL_MANAGER_H_ */
