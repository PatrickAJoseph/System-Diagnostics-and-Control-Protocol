
#include "Application.h"
#include "Motion_Processing.h"

uint8_t counter = 0;

int main(void)
{
  /* Device peripheral initializations. */

  __disable_irq();

  System_ClockInit();

  Config_read();

  Ticks_setTicks(Ticks_10ms);
  Ticks_enableInterrupt();

  UART_Init();

  /* External devices initializations. */

  RotaryEncoder_Init();
  MPU6050_Init();
  LED_Init();
  LCD_GUI_Init();

  PM_Init();
  MP_Init();

  /* Tasks to be executed in the main function. */

  TM_addTask(0,&LED_Update,10);
  TM_addTask(1,&LCD_GUI_Update,15);
  TM_addTask(2,&Protocol_Manager,1);

  __enable_irq();

  while (1)
  {
	if( Tick_IF )
	{
	  /* Loop through induvidual tasks. */

      for(int i = 0 ; i < TM_nTasks ; i++)
      {
    	/* Execute all pending executions of current tasks. */

    	uint32_t pendingExecutions = TM_Task[i].pendingExecutions;

        for(int j = 0 ; j < pendingExecutions ; j++ )
        {
        	if( TM_Task[i].runnable )
        	{
        	  (*TM_Task[i].fptr)();
        	}

        	TM_Task[i].pendingExecutions--;
        }
      }

	  Tick_IF = 0;
	}
  }

}
