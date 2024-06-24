
#include "UART.h"

void UART_Init()
{
  /****** GPIO configurations for UART communication. **************/

  /* Enable clocks for GPIOB and GPIOC. */

  RCC->AHB1ENR |= ( RCC_AHB1ENR_GPIOBEN_Msk | RCC_AHB1ENR_GPIOCEN_Msk );

  /* For UART, Tx pin is PB6 (UART1_TX) and Rx pin is PC7 (UART6_RX). */

  GPIOB->MODER |= ( 0x2 << ( 2 * 6 ) );
  GPIOC->MODER |= ( 0x2 << ( 2 * 7 ) );

  /* Transmit pin of UART1 is configured in very high-speed mode. */

  GPIOB->OSPEEDR |= ( 0x3UL << ( 2 * 6 ) );

  /* Configure alternate function registers. */

  GPIOB->AFR[0] |= ( 0x7UL << ( ( 6 - 0 ) << 2 ) );
  GPIOC->AFR[0] |= ( 0x8UL << ( ( 7 - 0 ) << 2 ) );

  /**********  UART1 initialization: Transmit. ***********/

  /*	Enable clock for USART1 module. */

  RCC->APB2ENR |= RCC_APB2ENR_USART1EN_Msk;

  /*	Enable USART1 module. */

  USART1->CR1 |= USART_CR1_UE_Msk;

  /*	Set number of stop bits to 1. */

  USART1->CR2 |= 0x0 << USART_CR2_STOP_Pos;

  /*	Set baud rate divider to 45 + 0/16 and oversampling by 16 for baud rate of 115200. */

  USART1->BRR 	|= 	( 45 << USART_BRR_DIV_Mantissa_Pos )
				|	( 0 << USART_BRR_DIV_Fraction_Pos );

  /* Baud rate generation for 921600. */

  //USART2->BRR 	|= 	( 2 << USART_BRR_DIV_Mantissa_Pos )
  //				|	( 14 << USART_BRR_DIV_Fraction_Pos );

  /*	Enable transmit DMA for UART1. */

  USART1->CR3 |= USART_CR3_DMAT_Msk;

  /*	Enable only transmission for USART1. */

  USART1->CR1 |= USART_CR1_TE_Msk;

  while( ( USART1->SR & USART_SR_TC_Msk ) == 0 );

  /**********  UART6 initialization: Receive. ***********/

  /*	Enable clock for USART6 module. */

  RCC->APB2ENR |= RCC_APB2ENR_USART6EN_Msk;

  /*	Enable USART6 module. */

  USART6->CR1 |= USART_CR1_UE_Msk;

  /*	Set number of stop bits to 1. */

  USART6->CR2 |= 0x0 << USART_CR2_STOP_Pos;

  /*	Set baud rate divider to 45 + 0/16 and oversampling by 16 for baud rate of 115200. */

  USART6->BRR 	|= 	( 45 << USART_BRR_DIV_Mantissa_Pos )
				|	( 0 << USART_BRR_DIV_Fraction_Pos );

  USART6->CR3 |= USART_CR3_DMAR_Msk;

  /*	Enable only transmission for USART1. */

  USART6->CR1 |= USART_CR1_RE_Msk;

  while( ( USART6->SR & USART_SR_TC_Msk ) == 0 );


  /****** Enable DMA transfers ******/

  UART_Receive_DMA_Enable( UART_RX_DMA_BUFFER, 6 );
}

/* Function to initialize UART DMA receive (UART6_RX). */

/* UART6_RX DMA: DMA controller 2, Stream 2, Channel 5. */

void UART_Receive_DMA_Enable(uint8_t* buffer, uint8_t nBytes)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN_Msk;

	/*	USART1 Receive stream is associated with DMA2
	 	stream 2, channel 5.
	 */

	/*	Disable the DMA stream. */

	DMA2_Stream2->CR &= ~DMA_SxCR_EN_Msk;

	/*	Wait until DMA stream disable request is acknowledged. */

	while( DMA2_Stream2->CR & DMA_SxCR_EN_Msk );

	/*	Set peripheral address register to the USART data register. */

	DMA2_Stream2->PAR = USART6_BASE + (uint32_t)0x4;

	/*	Set memory location to load the data into. */

	DMA2_Stream2->M0AR = (uint32_t)buffer;

	/*	Set number of data points per stream. */

	DMA2_Stream2->NDTR = nBytes;

	/*	Set DMA Stream channel. */

	DMA2_Stream2->CR |=  0x5U << DMA_SxCR_CHSEL_Pos;

	/*	Set DMA transfer direction from peripheral to memory. */

	DMA2_Stream2->CR |= 0x0U << DMA_SxCR_DIR_Pos;

	/*	Enable memory increment mode. */

	DMA2_Stream2->CR |= 0x1U << DMA_SxCR_MINC_Pos;

	/*	Enable circular mode. */

	DMA2_Stream2->CR |= 0x1U << DMA_SxCR_CIRC_Pos;

	/*	Enable DMA stream full transfer interrupt. */

	DMA2_Stream2->CR |=  DMA_SxCR_TCIE_Msk;

	/*	Enable DMA direct mode interrupt. */

	DMA2_Stream2->CR |= DMA_SxCR_DMEIE_Msk;

	/*	Enable DMA Half transfer interrupt. */

	//DMA1_Stream5->CR |= DMA_SxCR_HTIE_Msk;

	/*	Enable DMA Transfer Error interrupt. */

	//DMA1_Stream5->CR |= DMA_SxCR_TEIE_Msk;

	/*	Enable DMA Stream. */

	DMA2->LIFCR |= DMA_LIFCR_CTCIF2_Msk;

	DMA2_Stream2->CR |= DMA_SxCR_EN_Msk;

	while( ( DMA2_Stream2->CR & DMA_SxCR_EN_Msk ) == 0 );

	/*	Enable DMA1 Stream5 global interrupt. */

	NVIC_EnableIRQ(DMA2_Stream2_IRQn);
}


void UART_Receive_DMA_Disable()
{
  /*	Disable the DMA stream. */

  DMA2_Stream2->CR &= ~DMA_SxCR_EN_Msk;

  /*	Wait until DMA stream disable request is acknowledged. */

  while( DMA2_Stream2->CR & DMA_SxCR_EN_Msk );

  /* Clear all register contents. */

  DMA2_Stream2->CR = 0;
  DMA2_Stream2->FCR = 0;
  DMA2_Stream2->M0AR = 0;
  DMA2_Stream2->M1AR = 0;
  DMA2_Stream2->NDTR = 0;
  DMA2_Stream2->PAR = 0;

  /* Reset the DMA peripheral to its default state. */

  RCC->AHB1RSTR |= RCC_AHB1RSTR_DMA2RST_Msk;
}


void DMA2_Stream2_IRQHandler(void)
{
  if( DMA2->LISR & DMA_LISR_TCIF2 )
  {
    DMA2->LIFCR |= DMA_LIFCR_CTCIF2_Msk;
    SDCP_RXNQ(UART_RX_DMA_BUFFER);
  }
}

void UART_Polling_Transmit(uint8_t* buffer, uint8_t nBytes)
{
  uint8_t counter = 0;

  while(counter < nBytes)
  {
	while( ( USART1->SR & USART_SR_TXE_Msk ) == 0 );

	USART1->DR = buffer[counter];

    counter++;
  }

  USART1->SR &= ~USART_SR_TC_Msk;
}

void UART_DMA_Transmit(uint8_t* buffer, uint8_t nBytes)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN_Msk;

	/*	USART6 Receive stream is associated with DMA2
	 	stream 7, channel 4.
	 */

	/*	Disable the DMA stream. */

	DMA2_Stream7->CR &= ~DMA_SxCR_EN_Msk;

	/*	Wait until DMA stream disable request is acknowledged. */

	while( DMA2_Stream7->CR & DMA_SxCR_EN_Msk );

	/*	Set peripheral address register to the USART data register. */

	DMA2_Stream7->PAR = (uint32_t)(&(USART1->DR));

	/*	Set memory location to load the data into. */

	DMA2_Stream7->M0AR = (uint32_t)buffer;

	/*	Set number of data points per stream. */

	DMA2_Stream7->NDTR = nBytes;

	/*	Set DMA Stream channel. */

	DMA2_Stream7->CR |=  0x4U << DMA_SxCR_CHSEL_Pos;

	/*	Set DMA transfer direction: memory to peripheral. */

	DMA2_Stream7->CR |= 0x1U << DMA_SxCR_DIR_Pos;

	/*	Enable memory increment mode. */

	DMA2_Stream7->CR |= (0x1U << DMA_SxCR_MINC_Pos);

	/*	Disable circular mode. */

	DMA2_Stream7->CR |= (0x0U << DMA_SxCR_CIRC_Pos);

	/*	Enable DMA stream full transfer interrupt. */

	DMA2_Stream7->CR |=  DMA_SxCR_TCIE_Msk;

	/*	Enable DMA direct mode interrupt. */

	DMA2_Stream7->CR |= DMA_SxCR_DMEIE_Msk;

	/*	Enable DMA Stream. */

	//DMA1->HIFCR |= DMA_HIFCR_CTCIF6_Msk;

	DMA2_Stream7->CR |= DMA_SxCR_EN_Msk;

	while( !( DMA2_Stream7->CR & DMA_SxCR_EN_Msk ) );

	NVIC_EnableIRQ(DMA2_Stream7_IRQn);

	UART.UART_parameters.DMA_Tx_completed = 0;
}

void DMA2_Stream7_IRQHandler(void)
{
  if( DMA2->HISR & DMA_HISR_TCIF7 )
  {
    DMA2->HIFCR |= DMA_HIFCR_CTCIF7_Msk;
    UART.UART_parameters.DMA_Tx_completed = 1;
  }
}
