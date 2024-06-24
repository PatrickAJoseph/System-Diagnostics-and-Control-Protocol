
#ifndef INC_FLASH_H_
#define INC_FLASH_H_

#include "stm32f4xx.h"

/* Header file containing functions to control read and writes of configuration
 * from and to the program flash. */

#define Flash_Start		0x08040000UL
#define Flash_Stop		0x0805FFFFUL
#define Flash_Sector	6UL

typedef enum
{

  Flash_Ok = 0,
  Flash_Busy = 1,
  Flash_Error = 2,
  Flash_Unlocked = 3,
  Flash_Write_Success = 4,
  Flash_Write_Fail = 5,
  Flash_Locked = 6

}Flash_Operation_State_t;


/* Function to unlock flash. */

Flash_Operation_State_t Flash_Unlock();
Flash_Operation_State_t Flash_Lock();
Flash_Operation_State_t Flash_Sector_Erase();
Flash_Operation_State_t Flash_Error_State();

Flash_Operation_State_t Flash_writeByte(uint32_t address, uint8_t value);
Flash_Operation_State_t Flash_writeString(uint32_t address, char* string, uint16_t length);
Flash_Operation_State_t Flash_writeByte(uint32_t address, uint8_t value);
Flash_Operation_State_t Flash_writeHalfWord(uint32_t address, uint16_t value);
Flash_Operation_State_t Flash_writeWord(uint32_t address, uint32_t value);
Flash_Operation_State_t Flash_write(uint32_t flashStartAddress, uint32_t ramStartAddress, uint32_t nBytes);
Flash_Operation_State_t Flash_read(uint32_t fStartAddr, uint32_t rStartAddr, uint32_t n);

#endif /* INC_FLASH_H_ */
