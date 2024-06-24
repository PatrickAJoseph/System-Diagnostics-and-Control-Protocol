
#include "System.h"


/*********************************************************************************/

/*
 * @brief: System_ClockInit(): Function to configure system base clock to 84 MHz.
 *
 * */

void System_ClockInit()
{
	/*	Turn on High Speed Internal Oscillator (16MHz). */

	RCC->CR |= RCC_CR_HSION_Msk;

	/*	Wait until HSI oscillator is ready. */

	while( ( RCC->CR & RCC_CR_HSIRDY_Msk ) == 0 );

	/*	Load default calibration value of 0x10. */

	RCC->CR |= RCC_CR_HSITRIM_4;

	/*	Turn on High Speed External oscillator (8MHz). */

	RCC->CR |= RCC_CR_HSEON_Msk;

	/*	Wait until external oscillator is ready. */

	while( ( RCC->CR & RCC_CR_HSERDY_Msk ) == 0 );

	int i = 0;

	while( ++i < 1000 );

	/*	Enable clocks. */

	RCC->APB1ENR |= 	RCC_APB1ENR_PWREN_Msk; 			//	Power module.
	RCC->APB2ENR |= 	RCC_APB2ENR_SYSCFGEN_Msk;		//	System configuration module.

	/****	PLL Configuration. 	****/

	/*	Turn off PLL. */

	RCC->CR &= ~RCC_CR_PLLON_Msk;

	/*	Configure basic PLL settings. */

	RCC->PLLCFGR = 		RCC_PLLCFGR_PLLSRC_Msk					/*	Select HSE as PLL clock source. */
					|	( 4 << RCC_PLLCFGR_PLLM_Pos )			/*	Set the VCO clock divider. */
					|	( 84 << RCC_PLLCFGR_PLLN_Pos )			/*	Set the PLL VCO frequency multiplication factor. */
					|	( 0 << RCC_PLLCFGR_PLLP_Pos )			/*	Set the main PLL output clock division factor. */
					| 	( 4 << RCC_PLLCFGR_PLLQ_Pos );			/*	Set I2S PLL clock division factor. */

	/*	Turn ON PLL. */

	RCC->CR |= RCC_CR_PLLON_Msk;

	/*	Wait until PLL is ready. */

	while( ( RCC->CR & RCC_CR_PLLRDY_Msk ) == 0 );

	/*	Increase FLASH access latency since system frequency is increased. */

	*(__IO uint8_t *)ACR_BYTE0_ADDRESS = (uint32_t)2;

	RCC->CFGR =  ( 0x4U << RCC_CFGR_PPRE1_Pos ) |	//	Set APB1 low-speed clock divider to 2 to set its clock speed to 42 MHz.
				 ( 0x0U << RCC_CFGR_PPRE2_Pos ) |	//	No clock division for APB1 high-speed.
				 RCC_CFGR_SW_PLL;					//	Select PLL output as system clock source.

	/*	Wait until system clock source change in reflected. */

	while( ( RCC->CFGR & RCC_CFGR_SWS_Msk ) != RCC_CFGR_SWS_PLL );
}


/*************************************************************************************/

/*
 *
 * @brief: Function to generate delay using SysTick timer in steps of milliseconds.
 *
 * */

void System_SysTick_delay_ms(uint16_t delay_ms)
{
	while( delay_ms )
	{
		/*	Disable SysTick timer. */

		SysTick->CTRL = 0;

		/*	Load value with 0. */

		SysTick->VAL = 0;

		/*	Load upper limit of SysTick timer. */

		SysTick->LOAD = (F_CPU / 1000) - 1;

		/*	Enable SysTick timer and select clock source as processor clock. */

		SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk;

		/*	Wait until SysTick timer reaches limit. */

		while( ( SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk ) == 0 );

		/*	Decrement counter variable. */

		delay_ms--;
	}
}


