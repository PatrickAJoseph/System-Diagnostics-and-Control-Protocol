
#ifndef INC_SYSTEM_H_
#define INC_SYSTEM_H_

#include "main.h"

#define F_CPU	84000000UL		/*	System frequency is set to 84 MHz. */

/*
 * @brief: Function to initialize system clocks.
 *
 * */

void System_ClockInit();

/*
 * @brief: Functions to generate SysTick timer delay in milliseconds.
 *
 * */

void System_SysTick_delay_ms(uint16_t);

/*
 * @brief: Breakpoint function for debugging.
 *
 * */

void __breakpoint();

#endif /* INC_SYSTEM_H_ */
