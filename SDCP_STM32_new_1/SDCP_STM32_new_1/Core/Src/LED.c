
#include "LED.h"

void LED_Init()
{
  LED.LED_parameters.LED1_mode = (uint32_t)Config.LED_config.LED_config_segments.LED1_mode;
  LED.LED_parameters.LED2_mode = (uint32_t)Config.LED_config.LED_config_segments.LED2_mode;
  LED.LED_parameters.LED3_mode = (uint32_t)Config.LED_config.LED_config_segments.LED3_mode;

  LED_CNTRL_SMF.LED_CNTRL_SMF_segments.XLED_mode = Config.LED_config.LED_config_segments.LED1_mode;
  LED_CNTRL_SMF.LED_CNTRL_SMF_segments.YLED_mode = Config.LED_config.LED_config_segments.LED2_mode;
  LED_CNTRL_SMF.LED_CNTRL_SMF_segments.ZLED_mode = Config.LED_config.LED_config_segments.LED3_mode;

  LED.LED_parameters.LED1_state = 0;
  LED.LED_parameters.LED2_state = 0;
  LED.LED_parameters.LED3_state = 0;

  LED.LED_parameters.LED1_duty = 0;
  LED.LED_parameters.LED2_duty = 0;
  LED.LED_parameters.LED3_duty = 0;

  LED.LED_parameters.Test_LED_state = 0;

  /* Test LED configuration. */

  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN_Msk;

  GPIOA->MODER |= ( 0x1UL << ( 5 << 1 ) );

  /****************  Timer 3 initialization in PWM mode. ***********************/

  RCC->AHB1ENR |= ( RCC_AHB1ENR_GPIOBEN_Msk | RCC_AHB1ENR_GPIOCEN_Msk );

  /* Initialize Timer 3 in PWM mode. */

  /*	Enable clock for Timer 3. */

  RCC->APB1ENR |= RCC_APB1ENR_TIM3EN_Msk;

  /*  Set CC1, CC2 and CC3 unit to output mode. */

  TIM3->CCMR1 |= ( 0x0UL << TIM_CCMR1_CC1S_Pos ) | ( 0x0UL << TIM_CCMR1_CC2S_Pos );
  TIM3->CCMR2 |= ( 0x0UL << TIM_CCMR2_CC3S_Pos );

  /*  Set CC1, CC2 and CC3 unit output polarity to active high. */

  TIM3->CCER |= ( (0x0UL << TIM_CCER_CC1P_Pos)
		        | (0x0UL << TIM_CCER_CC2P_Pos)
				| (0x0UL << TIM_CCER_CC3P_Pos) );

  /*  Set PWM mode for CC1 to PWM mode 1. */

  TIM3->CCMR1 |= ( (0x6UL << TIM_CCMR1_OC1M_Pos) | (0x6UL << TIM_CCMR1_OC2M_Pos) );
  TIM3->CCMR2 |= ( 0X6ul << TIM_CCMR2_OC3M_Pos );

  /*	Set PWM period to 10 kHz. */

  TIM3->ARR = 419;
  TIM3->PSC = 9;

  /*  Set Duty Cycle to 50% */

  TIM3->CCR1 = (TIM3->ARR >> 1);

  /*	Set preload enable bit. */

  TIM3->CCMR1 |= ( TIM_CCMR1_OC1PE_Msk | TIM_CCMR1_OC2PE_Msk );
  TIM3->CCMR2 |= TIM_CCMR2_OC3PE_Msk;

  /*	Set auto-preload enable bit. */

  TIM3->CR1 |= TIM_CR1_ARPE_Msk;

  /*	Set count direction to up. */

  TIM3->CR1 |= 0x0 << TIM_CR1_DIR_Pos;

  /*	Set counting mode to edge aligned mode. */

  TIM3->CR1 |= 0x0 << TIM_CR1_CMS_Pos;

  /*	Enable CC1, CC2 and CC3 output. */

  TIM3->CCER |= ( TIM_CCER_CC3E_Msk | TIM_CCER_CC2E_Msk | TIM_CCER_CC1E_Msk );

  /*	Enable counter. */

  TIM3->CR1 |= TIM_CR1_CEN_Msk;

  /*Update state of LEDs.*/

  LED_Update();
}


void LED_Update()
{
	/* If LED over-riding is enabled, discard the values written via MCU
	 * and update from the communication stack. */

	if( LED_CNTRL_SMF.LED_CNTRL_SMF_segments.LED_OVRRD )
	{
		LED.LED_parameters.LED1_state = LED_CNTRL_SMF.LED_CNTRL_SMF_segments.XLED_state;
		LED.LED_parameters.LED2_state = LED_CNTRL_SMF.LED_CNTRL_SMF_segments.YLED_state;
		LED.LED_parameters.LED3_state = LED_CNTRL_SMF.LED_CNTRL_SMF_segments.ZLED_state;
		LED.LED_parameters.LED1_mode = LED_CNTRL_SMF.LED_CNTRL_SMF_segments.XLED_mode;
		LED.LED_parameters.LED2_mode = LED_CNTRL_SMF.LED_CNTRL_SMF_segments.YLED_mode;
		LED.LED_parameters.LED3_mode = LED_CNTRL_SMF.LED_CNTRL_SMF_segments.ZLED_mode;
		LED.LED_parameters.LED1_duty = LED_CNTRL_SMF.LED_CNTRL_SMF_segments.XLED_Brightness;
		LED.LED_parameters.LED2_duty = LED_CNTRL_SMF.LED_CNTRL_SMF_segments.YLED_Brightness;
		LED.LED_parameters.LED3_duty = LED_CNTRL_SMF.LED_CNTRL_SMF_segments.ZLED_Brightness;
	}

  /* Update the contents of the message frames. */

  LED_CNTRL_SMF.LED_CNTRL_SMF_segments.XLED_state = LED.LED_parameters.LED1_state;
  LED_CNTRL_SMF.LED_CNTRL_SMF_segments.YLED_state = LED.LED_parameters.LED2_state;
  LED_CNTRL_SMF.LED_CNTRL_SMF_segments.ZLED_state = LED.LED_parameters.LED3_state;
  LED_CNTRL_SMF.LED_CNTRL_SMF_segments.XLED_Brightness = LED.LED_parameters.LED1_duty;
  LED_CNTRL_SMF.LED_CNTRL_SMF_segments.YLED_Brightness = LED.LED_parameters.LED2_duty;
  LED_CNTRL_SMF.LED_CNTRL_SMF_segments.ZLED_Brightness = LED.LED_parameters.LED3_duty;
  LED_CNTRL_SMF.LED_CNTRL_SMF_segments.XLED_state = LED.LED_parameters.LED1_state;
  LED_CNTRL_SMF.LED_CNTRL_SMF_segments.YLED_state = LED.LED_parameters.LED2_state;
  LED_CNTRL_SMF.LED_CNTRL_SMF_segments.ZLED_state = LED.LED_parameters.LED3_state;
  LED_CNTRL_SMF.LED_CNTRL_SMF_segments.XLED_mode = LED.LED_parameters.LED1_mode;
  LED_CNTRL_SMF.LED_CNTRL_SMF_segments.YLED_mode = LED.LED_parameters.LED2_mode;
  LED_CNTRL_SMF.LED_CNTRL_SMF_segments.ZLED_mode = LED.LED_parameters.LED3_mode;

  if( ( LED.LED_parameters.LED1_mode == Accelerometer_measurement ) ||
	  ( LED.LED_parameters.LED1_mode == Gyroscope_measurement ) ||
	  ( LED.LED_parameters.LED1_mode == PWM_test )
   )
  {
	/* LED1: PB4 / TIM3_CH1. */

	/* PB4 assigned alternate function. */

	GPIOB->MODER &= (~( 0x3UL << ( 4 << 1 ) ));
    GPIOB->MODER |= ( 0x2UL << ( 4 << 1 ) );

    /* PB4 alternate function: TIM3_CH1. */

    GPIOB->AFR[0] &= (~( 0xFUL << ( 4 << 2 ) ));
    GPIOB->AFR[0] |= ( 0x2UL << ( 4 << 2 ) );
  }
  else if( ( LED.LED_parameters.LED1_mode == Angle_detection ) ||
		  ( LED.LED_parameters.LED1_mode == Vibration_detection ) ||
		  ( LED.LED_parameters.LED1_mode == GPIO_test )
  )
  {
    GPIOB->MODER &= ~( 0x3UL << ( 4 << 1 ) );
    GPIOB->MODER |= ( 0x1UL << ( 4 << 1 ) );

    GPIOB->AFR[0] &= (~( 0xFUL << ( 4 << 2 ) ));
    GPIOB->AFR[0] |= ( 0x0UL << ( 4 << 2 ) );

    if(LED.LED_parameters.LED1_state)
    {
      GPIOB->ODR |= ( 0x1UL << 4 );
    }
    else
    {
      GPIOB->ODR &= (~( 0x1UL << 4 ));
    }
  }

  if(
      ( LED.LED_parameters.LED2_mode == Accelerometer_measurement ) ||
	  ( LED.LED_parameters.LED2_mode == Gyroscope_measurement ) ||
	  ( LED.LED_parameters.LED2_mode == PWM_test )
    )
  {
	/* LED2: PB5 / TIM3_CH2. */

	/* PB5 assigned alternate function. */

	GPIOB->MODER &= (~( 0x3UL << ( 5 << 1 ) ));
    GPIOB->MODER |= ( 0x2UL << ( 5 << 1 ) );

    /* PB4 alternate function: TIM3_CH2. */

    GPIOB->AFR[0] &= (~( 0xFUL << ( 5 << 2 ) ));
    GPIOB->AFR[0] |= ( 0x2UL << ( 5 << 2 ) );
  }
  else if(
    ( LED.LED_parameters.LED2_mode == Angle_detection ) ||
	( LED.LED_parameters.LED2_mode == Vibration_detection ) ||
    ( LED.LED_parameters.LED2_mode == GPIO_test)
  )
  {
    GPIOB->MODER &= ~( 0x3UL << ( 5 << 1 ) );
    GPIOB->MODER |= ( 0x1UL << ( 5 << 1 ) );

    GPIOB->AFR[0] &= (~( 0xFUL << ( 5 << 2 ) ));
    GPIOB->AFR[0] |= ( 0x0UL << ( 5 << 2 ) );

    if(LED.LED_parameters.LED2_state)
    {
      GPIOB->ODR |= ( 0x1UL << 5 );
    }
    else
    {
      GPIOB->ODR &= (~( 0x1UL << 5 ));
    }
  }


  if(
	  ( LED.LED_parameters.LED3_mode == Accelerometer_measurement ) ||
	  ( LED.LED_parameters.LED3_mode == Gyroscope_measurement ) ||
	  ( LED.LED_parameters.LED3_mode == PWM_test )
  )
  {
	/* LED3: PC8 / TIM3_CH3. */

	/* PC8 assigned alternate function. */

	GPIOC->MODER &= (~( 0x3UL << ( 8 << 1 ) ));
    GPIOC->MODER |= ( 0x2UL << ( 8 << 1 ) );

    /* PC8 alternate function: TIM3_CH3. */

    GPIOC->AFR[1] &= (~( 0xFUL << ( ( 8 - 8 ) << 2 ) ));
    GPIOC->AFR[1] |= ( 0x2UL << ( ( 8 - 8 ) << 2 ) );
  }
  else if(
		  ( LED.LED_parameters.LED3_mode == Angle_detection ) ||
		  ( LED.LED_parameters.LED3_mode == Vibration_detection ) ||
		  ( LED.LED_parameters.LED3_mode == GPIO_test )
  )
  {
    GPIOC->MODER &= ~( 0x3UL << ( 8 << 1 ) );
    GPIOC->MODER |= ( 0x1UL << ( 8 << 1 ) );

    GPIOC->AFR[1] &= (~( 0xFUL << ( ( 8 - 8 )<< 2 ) ));
    GPIOC->AFR[1] |= ( 0x0UL << ( ( 8 - 8 ) << 2 ) );

    if(LED.LED_parameters.LED3_state)
    {
      GPIOC->ODR |= ( 0x1UL << 8 );
    }
    else
    {
      GPIOC->ODR &= (~( 0x1UL << 8 ));
    }
  }

  if( LED.LED_parameters.Test_LED_state )
  {
    GPIOA->ODR |= ( 0x1U << 5 );
  }
  else
  {
    GPIOA->ODR &= ~( 0x1UL << 5 );
  }

  /* Set LED duty cycles. */

  TIM3->CCR1 = LED_PWM_Map(LED.LED_parameters.LED1_duty);
  TIM3->CCR2 = LED_PWM_Map(LED.LED_parameters.LED2_duty);
  TIM3->CCR3 = LED_PWM_Map(LED.LED_parameters.LED3_duty);
}

uint16_t LED_PWM_Map(uint8_t duty)
{
  uint32_t result = 0;

  result = (uint32_t)duty * (uint32_t)TIM3->ARR;
  result = (5*result) >> 9;

  return( (uint16_t)result );
}
