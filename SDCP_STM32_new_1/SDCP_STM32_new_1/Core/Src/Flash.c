
#include "Flash.h"

Flash_Operation_State_t Flash_Unlock()
{
  /* Disable interrupts. */

  __disable_irq();

  FLASH->KEYR = 0x45670123;

  //asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");

  FLASH->KEYR = 0xCDEF89AB;

  //asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");

  /* Wait until busy flag is reset. */

  while( (FLASH->SR & FLASH_SR_BSY_Msk) );

  /* Enable all flash interrupt flags (error and end of operations). */

  FLASH->CR |= ( ( 1UL << 25 ) | FLASH_CR_EOPIE_Msk );

  /* Enable interrupts. */

  //__enable_irq();

  return(Flash_Unlocked);
}

Flash_Operation_State_t Flash_Lock()
{
  /* Disable interrupts. */

  __disable_irq();

  /* Set LOCK bit to lock the flash and access to flash control register. */

  FLASH->CR |= FLASH_CR_LOCK_Msk;

  /* Wait until flash interface completes the required operation. */

  while( ( FLASH->SR & FLASH_SR_BSY_Msk ) );

  /* Enable interrupts. */

  //__enable_irq();
}


Flash_Operation_State_t Flash_State()
{

}


Flash_Operation_State_t Flash_Sector_Erase()
{
  /* Disable interrupts. */

  __disable_irq();

  /* Check flash busy flag. */

  while( FLASH->SR & FLASH_SR_BSY_Msk );

  /* Select sector to be erased and set the SER bit. */

  FLASH->CR |= ( ( FLASH_CR_SER_Msk )
		    |  ( (uint32_t)(Flash_Sector) << FLASH_CR_SNB_Pos ) );

  /* Set start bit for start erasing of the required sector. */

  FLASH->CR |= FLASH_CR_STRT_Msk;

  /* Wait for some time before starting the poll operation. */

  asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
  asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");

  Flash_Operation_State_t operationState;

  /* Wait until the busy flag is cleared or the end of operation bit is not set. */

  while( (FLASH->SR & FLASH_SR_BSY_Msk) || !(FLASH->SR & FLASH_SR_EOP_Msk) )
  {
    if(   ( FLASH->SR & ( 1UL << 1 ) )
        | ( FLASH->SR & FLASH_SR_WRPERR_Msk )
		| ( FLASH->SR & FLASH_SR_PGAERR_Msk )
		| ( FLASH->SR & FLASH_SR_PGPERR_Msk )
		| ( FLASH->SR & FLASH_SR_PGSERR_Msk )
		| ( FLASH->SR & FLASH_SR_RDERR_Msk ) )
    {
      /* Clear all interrupt flags. */

      FLASH->SR |= ( FLASH_SR_WRPERR_Msk | FLASH_SR_PGAERR_Msk | FLASH_SR_PGPERR_Msk
      		        | FLASH_SR_PGSERR_Msk | FLASH_SR_RDERR_Msk | ( 1UL << 1 ) );
      return(Flash_Error);
    }
  }

  //__enable_irq();

  return(Flash_Ok);
}

Flash_Operation_State_t Flash_writeByte(uint32_t address, uint8_t value)
{
  __disable_irq();

  /* Wait until flash operations are completed. */

  while( FLASH->SR & FLASH_SR_BSY_Msk );

  /* Set PG bit to start programming. */

  FLASH->CR |= FLASH_CR_PG_Msk;

  /* Set flash access size to access a byte. */

  FLASH->CR &= ~( FLASH_CR_PSIZE_Msk );
  FLASH->CR |= ( 0x0UL << FLASH_CR_PSIZE_Pos );

  /* Wait until flash controller has completed operation. */

  while( FLASH->SR & FLASH_SR_BSY_Msk );

  /* Load required value into memory address. */

  *((volatile uint8_t*)address) = value;

  Flash_Operation_State_t operationState;

  /* Wait until the busy flag is cleared or the end of operation bit is not set. */

  while( (FLASH->SR & FLASH_SR_BSY_Msk) || !(FLASH->SR & FLASH_SR_EOP_Msk) )
  {
    if(   ( FLASH->SR & ( 1UL << 1 ) )
        | ( FLASH->SR & FLASH_SR_WRPERR_Msk )
		| ( FLASH->SR & FLASH_SR_PGAERR_Msk )
		| ( FLASH->SR & FLASH_SR_PGPERR_Msk )
		| ( FLASH->SR & FLASH_SR_PGSERR_Msk )
		| ( FLASH->SR & FLASH_SR_RDERR_Msk ) )
    {
      /* Clear all interrupt flags. */

      FLASH->SR |= ( FLASH_SR_WRPERR_Msk | FLASH_SR_PGAERR_Msk | FLASH_SR_PGPERR_Msk
      		        | FLASH_SR_PGSERR_Msk | FLASH_SR_RDERR_Msk | ( 1UL << 1 ) );

      /* Disable flash programming. */

      FLASH->CR &= (~FLASH_CR_PG_Msk);

      return(Flash_Error);
    }
  }

  /* Disable programming. */

  FLASH->CR &= (~FLASH_CR_PG_Msk);

  //__enable_irq();

  return(Flash_Ok);

}


Flash_Operation_State_t Flash_writeString(uint32_t address, char* string, uint16_t length)
{
  Flash_Operation_State_t flashState;

  uint16_t counter = 0;

  while(length)
  {
	flashState = Flash_Write_U8(address+(uint32_t)counter, (uint8_t)string[counter]);
    counter++;
    length--;

    if(flashState != Flash_Ok)
    {
      break;
    }
  }

  return(flashState);
}



Flash_Operation_State_t Flash_writeHalfWord(uint32_t address, uint16_t value)
{
  __disable_irq();

  /* Wait until flash operations are completed. */

  while( FLASH->SR & FLASH_SR_BSY_Msk );

  /* Set PG bit to start programming. */

  FLASH->CR |= FLASH_CR_PG_Msk;

  /* Set flash access size to access a half word. */

  FLASH->CR &= ~( FLASH_CR_PSIZE_Msk );
  FLASH->CR |= ( 0x1UL << FLASH_CR_PSIZE_Pos );

  /* Wait until flash controller has completed operation. */

  while( FLASH->SR & FLASH_SR_BSY_Msk );

  /* Load required value into memory address. */

  *((volatile uint16_t*)address) = value;

  Flash_Operation_State_t operationState;

  /* Wait until the busy flag is cleared or the end of operation bit is not set. */

  while( (FLASH->SR & FLASH_SR_BSY_Msk) || !(FLASH->SR & FLASH_SR_EOP_Msk) )
  {
    if(   ( FLASH->SR & ( 1UL << 1 ) )
        | ( FLASH->SR & FLASH_SR_WRPERR_Msk )
		| ( FLASH->SR & FLASH_SR_PGAERR_Msk )
		| ( FLASH->SR & FLASH_SR_PGPERR_Msk )
		| ( FLASH->SR & FLASH_SR_PGSERR_Msk )
		| ( FLASH->SR & FLASH_SR_RDERR_Msk ) )
    {
      /* Clear all interrupt flags. */

      FLASH->SR |= ( FLASH_SR_WRPERR_Msk | FLASH_SR_PGAERR_Msk | FLASH_SR_PGPERR_Msk
      		        | FLASH_SR_PGSERR_Msk | FLASH_SR_RDERR_Msk | ( 1UL << 1 ) );
      return(Flash_Error);
    }
  }

  __enable_irq();

  return(Flash_Ok);

}


Flash_Operation_State_t Flash_writeWord(uint32_t address, uint32_t value)
{
  __disable_irq();

  /* Wait until flash operations are completed. */

  while( FLASH->SR & FLASH_SR_BSY_Msk );

  /* Set PG bit to start programming. */

  FLASH->CR |= FLASH_CR_PG_Msk;

  /* Set flash access size to access a half word. */

  FLASH->CR &= ~( FLASH_CR_PSIZE_Msk );
  FLASH->CR |= ( 0x2UL << FLASH_CR_PSIZE_Pos );

  /* Wait until flash controller has completed operation. */

  while( FLASH->SR & FLASH_SR_BSY_Msk );

  /* Load required value into memory address. */

  *((volatile uint32_t*)address) = value;

  Flash_Operation_State_t operationState;

  /* Wait until the busy flag is cleared or the end of operation bit is not set. */

  while( (FLASH->SR & FLASH_SR_BSY_Msk) || !(FLASH->SR & FLASH_SR_EOP_Msk) )
  {
    if(   ( FLASH->SR & ( 1UL << 1 ) )
        | ( FLASH->SR & FLASH_SR_WRPERR_Msk )
		| ( FLASH->SR & FLASH_SR_PGAERR_Msk )
		| ( FLASH->SR & FLASH_SR_PGPERR_Msk )
		| ( FLASH->SR & FLASH_SR_PGSERR_Msk )
		| ( FLASH->SR & FLASH_SR_RDERR_Msk ) )
    {
      /* Clear all interrupt flags. */

      FLASH->SR |= ( FLASH_SR_WRPERR_Msk | FLASH_SR_PGAERR_Msk | FLASH_SR_PGPERR_Msk
      		        | FLASH_SR_PGSERR_Msk | FLASH_SR_RDERR_Msk | ( 1UL << 1 ) );
      return(Flash_Error);
    }
  }

  //__enable_irq();

  return(Flash_Ok);

}



Flash_Operation_State_t Flash_write(uint32_t flashStartAddress,
		                            uint32_t ramStartAddress,
									uint32_t nBytes)
{
  uint32_t counter = 0;
  uint8_t value = 0;
  Flash_Operation_State_t flashState;

  while( counter < nBytes  )
  {
	value =   *( (volatile uint8_t*)(ramStartAddress + counter) );

    flashState = Flash_writeByte( flashStartAddress + counter, value );

    if( flashState != Flash_Ok )
    {
      return(Flash_Error);
    }

    counter++;
  }

  return(Flash_Ok);
}


Flash_Operation_State_t Flash_read(uint32_t fStartAddr, uint32_t rStartAddr, uint32_t n)
{
  uint32_t counter = 0;

  while( counter < n )
  {
    *( (uint32_t*)( rStartAddr + counter ) ) = *( (uint8_t*)( fStartAddr + counter ) );
    counter++;
  }
}