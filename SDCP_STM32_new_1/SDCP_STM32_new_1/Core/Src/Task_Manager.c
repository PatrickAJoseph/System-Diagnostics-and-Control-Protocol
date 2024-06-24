
#include "Task_Manager.h"


/*
 * Function to add a task to the task dispatcher.
 *
 * */

void TM_addTask(uint8_t taskIndex, void* taskFunctionPtr, uint32_t taskTickInterval)
{
  TM_Task[taskIndex].fptr = taskFunctionPtr;
  TM_Task[taskIndex].interval = taskTickInterval;

  TM_Task[taskIndex].counter = 0;
  TM_Task[taskIndex].pendingExecutions = 0;
  TM_Task[taskIndex].runnable = 1;
}


/* Task dispatcher. */

void TM_dispatcher()
{
  /* Loop through available tasks. */

  for(int i = 0 ; i < TM_nTasks ; i++ )
  {
	/* Check if task is set as runnable. */

    if( TM_Task[i].runnable == 1 )
    {
      /* Increment task counter. */

      TM_Task[i].counter++;

      /* If task counter has reached task interval limit. */

      if( TM_Task[i].counter >= TM_Task[i].interval )
      {
    	  TM_Task[i].counter = 0; // Reset task counter.
    	  TM_Task[i].pendingExecutions++; // Increment pending executions.
      }
    }
    else /* If task is not set as runnable. */
    {
    	TM_Task[i].counter = 0;
    	TM_Task[i].pendingExecutions = 0;
    }
  }
}
