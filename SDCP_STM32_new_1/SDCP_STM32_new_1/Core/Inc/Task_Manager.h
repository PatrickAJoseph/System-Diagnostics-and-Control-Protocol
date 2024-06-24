
#ifndef INC_TASK_MANAGER_H_
#define INC_TASK_MANAGER_H_

#include "main.h"

#define Task_Tick_Time_ms	10
#define TM_nTasks		    5


/* Structure for task. */

typedef struct Task
{
  uint32_t pendingExecutions;
  uint32_t runnable;
  uint32_t counter;
  uint32_t interval;
  void    (*fptr)();

}Task;


Task TM_Task[TM_nTasks];   // Tasks.

void TM_addTask(uint8_t taskIndex, void* taskFunctionPtr, uint32_t taskTickInterval);
void TM_dispatcher();

#endif /* INC_TASK_MANAGER_H_ */
