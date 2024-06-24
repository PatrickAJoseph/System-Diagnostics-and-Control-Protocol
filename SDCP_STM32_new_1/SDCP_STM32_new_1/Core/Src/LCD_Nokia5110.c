
#include "LCD_Nokia5110.h"

void Nokia5110_clear()
{
  Nokia5110.x_pos = 0;
  Nokia5110.y_pos = 0;

  for(int i = 0 ; i < 504; i++)
  {
    Nokia5110.byte[i] = 0x00;
  }
}

static void Nokia5110_SysTick_delay_ms(uint16_t delay_ms)
{
	while( delay_ms )
	{
		/*	Disable SysTick timer. */

		SysTick->CTRL = 0;

		/*	Load value with 0. */

		SysTick->VAL = 0;

		/*	Load upper limit of SysTick timer. */

		SysTick->LOAD = (84000000UL / 1000UL) - 1;

		/*	Enable SysTick timer and select clock source as processor clock. */

		SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk;

		/*	Wait until SysTick timer reaches limit. */

		while( ( SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk ) == 0 );

		/*	Decrement counter variable. */

		delay_ms--;
	}
}



void Nokia5110_GPIO_Init()
{
	RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOAEN_Msk
			     |  RCC_AHB1ENR_GPIOBEN_Msk
				 |  RCC_AHB1ENR_GPIOCEN_Msk);

	GPIOA->MODER 	|= 		( 0x1 << ( 2 * 5 ) );	//	PA5: Test LED.

	GPIOB->MODER 	|=		( 0x1 << ( 2 * 2 ) );	//	PB2: LCD Reset.

	GPIOB->MODER 	|=		( 0x2 << ( 2 * 12 ) );	//	PB12: LCD CS.
	GPIOB->MODER 	|=		( 0x2 << ( 2 * 13 ) );	//	PB13: LCD Clock.
	GPIOB->MODER 	|=		( 0x2 << ( 2 * 15 ) );	//	PB15: LCD Data.

	GPIOB->MODER 	|=		( 0x1 << ( 2 * 14 ) );	//	PB14: LCD Data/Command.

	/* Outputs configured as very high speed.*/

	GPIOB->OSPEEDR |= ( 0x3 << ( 2 * 2 ) );		//	PB2:	LCD Reset.
	GPIOB->OSPEEDR |= ( 0x3 << ( 2 *12 ) );		//	PB12:	LCD CS.
	GPIOB->OSPEEDR |= ( 0x3 << ( 2 * 13 ) );	//	PB13:	LCD Clock.
	GPIOB->OSPEEDR |= ( 0x3 << ( 2 * 14 ) );	//	PB14:	LCD Data/command.
	GPIOB->OSPEEDR |= ( 0x3 << ( 2 * 15 ) );	//	PB15:	LCD Data.

	/* Use for hardware SPI. */

	GPIOB->AFR[1] |= ( 0x5UL << ( ( 12 - 8 ) * 4 ) );
	GPIOB->AFR[1] |= ( 0x5UL << ( ( 13 - 8 ) * 4 ) );
	GPIOB->AFR[1] |= ( 0x5UL << ( ( 15 - 8 ) * 4 ) );

	Nokia5110.pending = 0;
}

/* Function to initialize SPI interface of MCU. */

void Nokia5110_SPI_Init(){

	RCC->APB1ENR |= RCC_APB1ENR_SPI2EN_Msk;

	/****	Initialize in SPI Master Mode. ****/

	/*	Set baud rate to F_PCLK / 256. */

	SPI2->CR1 = 0x5 << SPI_CR1_BR_Pos;

	/*	Set CPOL = 0 and CPHA = 0. */

	SPI2->CR1 &= ~( SPI_CR1_CPOL_Msk | SPI_CR1_CPHA_Msk );

	//SPI2->CR1 |= ( SPI_CR1_CPOL_Msk );

	/*	Set data frame length to 8 bits. */

	SPI2->CR1 &= ~SPI_CR1_DFF_Msk;

	/*	Select MSB first mode. */

	SPI2->CR1 &= ~SPI_CR1_LSBFIRST_Msk;

	/* Set SPI in Motorola mode. */

	SPI2->CR2 &= ~SPI_CR2_FRF_Msk;

	/*	Set SPI to master mode. */

	SPI2->CR1 |= SPI_CR1_MSTR_Msk;

	/* Set SPI mode to unidirectional mode. */

	SPI2->CR1 &= ~SPI_CR1_BIDIMODE_Msk;

	SPI2->CR2 |= SPI_CR2_SSOE;	//	Enable NSS output from SPI peripheral.

	SPI2->CR1 &= ~SPI_CR1_SSM; //Software slave management disabled.

	/* Enable SPI Module. */

	SPI2->CR1 |= SPI_CR1_SPE_Msk;
}

void Nokia5110_SPI_TxP(uint8_t byte){

  while( ( SPI2->SR & SPI_SR_TXE ) == 0 );

  *((volatile uint8_t*) &(SPI2->DR)) = byte;

  while( ( SPI2->SR & SPI_SR_TXE ) == 0 );

  while( ( SPI2->SR & SPI_SR_BSY ) );

  (void)SPI2->DR;
  (void)SPI2->SR;
}

void Nokia5110_SPI_DMA_TransferByte(uint8_t byte,uint8_t type){

  /* If byte type is data (1), set Data/nCommand signal, else if byte type is
   * command(0), clear Data/nCommand signal.
   * */

  type ? ( GPIOC->ODR |= ( 1 << 8 ) ) : ( GPIOC->ODR &= ~( 1 << 8 ) );

  if(!(RCC->AHB1ENR & RCC_AHB1ENR_DMA1EN_Msk)){
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN_Msk;
  }

  /*  Disable the DMA stream. */

  DMA1_Stream4->CR &= ~DMA_SxCR_EN_Msk;

  /*  Wait until DMA stream disable request is acknowledged. */

  while( DMA1_Stream4->CR & DMA_SxCR_EN_Msk );

  /*  Set peripheral address register to the USART data register. */

  DMA1_Stream4->PAR = (uint32_t)((volatile uint8_t*)(&(SPI2->DR)));

  /*  Set memory location to load the data into. */

  DMA1_Stream4->M0AR = (uint32_t)&byte;

  /*  Set number of data points per stream. */

  DMA1_Stream4->NDTR = 1;

  /*  Set DMA Stream channel. */

  DMA1_Stream4->CR |=  0x0U << DMA_SxCR_CHSEL_Pos;

  /*  Set DMA transfer direction: peripheral to memory. */

  DMA1_Stream4->CR |= 0x1U << DMA_SxCR_DIR_Pos;

  /*  Enable memory increment mode. */

  DMA1_Stream4->CR |= 0x0U << DMA_SxCR_MINC_Pos;

  /*  Disable circular mode. */

  DMA1_Stream4->CR |= 0x0U << DMA_SxCR_CIRC_Pos;

  /*  Enable DMA stream full transfer interrupt. */

  DMA1_Stream4->CR |=  DMA_SxCR_TCIE_Msk;

  /*	Enable DMA direct mode interrupt. */

  DMA1_Stream4->CR |= DMA_SxCR_DMEIE_Msk;

  /*	Enable DMA Stream. */

  DMA1->HIFCR |= DMA_HIFCR_CTCIF4_Msk;

  DMA1_Stream4->CR |= DMA_SxCR_EN_Msk;

  while( !( DMA1_Stream4->CR & DMA_SxCR_EN_Msk ) );

  /* Enable DMA transfer for SPI Peripheral. */

  SPI2->CR2 |= SPI_CR2_TXDMAEN_Msk;

  /* Enable DMA1_Stream4 Interrupt. */

  NVIC_EnableIRQ(DMA1_Stream4_IRQn);
}


/* SPI DMA Full transfer callback. */

void DMA1_Stream4_IRQHandler(void){

  if(DMA1->HISR & DMA_HISR_TCIF4){

	/* Set display updated flag. */

	Nokia5110.updated = 1;
	Nokia5110.pending = 0;

	/* Clear SPI DMA Transmit mask. */

    SPI2->CR2 &= ~SPI_CR2_TXDMAEN_Msk;

    /* Read Data Register and Status register to clear over-run bit. */

    (void)SPI2->DR;
    (void)SPI2->SR;

    DMA1->HIFCR |= DMA_HIFCR_CTCIF4_Msk;
  }
}

/* Function to write a byte of data into the LCD. */

void Nokia5110_writeByte(uint8_t byte, uint8_t type){

  (type) ? ( GPIOB->ODR |= ( 1 << 14 ) ) : ( GPIOB->ODR &= ~( 1 << 14 ) );
  //Nokia5110_SPI_TxP(byte);
  Nokia5110_SPI_DMA_TransferByte(byte,type);
  Nokia5110_SysTick_delay_ms(1);
}


void Nokia5110_updateDisplay(){

  GPIOB->ODR |= ( 1 << 14 );

  if(!(RCC->AHB1ENR & RCC_AHB1ENR_DMA1EN_Msk)){
      RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN_Msk;
  }

  /* Clear display update flag. */

  Nokia5110.updated = 0;
  Nokia5110.pending = 1;

  /*  Disable the DMA stream. */

  DMA1_Stream4->CR &= ~DMA_SxCR_EN_Msk;

  /*  Wait until DMA stream disable request is acknowledged. */

  while( DMA1_Stream4->CR & DMA_SxCR_EN_Msk );

  /*  Set peripheral address register to the SPI2 data register. */

  DMA1_Stream4->PAR = (uint32_t)((volatile uint8_t*)(&(SPI2->DR)));

  /*  Set memory location to load the data into. */

  DMA1_Stream4->M0AR = (uint32_t)Nokia5110.byte;

  /*  Set number of data points per stream. */

  DMA1_Stream4->NDTR = 504;

  /*  Set DMA Stream channel. */

  DMA1_Stream4->CR |=  0x0U << DMA_SxCR_CHSEL_Pos;

  /*  Set DMA transfer direction: peripheral to memory. */

  DMA1_Stream4->CR |= 0x1U << DMA_SxCR_DIR_Pos;

  /*  Enable memory increment mode. */

  DMA1_Stream4->CR |= 0x1U << DMA_SxCR_MINC_Pos;

  /*  Disable circular mode. */

  DMA1_Stream4->CR |= 0x0U << DMA_SxCR_CIRC_Pos;

  /*  Enable DMA stream full transfer interrupt. */

  DMA1_Stream4->CR |=  DMA_SxCR_TCIE_Msk;

  /*  Enable DMA direct mode interrupt. */

  DMA1_Stream4->CR |= DMA_SxCR_DMEIE_Msk;

  /*  Enable DMA Stream. */

  DMA1->HIFCR |= DMA_HIFCR_CTCIF4_Msk;

  DMA1_Stream4->CR |= DMA_SxCR_EN_Msk;

  while( !( DMA1_Stream4->CR & DMA_SxCR_EN_Msk ) );

  /* Enable DMA transfer for SPI Peripheral. */

  SPI2->CR2 |= SPI_CR2_TXDMAEN_Msk;

  /* Enable DMA1_Stream4 Interrupt. */

  NVIC_EnableIRQ(DMA1_Stream4_IRQn);
  NVIC_SetPriority(DMA1_Stream4_IRQn,6);
}

void Nokia5110_Init(){

  Nokia5110_GPIO_Init();

  /* Send reset signal. */

  Nokia5110_SysTick_delay_ms(150);
  GPIOB->ODR &= ~( 1 << 2 );
  Nokia5110_SysTick_delay_ms(200);
  GPIOB->ODR |= ( 1 << 2 );
  Nokia5110_SysTick_delay_ms(200);

  Nokia5110_SPI_Init();


  for(int i = 0 ; i < 15; i++){
    Nokia5110_writeByte(0x00,0);
  }

  Nokia5110_writeByte(0x21,0);
  Nokia5110_writeByte(0xB5,0);
  Nokia5110_writeByte(0x04,0);
  Nokia5110_writeByte(0x14,0);
  Nokia5110_writeByte(0x20,0);
  Nokia5110_writeByte(0x0C,0);

  Nokia5110_SysTick_delay_ms(200);

  Nokia5110.x_pos = 0;
  Nokia5110.y_pos = 0;
}


void Nokia5110_setPosition(uint8_t x_position, uint8_t y_position){

  /* Entry mode: standard instruction set. */

  Nokia5110_writeByte(0x20,0);

  /* X position for the Nokia 5110 display varies from 0 to 83. */

  Nokia5110_writeByte(0x80 + (x_position%83), 0 );

  /* Y position for the Nokia 5110 display varies from 0 to 47
   * for bit-wise addressing along y-axis. */

  Nokia5110_writeByte(0x40 + (y_position%48), 0 );
}

void Nokia5110_printASCII(char character){

  uint8_t buffer[6];

  buffer[5] = 0;

  CBM_ascii5x8( (uint8_t)character, buffer );

  if( !(Nokia5110.y_pos%8) ){

  Nokia5110.byte[ Nokia5110.x_pos + 0 + 84 * (Nokia5110.y_pos>>3) ] = buffer[0];
  Nokia5110.byte[ Nokia5110.x_pos + 1 + 84 * (Nokia5110.y_pos>>3) ] = buffer[1];
  Nokia5110.byte[ Nokia5110.x_pos + 2 + 84 * (Nokia5110.y_pos>>3) ] = buffer[2];
  Nokia5110.byte[ Nokia5110.x_pos + 3 + 84 * (Nokia5110.y_pos>>3) ] = buffer[3];
  Nokia5110.byte[ Nokia5110.x_pos + 4 + 84 * (Nokia5110.y_pos>>3) ] = buffer[4];
  Nokia5110.byte[ Nokia5110.x_pos + 5 + 84 * (Nokia5110.y_pos>>3) ] = buffer[5];

  }
  else{

  uint8_t Pos = (Nokia5110.y_pos%8);
  uint8_t counter = 0;
  uint8_t mskH = ( ( (0xFF & (( 1 << (8-Pos) ) - 1) ) ) << Pos );
  uint8_t mskL = (( 0xFF >> (8-Pos) ) & (( 1 << Pos ) - 1));

  while(counter<6){

	Nokia5110.byte[ (Nokia5110.x_pos + counter) + ( 84 * ((Nokia5110.y_pos>>3) + 0) ) ] &= ~mskH;
    Nokia5110.byte[ (Nokia5110.x_pos + counter) + ( 84 * ((Nokia5110.y_pos>>3) + 0) ) ]
				  |= ( ( (buffer[counter] & (( 1 << (8-Pos) ) - 1) ) ) << Pos );
    Nokia5110.byte[ (Nokia5110.x_pos + counter) + ( 84 * ((Nokia5110.y_pos>>3) + 1) ) ] &= ~mskL;
    Nokia5110.byte[ (Nokia5110.x_pos + counter) + ( 84 * ((Nokia5110.y_pos>>3) + 1) ) ]
  				  |= (( buffer[counter] >> (8-Pos) ) & (( 1 << Pos ) - 1));
    counter++;
  }

  }

  Nokia5110.x_pos += 6;

  if( Nokia5110.x_pos >= 84 ){
    Nokia5110.x_pos = 0;
	Nokia5110.y_pos++;
  }

  Nokia5110.y_pos = (Nokia5110.y_pos % 48);
}

void Nokia5110_printString(char* str){

  while(*str != '\0'){
    Nokia5110_printASCII((char)(*str));
    str++;
  }
}

static uint32_t U8_previous = 0;

void Nokia5110_printU8(uint8_t n)
{
  if(n == 0){

	uint8_t counter = 0;

	while(U8_previous){
      counter++;
      U8_previous /= 10;
	}

    Nokia5110_printASCII('0');

    while(counter){
      Nokia5110_printASCII(' ');
      counter--;
    }

    U8_previous = 0;

    return;
  }

  uint32_t tempU8 = n;
  uint8_t buffer[4];
  uint8_t opBuffer[4];

  U8_previous = n;

  buffer[2] = (uint8_t)'0' + (uint8_t)(tempU8 % 10);
  tempU8 /= 10;
  buffer[1] = (uint8_t)'0' + (uint8_t)(tempU8 % 10);
  tempU8 /= 10;
  buffer[0] = (uint8_t)'0' + (uint8_t)(tempU8 % 10);

  buffer[3] = '\0';

  uint8_t startIndex = 0;

  while( buffer[startIndex] == (uint8_t)'0' ){
    startIndex++;
  }

  uint8_t counter = 0;

  while( startIndex != 4 ){
	opBuffer[counter] = buffer[startIndex];
	counter++;
    startIndex++;
  }

  opBuffer[counter] = (uint8_t)'\0';

  Nokia5110_printString((char*)opBuffer);
  Nokia5110_printASCII(' ');
}

static uint16_t U16_previous = 0;

void Nokia5110_printU16(uint16_t n)
{

  if(n == 0){

    uint8_t counter = 0;

    while(U16_previous){
      counter++;
	  U16_previous /= 10;
	}

	Nokia5110_printASCII('0');

	while(counter){
      Nokia5110_printASCII(' ');
	  counter--;
	}

	U16_previous = 0;

	return;
  }

  uint32_t tempU16 = n;
  uint8_t buffer[6];
  uint8_t opBuffer[6];

  buffer[4] = (uint8_t)'0' + (uint8_t)(tempU16 % 10);
  tempU16 /= 10;
  buffer[3] = (uint8_t)'0' + (uint8_t)(tempU16 % 10);
  tempU16 /= 10;
  buffer[2] = (uint8_t)'0' + (uint8_t)(tempU16 % 10);
  tempU16 /= 10;
  buffer[1] = (uint8_t)'0' + (uint8_t)(tempU16 % 10);
  tempU16 /= 10;
  buffer[0] = (uint8_t)'0' + (uint8_t)(tempU16 % 10);

  buffer[5] = '\0';

  uint8_t startIndex = 0;

  while( buffer[startIndex] == (uint8_t)'0' ){
    startIndex++;
  }

  uint8_t counter = 0;

  while( startIndex != 6 ){
	opBuffer[counter] = buffer[startIndex];
	counter++;
    startIndex++;
  }

  opBuffer[counter] = (uint8_t)'\0';

  Nokia5110_printString((char*)opBuffer);
  Nokia5110_printASCII(' ');
}

static uint32_t U32_previous = 0;

void Nokia5110_printU32(uint32_t n)
{

  if(n == 0){

    uint8_t counter = 0;

    while(U32_previous){
	  counter++;
	  U32_previous /= 10;
    }

	Nokia5110_printASCII('0');

    while(counter){
      Nokia5110_printASCII(' ');
      counter--;
    }

    U32_previous = 0;

    return;
  }

  uint32_t tempU32 = n;
  uint8_t buffer[11];
  uint8_t opBuffer[11];

  buffer[9] = (uint8_t)'0' + (uint8_t)(tempU32 % 10);
  tempU32 /= 10;
  buffer[8] = (uint8_t)'0' + (uint8_t)(tempU32 % 10);
  tempU32 /= 10;
  buffer[7] = (uint8_t)'0' + (uint8_t)(tempU32 % 10);
  tempU32 /= 10;
  buffer[6] = (uint8_t)'0' + (uint8_t)(tempU32 % 10);
  tempU32 /= 10;
  buffer[5] = (uint8_t)'0' + (uint8_t)(tempU32 % 10);
  tempU32 /= 10;
  buffer[4] = (uint8_t)'0' + (uint8_t)(tempU32 % 10);
  tempU32 /= 10;
  buffer[3] = (uint8_t)'0' + (uint8_t)(tempU32 % 10);
  tempU32 /= 10;
  buffer[2] = (uint8_t)'0' + (uint8_t)(tempU32 % 10);
  tempU32 /= 10;
  buffer[1] = (uint8_t)'0' + (uint8_t)(tempU32 % 10);
  tempU32 /= 10;
  buffer[0] = (uint8_t)'0' + (uint8_t)(tempU32 % 10);

  buffer[10] = '\0';

  uint8_t startIndex = 0;

  while( buffer[startIndex] == (uint8_t)'0' ){
    startIndex++;
  }

  uint8_t counter = 0;

  while( startIndex != 11 ){
	opBuffer[counter] = buffer[startIndex];
	counter++;
    startIndex++;
  }

  opBuffer[counter] = (uint8_t)'\0';

  Nokia5110_printString((char*)opBuffer);
  Nokia5110_printASCII(' ');
}

void Nokia5110_printU32_NB(uint32_t n,uint8_t d)
{
  uint32_t tempU32 = n;
  uint8_t buffer[11];

  if(d!=0){

  for( int i = 0 ; i < d ; i++ )
  {
	buffer[d-1-i] = (uint32_t)'0' + (uint8_t)(tempU32%10);
	tempU32 /= 10;
  }

  buffer[d] = (uint8_t)'\0';

  Nokia5110_printString((char*)buffer);

  return;
  }


  if(n == 0){

    uint8_t counter = 0;

    while(U32_previous){
	  counter++;
	  U32_previous /= 10;
    }

	Nokia5110_printASCII('0');

    while(counter){
      Nokia5110_printASCII(' ');
      counter--;
    }

    U32_previous = 0;

    return;
  }

  tempU32 = n;
  uint8_t opBuffer[11];

  buffer[9] = (uint8_t)'0' + (uint8_t)(tempU32 % 10);
  tempU32 /= 10;
  buffer[8] = (uint8_t)'0' + (uint8_t)(tempU32 % 10);
  tempU32 /= 10;
  buffer[7] = (uint8_t)'0' + (uint8_t)(tempU32 % 10);
  tempU32 /= 10;
  buffer[6] = (uint8_t)'0' + (uint8_t)(tempU32 % 10);
  tempU32 /= 10;
  buffer[5] = (uint8_t)'0' + (uint8_t)(tempU32 % 10);
  tempU32 /= 10;
  buffer[4] = (uint8_t)'0' + (uint8_t)(tempU32 % 10);
  tempU32 /= 10;
  buffer[3] = (uint8_t)'0' + (uint8_t)(tempU32 % 10);
  tempU32 /= 10;
  buffer[2] = (uint8_t)'0' + (uint8_t)(tempU32 % 10);
  tempU32 /= 10;
  buffer[1] = (uint8_t)'0' + (uint8_t)(tempU32 % 10);
  tempU32 /= 10;
  buffer[0] = (uint8_t)'0' + (uint8_t)(tempU32 % 10);

  buffer[10] = '\0';

  uint8_t startIndex = 0;

  while( buffer[startIndex] == (uint8_t)'0' ){
    startIndex++;
  }

  uint8_t counter = 0;

  while( startIndex != 11 ){
	opBuffer[counter] = buffer[startIndex];
	counter++;
    startIndex++;
  }

  opBuffer[counter] = (uint8_t)'\0';

  Nokia5110_printString((char*)opBuffer);
}


void Nokia5110_printI8(int8_t n){

  if(n<0){
    n = -n;
	Nokia5110_printASCII('-');
  }

  Nokia5110_printU8((uint8_t)n);
}

void Nokia5110_printI16(int16_t n){

  if(n<0){
    n = -n;
	Nokia5110_printASCII('-');
  }

  Nokia5110_printU16((uint16_t)n);
}

void Nokia5110_printI32(int32_t n){

  if(n<0){
    n = -n;
	Nokia5110_printASCII('-');
  }

  Nokia5110_printU32((uint32_t)n);
}

void Nokia5110_printFloat(float n, uint8_t p){

  if(n < 0.0f){
    Nokia5110_printASCII('-');
    n = -n;
  }

  uint32_t inp = (uint32_t)n;
  uint32_t frp;

  if( p == 1 ){ frp = (uint32_t)((10.0f*n) - (10.0f*(float)inp)); };
  if( p == 2 ){ frp = (uint32_t)((100.0f*n) - (100.0f*(float)inp)); };
  if( p == 3 ){ frp = (uint32_t)((1000.0f*n) - (1000.0f*(float)inp)); };
  if( p == 4 ){ frp = (uint32_t)((10000.0f*n) - (10000.0f*(float)inp)); };

  Nokia5110_printU32_NB(inp,0);

  if( p!= 0 ){
    Nokia5110_printASCII('.');
    Nokia5110_printU32_NB(frp,p);
    Nokia5110_printASCII(' ');
  };
}

/*

void Nokia5110_updateDisplay(){
  Nokia5110_setPosition(0,0);

  for(int i = 0 ; i < 504 ; i++){
    Nokia5110_writeByte(Nokia5110.byte[i],1);
  }
}

*/

void Nokia5110_drawHorizontalLine(uint16_t xpxl_pos,
		                     uint16_t ypxl_pos,
		                     uint8_t length, uint8_t width){

  uint16_t position = ypxl_pos - 8*(uint16_t)(ypxl_pos/8);
  uint16_t mask = (( 1 << width ) - 1);

  for( int i = 0 ; i < length ; i++ ){
    Nokia5110.byte[84*(ypxl_pos>>3) + xpxl_pos + i] |= ( mask << position );
  }
}


void Nokia5110_drawVerticalLine(uint16_t xpxl_pos,
		                     uint16_t ypxl_pos,
		                     uint8_t length, uint8_t width){

  uint16_t tLength = length;
  uint16_t mask;

  for( int i = 0 ; i < length ; i++ ){
	mask = ( ( tLength >= 8 ) ? 0xFF : ( ( 1 << tLength ) - 1 ) );
	for( int j = 0 ; j < width ; j++ ){
		Nokia5110.byte[84*(((ypxl_pos+i)>>3)) + xpxl_pos + j ] |= mask;
	}
    tLength--;
  }
}

void Nokia5110_drawRectangle(uint16_t xpxl_pos, uint16_t ypxl_pos,
		                     uint16_t length, uint16_t width, uint16_t borderWidth)
{
  Nokia5110_drawVerticalLine(xpxl_pos,ypxl_pos,width,borderWidth);
  Nokia5110_drawVerticalLine(xpxl_pos+length-borderWidth,ypxl_pos,width,borderWidth);
  Nokia5110_drawHorizontalLine(xpxl_pos,ypxl_pos,length,borderWidth);
  Nokia5110_drawHorizontalLine(xpxl_pos,ypxl_pos+width-borderWidth,length,borderWidth);
}

void Nokia5110_drawBitmap(uint8_t* buffer,
		                  uint16_t x_org, uint16_t y_org,
						  uint16_t x_len, uint16_t y_len){

  uint16_t y_counter = 0;
  uint16_t x_counter = 0;
  uint16_t Pos = 0;
  uint16_t mskH = 0;
  uint16_t mskL = 0;
  uint16_t positionCounter = 0;

  while( y_counter < y_len ){

    while( x_counter < x_len ){

      if( (( y_org + y_counter ) % 8) == 0 ){
    	  Nokia5110.byte[ (x_org+x_counter) + 84 * ((y_org + y_counter)>>3) ] = buffer[positionCounter];
      }
      else
      {
    	  Pos = ( ( y_org+y_counter ) %8 );
    	  mskH = ( ( (0xFF & (( 1 << (8-Pos) ) - 1) ) ) << Pos );
    	  mskL = (( 0xFF >> (8-Pos) ) & (( 1 << Pos ) - 1));

          Nokia5110.byte[ (x_org + x_counter) + ( 84 * (((y_org+y_counter)>>3) + 0) ) ] &= ~mskH;
    	  Nokia5110.byte[ (x_org + x_counter) + ( 84 * (((y_org+y_counter)>>3) + 0) ) ]
    				|= ( ( (buffer[positionCounter] & (( 1 << (8-Pos) ) - 1) ) ) << Pos );
    	  Nokia5110.byte[ (x_org + x_counter) + ( 84 * (((y_org+y_counter)>>3) + 1) ) ] &= ~mskL;
    	  Nokia5110.byte[ (x_org + x_counter) + ( 84 * (((y_org+y_counter)>>3) + 1) ) ]
    	  				  |= (( buffer[positionCounter] >> (8-Pos) ) & (( 1 << Pos ) - 1));
      }

      positionCounter++;
      x_counter++;

      asm("NOP");
      asm("NOP");

    }/* end: while(x_counter<x_len) */

    x_counter = 0;
    y_counter+= 8;

    asm("NOP");
    asm("NOP");

  }/* end: while(y_counter<y_len) */

  asm("NOP");
  asm("NOP");
}
