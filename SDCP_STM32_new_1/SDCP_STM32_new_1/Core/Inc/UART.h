
#ifndef INC_UART_H_
#define INC_UART_H_

#include "main.h"

/* UART6 is configured in Rx mode only. */
/* UART1 is configured in Tx mode only. */

#define MAX_RX_DMA_BUFFER_SZ	6

union UART
{
  uint32_t all;

  struct UART_parameters
  {

	uint32_t Rx_DMA_enabled : 1;
    uint32_t Tx_DMA_enabled : 1;
    uint32_t DMA_Rx_completed : 1;
    uint32_t DMA_Tx_completed : 1;

  }UART_parameters;

}UART;

uint8_t UART_RX_DMA_BUFFER[MAX_RX_DMA_BUFFER_SZ];

void UART_Init();

void UART_Receive_DMA_Enable(uint8_t* buffer, uint8_t nBytes);
void UART_Receive_DMA_Disable();

void UART_DMA_Transmit(uint8_t* buffer, uint8_t nBytes);
void UART_Polling_Transmit(uint8_t* buffer, uint8_t nBytes);

#endif /* INC_UART_H_ */
