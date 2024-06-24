
#include "Rotary_Encoder.h"

#define SW_DEBOUNCE_LIMIT		1
#define DT_DEBOUNCE_LIMIT		1
#define CLK_DEBOUNCE_LIMIT		1

static uint8_t rotaryEncoder_SW_counter = 0;
static uint8_t rotaryEncoder_DT_counter = 0;
static uint8_t rotaryEncoder_CLK_counter = 0;

void RotaryEncoder_Init(){

  /* Enable Port C.*/

  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN_Msk;

  /* Configure PC10, PC11 and PC12 as inputs. */

  GPIOC->MODER &= ~( 0x3UL << ( 2 * 10 ) );
  GPIOC->MODER &= ~( 0x3UL << ( 2 * 11 ) );
  GPIOC->MODER &= ~( 0x3UL << ( 2 * 12 ) );

  /* Enable pull-up resistor at PC10, PC11 and PC12. */

   GPIOC->PUPDR |= ( 0x1UL << ( 2 * 10 ) );
   GPIOC->PUPDR |= ( 0x1UL << ( 2 * 11 ) );
   GPIOC->PUPDR |= ( 0x1UL << ( 2 * 12 ) );

  /* Enable external interrupts on pins PC10, PC11 and PC12. */

  /* Source for EXTI12 is from PC. */

  SYSCFG->EXTICR[3] |= ( 0x2UL << ( 0 << 2 ) );

  /* Source for EXTI11 is from PC. */

  SYSCFG->EXTICR[2] |= ( 0x2UL << ( 3 << 2 ) );

  /* Source for EXTI12 is from PC. */

  SYSCFG->EXTICR[2] |= ( 0x2UL << ( 2 << 2 ) );

  /* Unmask interrupt bits for EXTI10, EXTI11 and EXTI12. */

  //EXTI->IMR |= ( ( 1 << 10 ) | ( 1 << 11 ) | ( 1 << 12 ) );
  //EXTI->EMR |= ( ( 1 << 10 ) | ( 1 << 11 ) | ( 1 << 12 ) );

  EXTI->IMR |= ( ( 1 << 10 ) | ( 1 << 12 ) );
  EXTI->EMR |= ( ( 1 << 10 ) | ( 1 << 12 ) );


  /* Enable interrupt for EXTI10, EXTI11 and EXTI12 at rising and falling
   * edge. */

  //EXTI->RTSR |= ( ( 1 << 12 ) );
  EXTI->FTSR |= ( ( 1 << 10 ) | ( 1 << 12) );

  /* Enable interrupts for EXTI13, EXTI14 and EXTI15. */

  NVIC_SetPriority (EXTI15_10_IRQn, 9);
  NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void EXTI15_10_IRQHandler(void)
{
  //__disable_irq();

  uint8_t CLK = ( ( ( GPIOC->IDR & ( 1 << 11 ) ) >> 11 ) == 1 );
  uint8_t SW = ( ( ( GPIOC->IDR & ( 1 << 10 ) ) >> 10 ) == 0 );
  uint8_t DT = ( ( ( GPIOC->IDR & ( 1 << 12 ) ) >> 12 ) == 1 );

  /* EXTI10: Rotary encoder switch press. */

  if( (EXTI->PR & ( 1 << 10 )) ){

	    if(SW)
	    {

		rotaryEncoder_SW_counter++;

		if( rotaryEncoder_SW_counter >= SW_DEBOUNCE_LIMIT )
		{
		  rotaryEncoder_SW_counter = 0;
		  if(Rotary_Encoder.parameters.switch_state == 1 )
		  {
	        Rotary_Encoder.parameters.switch_state = 0;
		  }
		  else
		  {
		    Rotary_Encoder.parameters.switch_state = 1;
		  }
	      Rotary_Encoder.parameters.switch_counter++;
		}

	    }

    EXTI->PR |= ( 1 << 10 );
  }
  if( EXTI->PR & ( 1 << 12 ) ){

	//Rotary_Encoder.parameters.switch_state = 0;

    if( !CLK ){

      rotaryEncoder_DT_counter++;

      if(rotaryEncoder_DT_counter >= DT_DEBOUNCE_LIMIT)
      {
    	rotaryEncoder_DT_counter = 0;

    	Rotary_Encoder.parameters.clockwise = 0;
        Rotary_Encoder.parameters.anticlockwise = 1;

        if(Rotary_Encoder.parameters.rotation_counter){
          Rotary_Encoder.parameters.rotation_counter--;
        }
      }
    }
    if( CLK & !DT )
    {
    	rotaryEncoder_CLK_counter++;

    	if(rotaryEncoder_CLK_counter >= CLK_DEBOUNCE_LIMIT )
    	{
    	  rotaryEncoder_CLK_counter = 0;

          Rotary_Encoder.parameters.clockwise = 1;
          Rotary_Encoder.parameters.anticlockwise = 0;
          Rotary_Encoder.parameters.rotation_counter++;
    	}
    }

    EXTI->PR |= ( 1 << 12 );
  }

  //EXTI->PR |= ( ( 1 << 10 ) | ( 1 << 11 ) | ( 1 << 12 ) );

  /* Update communication frame variables. */

  GUI_SMF.GUI_SMF_segments.RENC_CLK  = Rotary_Encoder.parameters.clockwise;
  GUI_SMF.GUI_SMF_segments.RENC_ACLK = Rotary_Encoder.parameters.anticlockwise;
  GUI_SMF.GUI_SMF_segments.RENC_SW   = Rotary_Encoder.parameters.switch_state;
  GUI_SMF.GUI_SMF_segments.rotation_counter = Rotary_Encoder.parameters.rotation_counter;
  GUI_SMF.GUI_SMF_segments.switch_counter = Rotary_Encoder.parameters.switch_counter;

  //__enable_irq();
}


