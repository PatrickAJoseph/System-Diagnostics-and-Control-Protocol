Function structure

#define PM_nFrames		  <number of frames>
#define PM_nTransmitEvents <number of transmit times> 

#include "SDCP.h"

uint8_t PM_SMF_Tx_buffer[ 6 * PM_nFrames ];
uint8_t PM_SMF_Transmit_Counter[ PM_nTransmitEvents ];
uint8_t PM_SMF_Transmit_Counter_Limits[ PM_nTransmitEvents ];
uint32_t PM_SMF_Transmit_Event;

uint32_t PM_pSMFs[PM_nFrames];
uint8_t PM_FIDs[PM_nFrames] = {<array of FIDs>};


void PM_Init()
{
  /* Initilaize pointers to message frame unions. */

  PM_pSMFs[0] = (uint32_t)&<name of frame 0>_SMF;
  PM_pSMFs[1] = (uint32_t)&<name of frame 1>_SMF;
  PM_pSMFs[2] = (uint32_t)&<name of frame 2>_SMF;
                     .
					 .
					 .
					 .
  PM_pSMFs[N-1] = (uint32_t)&<name of frame N-1>_SMF;
                   
  /* Application specific initialization go here. */
				   
}


static void PM_push_transmit_buffer(uint8_t* buffer, uint16_t startPosition)
{
  PM_SMF_Transmit_Buffer[startPosition+0] = buffer[0];
  PM_SMF_Transmit_Buffer[startPosition+1] = buffer[1];
  PM_SMF_Transmit_Buffer[startPosition+2] = buffer[2];
  PM_SMF_Transmit_Buffer[startPosition+3] = buffer[3];
  PM_SMF_Transmit_Buffer[startPosition+4] = buffer[4];
  PM_SMF_Transmit_Buffer[startPosition+5] = buffer[5];    
}

void PM_SMF_Transmit()
{
  /* Scheduler counter. */

  for( int i = 0 ; i < PM_nTransmitCounter ; i++ )
  {
    PM_SMF_Transmit_Counter[i]++;

    if(PM_SMF_Transmit_Counter[i] >= PM_SMF_Transmit_Counter_Limit[i] )
    {
      PM_SMF_Transmit_Counter[i] = 0;
	  PM_SMF_Transmit_Event |= (1 << i);
	}
  }

  
  /* Load transmit buffer. */

  uint8_t tempBuffer[6];   // Temporary buffer to frame values.
  uint16_t position = 0;

  /* Transmit event 0. */
  
  if( PM_SMF_Transmit_Event & ( 1 << 0 ) )
  {
    SDCP_encode( &<frame_0_of_event_0>_SMF, <frame_0_of_event_0>_FID, 0, buffer );
    PM_push_transmit_buffer(buffer,position); 
    position += 6;	

    SDCP_encode( &<frame_1_of_event_0>_SMF, <frame_1_of_event_0>_FID, 0, buffer );
    PM_push_transmit_buffer(buffer,position); 
    position += 6;	

    SDCP_encode( &<frame_2_of_event_0>_SMF, <frame_2_of_event_0>_FID, 0, buffer );
    PM_push_transmit_buffer(buffer,position); 
    position += 6;	
	
	.
	.
	.
	.
	.
	
    SDCP_encode( &<frame_n_of_event_0>_SMF, <frame_n_of_event_0>_FID, 0, buffer );
    PM_push_transmit_buffer(buffer,position); 
    position += 6;

    PM_SMF_Transmit_Event &= ( ~( 1 << 0 ) );	
  }
  
  /* Transmit event N. */
  
  if( PM_SMF_Transmit_Event & ( 1 << N ) )
  {
    SDCP_encode( &<frame_0_of_event_N>_SMF, <frame_0_of_event_N>_FID, 0, buffer );
    PM_push_transmit_buffer(buffer,position); 
    position += 6;	

    SDCP_encode( &<frame_1_of_event_N>_SMF, <frame_1_of_event_N>_FID, 0, buffer );
    PM_push_transmit_buffer(buffer,position); 
    position += 6;	

    SDCP_encode( &<frame_2_of_event_N>_SMF, <frame_2_of_event_N>_FID, 0, buffer );
    PM_push_transmit_buffer(buffer,position); 
    position += 6;	
	
	.
	.
	.
	.
	.
	
    SDCP_encode( &<frame_n_of_event_N>_SMF, <frame_n_of_event_N>_FID, 0, buffer );
    PM_push_transmit_buffer(buffer,position); 
    position += 6;	

    PM_SMF_Transmit_Event &= ( ~( 1 << N ) );	
  }
  
}
 