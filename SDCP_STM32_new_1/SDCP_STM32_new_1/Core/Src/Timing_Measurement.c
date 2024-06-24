
#include "Timing_Measurement.h"

void Ticks_setTicks(Ticks_Ticks_t ticks)
{

  /* Stop Timer 4 and Timer 5 modules. */

  TIM4->CR1 &= (~TIM_CR1_CEN_Msk);
  TIM5->CR1 &= (~TIM_CR1_CEN_Msk);

  /* Reset counter values. */

  TIM4->CNT = 0;
  TIM5->CNT = 0;

  /* Store current tick configuration. */

  Ticks_CurrentTickBase = ticks;

  /*********	Configure clock tick generator (Timer 4).   **************/

  /*	Enable clock for Timer 4. */

  RCC->APB1ENR |= RCC_APB1ENR_TIM4EN_Msk;   // APB1 Maximum Clock Frequency is 42 MHz.

  /* Timer overflow timer = 42000000 (APB1 max. frequency ) / ( ( 1 + ARR ) * ( 1 + PSC ) ). */

  /* Prescaler is increased by 2 since it was observed that update event was generated on reaching
   * the timer upper limit (ARR) as well as the timer lower limit (0).*/

  switch(ticks)
  {
    case(Ticks_1us):
	{
      TIM4->ARR = 41;
      TIM4->PSC = 1;
	}
    break;

    case(Ticks_10us):
	{
      TIM4->ARR = 419;
      TIM4->PSC = 1;
	}
    break;

    case(Ticks_100us):
	{
      TIM4->ARR = (4200 - 1);
      TIM4->PSC = 1;
	}
    break;

    case(Ticks_1ms):
	{
      TIM4->ARR = (42000 - 1);
      TIM4->PSC = 1;
	}
    break;

    case(Ticks_10ms):
	{
      TIM4->ARR = (42000 - 1);
      TIM4->PSC = (10 - 1) << 1;
	}
    break;

    case(Ticks_100ms):
	{
      TIM4->ARR = (42000 - 1);
      TIM4->PSC = (100 - 1) << 1;
	}
    break;

    case(Ticks_1000ms):
	{
      TIM4->ARR = (42000 - 1);
      TIM4->PSC = (1000 - 1) << 1;
	}
    break;
  }

  /*  Set auto-preload enable bit. */

  TIM4->CR1 |= TIM_CR1_ARPE_Msk;

  /*	Set count direction to up. */

  TIM4->CR1 |= 0x0 << TIM_CR1_DIR_Pos;

  /*	Set counting mode to edge aligned mode. */

  TIM4->CR1 |= 0x0 << TIM_CR1_CMS_Pos;

  /* Enable master mode for Timer 4 with update event signal
   * used as external trigger output signal. */

  TIM4->CR2 |= ( 0x2UL << TIM_CR2_MMS_Pos );

  /* Make individual configurations for Timer 5 which receives clock from.
   * Timer 4. */

  RCC->APB1ENR |= RCC_APB1ENR_TIM5EN_Msk;

  /* Set Timer 5 prescaler to 0 and auto reload limit to maximum. */

  TIM5->PSC = 0;
  TIM5->ARR = 0xFFFFFFFFUL;

  /*  Set auto-preload enable bit. */

  TIM5->CR1 |= TIM_CR1_ARPE_Msk;

  /*	Set count direction to up. */

  TIM5->CR1 |= 0x0 << TIM_CR1_DIR_Pos;

  /*	Set counting mode to edge aligned mode. */

  TIM5->CR1 |= 0x0 << TIM_CR1_CMS_Pos;

  /* Set trigger source for Timer 5 as ITR2 (TS=010). */

  TIM5->SMCR = ( 0x2UL << TIM_SMCR_TS_Pos );

  /* Set slave mode to external clock source 1. */

  TIM5->SMCR |= ( 0x7UL << TIM_SMCR_SMS_Pos );

  /*	Enable counter of Timer 4. */

  TIM4->CR1 |= TIM_CR1_CEN_Msk;

  /* Enable counter of Timer 5. */

  TIM5->CR1 |= TIM_CR1_CEN_Msk;

 asm("NOP");
}

uint32_t Ticks_getTicks()
{
  return(TIM5->CNT);
}


static uint32_t Ticks_previousTick = 0;

uint32_t Ticks_getDeltaTick()
{
  uint32_t tempU32 = 0;

  tempU32 = ( (TIM5->CNT) - Ticks_previousTick );

  Ticks_previousTick = TIM5->CNT;

  return(tempU32);
}


static uint32_t Ticks_captureStartTick = 0;

uint32_t Ticks_startCapture()
{
  Ticks_captureStartTick = (TIM5->CNT);

  return(Ticks_captureStartTick);
}

static uint32_t Ticks_captureStopTick = 0;

uint32_t Ticks_stopCapture()
{
  Ticks_captureStopTick = (TIM5->CNT);

  return(Ticks_captureStopTick - Ticks_captureStartTick);
}

uint32_t Ticks_TicksToUs(uint32_t ticks)
{
  uint32_t microseconds = 0;

  switch(Ticks_CurrentTickBase)
  {
    case(Ticks_1us):
    {
      microseconds = ticks;
	}
    break;

    case(Ticks_10us):
	{
      microseconds = (10 * ticks);
	}
    break;

    case(Ticks_100us):
	{
      microseconds = (100 * ticks);
	}
    break;

    case(Ticks_1ms):
	{
      microseconds = (1000 * ticks);
	}
    break;

    case(Ticks_10ms):
	{
      microseconds = ( 10000 * ticks );
	}
    break;

    case(Ticks_100ms):
	{
      microseconds = ( 100000 * ticks );
	}
    break;

    case(Ticks_1000ms):
	{
      microseconds = ( 1000000 * ticks );
	}
    break;
  }

  return(microseconds);
}


void Ticks_enableInterrupt()
{
  /* Disable interrupts. */

  //__disable_irq();

  /* Enable update event interrupt flag. */

  TIM4->DIER = TIM_DIER_UIE_Msk;	//	Enable update event interrupt.

  NVIC_EnableIRQ(TIM4_IRQn);
  NVIC_SetPriority(TIM4_IRQn,4);

  /* Enable interrupts. */

  //__enable_irq();
}

void Ticks_disableInterrupt()
{
  /* Disable interrupts. */

  //__disable_irq();

  /* Enable update event interrupt flag. */

  TIM4->DIER &= ~TIM_DIER_UIE_Msk;	//	Enable update event interrupt.

  NVIC_DisableIRQ(TIM4_IRQn);

  /* Enable interrupts. */

  //__enable_irq();
}

/* IRQ Handler for timer tick event. */

void TIM4_IRQHandler(void)
{
  if(TIM4->SR & TIM_SR_UIF_Msk)
  {
    Tick_Handler();

	/* Clear update flag. */

    TIM4->SR &= ~TIM_SR_UIF_Msk;

    Tick_IF = 1;
  }
}


/***************** Tick Handler  **********/

static uint8_t toggleCounter = 0;

void Tick_Handler()
{
  if( toggleCounter >= 10 )
  {
    GPIOA->ODR ^= ( 1 << 5 );
    toggleCounter = 0;
  }
  else
  {
	  toggleCounter++;
  }

  MPU6050_readAllSensorData();
  MP_Update();
  //Protocol_Manager();
  TM_dispatcher();
}

