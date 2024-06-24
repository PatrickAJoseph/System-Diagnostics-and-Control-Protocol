
#include "SDCP.h"

#include <stdint.h>
#include <stdio.h>

int8_t SDCP_TXQ_front = - 1;		//	SDCP transmit circular queue front.
int8_t SDCP_TXQ_back = -1;			//	SDCP transmit circular queue back.
int8_t SDCP_RXQ_front = - 1;    	//  SDCP receive circular queue front.
int8_t SDCP_RXQ_back = -1;     		//  SDCP receive circular queue back.

/* Function to encode a message frame. */

void SDCP_encode(void* frame, uint8_t frameID, uint8_t priority, uint8_t* buffer){

  /* Obtain data from the frame structure. */

  uint32_t data = (*((__DF*)frame)).__data;

  /* The most significant byte contains the frame ID along with the priority. */

  buffer[0] = ( (priority << 7) + frameID );

  /* The next four bytes contain data. */

  buffer[1] = (uint8_t)( (data & 0xFF000000UL) >> 24 );
  buffer[2] = (uint8_t)( (data & 0x00FF0000UL) >> 16 );
  buffer[3] = (uint8_t)( (data & 0x0000FF00UL) >> 8 );
  buffer[4] = (uint8_t)( (data & 0x000000FFUL) >> 0 );

  /* Calculate 8-bit checksum and load it into the last byte of the data frame. */

  buffer[5] = (uint8_t)(((uint16_t)buffer[0] + (uint16_t)buffer[1] + (uint16_t)buffer[2] + (uint16_t)buffer[3] + (uint16_t)buffer[4])%255);
  buffer[5] = ~buffer[5];
}

/* Function to decode a message frame. */

uint8_t SDCP_decode( uint8_t* buffer, uint8_t frameID, void* frame ){

  /* Check if the frame ID of the received message matches with the expected frame to be decoded. */

  if( ( buffer[0] & 0x7F ) == frameID ){

    /* Extract the data bytes from the buffer. */

    uint32_t frameData = ( ( (uint32_t)(buffer[1]) << 24 )
	                   + ( (uint32_t)(buffer[2]) << 16 )
                       + ( (uint32_t)(buffer[3]) << 8 )
                       + ( (uint32_t)(buffer[4]) << 0 ) );

    /* Load data into the union. */

    (*((__DF*)frame)).__data = frameData;

    return(1);
  }

	return(0);
}

/***** Function to check if checksum of received data is fine. *****/

uint8_t SDCP_checkCKSUM(uint8_t* buffer){

  uint8_t checksum = (uint8_t)(((uint16_t)buffer[0] + (uint16_t)buffer[1] + (uint16_t)buffer[2] + (uint16_t)buffer[3]+ (uint16_t)buffer[4] + (uint16_t)buffer[5])%255);

  return( (checksum == 0) );
}


/* Function to load an encoded data frame into the transmit circular queue. */

uint8_t SDCP_TXNQ( uint8_t* buffer ){

  /* Check priority of the incoming 6-byte encoded frame. */

  if( buffer[0] & 0x80 ){

	/* If priority of the message frame is high, load encoded message frame
	   at the back of the queue. */

	/* Check if queue is full. This is done by checking if the rear is about
	   to exceed the front. */

	int8_t nextFront;

	nextFront = SDCP_TXQ_front - 1;

	if(nextFront <= -1){
	  nextFront = SDCP_TXQ_SIZE - 1;
	}

	if( ( ( SDCP_TXQ_front == 0 ) && ( SDCP_TXQ_back == ( SDCP_TXQ_SIZE - 1 ) ) )
		 || ( ( nextFront % SDCP_TXQ_SIZE ) == SDCP_TXQ_back ) ){

        return(0);

	}
    else if(SDCP_TXQ_front == -1){    /* If this is the first message frame loaded into the queue. */

        SDCP_TXQ_front = SDCP_TXQ_back = 0;

    }
	else if( (SDCP_TXQ_front - 1) == - 1 ){  /* If rear is about to warp to start. */

		printf("\nTXQ: Queue on front wrap.\n");

        SDCP_TXQ_front = SDCP_TXQ_SIZE - 1;
	}
    else{  /* If the queue is NOT full and element is NOT the first element in queue AND the queue is NOT warping around. */

        SDCP_TXQ_front--;

    }

	/* Load message frame into queue. */

	SDCP_TXQ[6*SDCP_TXQ_front+0] = 	buffer[0];
	SDCP_TXQ[6*SDCP_TXQ_front+1] = 	buffer[1];
	SDCP_TXQ[6*SDCP_TXQ_front+2] = 	buffer[2];
	SDCP_TXQ[6*SDCP_TXQ_front+3] = 	buffer[3];
    SDCP_TXQ[6*SDCP_TXQ_front+4] = 	buffer[4];
    SDCP_TXQ[6*SDCP_TXQ_front+5] = 	buffer[5];

  }
  else{

	/* If priority of the message frame is low, load encoded message frame
	   at the back of the queue. */

	/* Check if queue is full. This is done by checking if the rear is about
	   to exceed the front. */

	if( ( ( SDCP_TXQ_front == 0 ) && ( SDCP_TXQ_back == ( SDCP_TXQ_SIZE - 1 ) ) )
		 || ( ( ( SDCP_TXQ_back + 1 ) % SDCP_TXQ_SIZE ) == SDCP_TXQ_front ) ){

        return(0);

	}
    else if(SDCP_TXQ_front == -1){    /* If this is the first messag frame loaded into the queue. */

        SDCP_TXQ_front = SDCP_TXQ_back = 0;

    }
	else if( (SDCP_TXQ_back == (SDCP_TXQ_SIZE-1)) && ( SDCP_TXQ_front != 0 ) ){  /* If rear is about to warp to start. */

        SDCP_TXQ_back = 0;
	}
    else{  /* If the queue is NOT full and element is NOT the first element in queue AND the queue is NOT warping around. */

        SDCP_TXQ_back++;

    }

	/* Load message frame into queue. */

	SDCP_TXQ[6*SDCP_TXQ_back+0] = 	buffer[0];
	SDCP_TXQ[6*SDCP_TXQ_back+1] = 	buffer[1];
	SDCP_TXQ[6*SDCP_TXQ_back+2] = 	buffer[2];
	SDCP_TXQ[6*SDCP_TXQ_back+3] = 	buffer[3];
    SDCP_TXQ[6*SDCP_TXQ_back+4] = 	buffer[4];
    SDCP_TXQ[6*SDCP_TXQ_back+5] = 	buffer[5];

  }


  return(1);
}




/* Function to dequeue an encoded data frame from the transmit circular queue. */

uint8_t SDCP_TXDQ( uint8_t* buffer ){

   /* If the queue was not initialized, do nothing. */

   if( SDCP_TXQ_front == - 1 ){

		return(0);

   }

   /* Retrive data from queue. */

   buffer[0] = SDCP_TXQ[6*SDCP_TXQ_front+0];
   buffer[1] = SDCP_TXQ[6*SDCP_TXQ_front+1];
   buffer[2] = SDCP_TXQ[6*SDCP_TXQ_front+2];
   buffer[3] = SDCP_TXQ[6*SDCP_TXQ_front+3];
   buffer[4] = SDCP_TXQ[6*SDCP_TXQ_front+4];
   buffer[5] = SDCP_TXQ[6*SDCP_TXQ_front+5];

   /* Replace dequeues slots with zeroes. */

   SDCP_TXQ[6*SDCP_TXQ_front+0] = 0;
   SDCP_TXQ[6*SDCP_TXQ_front+1] = 0;
   SDCP_TXQ[6*SDCP_TXQ_front+2] = 0;
   SDCP_TXQ[6*SDCP_TXQ_front+3] = 0;
   SDCP_TXQ[6*SDCP_TXQ_front+4] = 0;
   SDCP_TXQ[6*SDCP_TXQ_front+5] = 0;

   /* If queue overflows, set queue front and queue back to -1. */

   if( SDCP_TXQ_front == SDCP_TXQ_back ){

	  SDCP_TXQ_front = SDCP_TXQ_back = -1;

   }else if( SDCP_TXQ_front == (SDCP_TXQ_SIZE - 1) ){  /* If front position exceeds queue size, go to start of queue.*/

      SDCP_TXQ_front = 0;

   }else{

      SDCP_TXQ_front++;

   }

    return(1);
}




/* Function to load an encoded data frame into the receive circular queue. */

uint8_t SDCP_RXNQ( uint8_t* buffer ){

  /* Check priority of the incoming 6-byte encoded frame. */

  if( buffer[0] & 0x80 ){

	/* If priority of the message frame is high, load encoded message frame
	   at the back of the queue. */

	/* Check if queue is full. This is done by checking if the rear is about
	   to exceed the front. */

	int8_t nextFront;

	nextFront = SDCP_RXQ_front - 1;

	if(nextFront <= -1){
	  nextFront = SDCP_RXQ_SIZE - 1;
	}

	if( ( ( SDCP_RXQ_front == 0 ) && ( SDCP_RXQ_back == ( SDCP_RXQ_SIZE - 1 ) ) )
		 || ( ( nextFront % SDCP_RXQ_SIZE ) == SDCP_RXQ_back ) ){

        return(0);

	}
    else if(SDCP_RXQ_front == -1){    /* If this is the first message frame loaded into the queue. */

        SDCP_RXQ_front = SDCP_RXQ_back = 0;

    }
	else if( (SDCP_RXQ_front - 1) == - 1 ){  /* If rear is about to warp to start. */

		printf("\nRXQ: Queue on front wrap.\n");

        SDCP_RXQ_front = SDCP_RXQ_SIZE - 1;
	}
    else{  /* If the queue is NOT full and element is NOT the first element in queue AND the queue is NOT warping around. */

        SDCP_RXQ_front--;

    }

	/* Load message frame into queue. */

	SDCP_RXQ[6*SDCP_RXQ_front+0] = 	buffer[0];
	SDCP_RXQ[6*SDCP_RXQ_front+1] = 	buffer[1];
	SDCP_RXQ[6*SDCP_RXQ_front+2] = 	buffer[2];
	SDCP_RXQ[6*SDCP_RXQ_front+3] = 	buffer[3];
    SDCP_RXQ[6*SDCP_RXQ_front+4] = 	buffer[4];
    SDCP_RXQ[6*SDCP_RXQ_front+5] = 	buffer[5];

  }
  else{

	/* If priority of the message frame is low, load encoded message frame
	   at the back of the queue. */

	/* Check if queue is full. This is done by checking if the rear is about
	   to exceed the front. */

	if( ( ( SDCP_RXQ_front == 0 ) && ( SDCP_RXQ_back == ( SDCP_RXQ_SIZE - 1 ) ) )
		 || ( ( ( SDCP_RXQ_back + 1 ) % SDCP_RXQ_SIZE ) == SDCP_RXQ_front ) ){

        return(0);

	}
    else if(SDCP_RXQ_front == -1){    /* If this is the first messag frame loaded into the queue. */

        SDCP_RXQ_front = SDCP_RXQ_back = 0;

    }
	else if( (SDCP_RXQ_back == (SDCP_RXQ_SIZE-1)) && ( SDCP_RXQ_front != 0 ) ){  /* If rear is about to warp to start. */

        SDCP_RXQ_back = 0;
	}
    else{  /* If the queue is NOT full and element is NOT the first element in queue AND the queue is NOT warping around. */

        SDCP_RXQ_back++;

    }

	/* Load message frame into queue. */

	SDCP_RXQ[6*SDCP_RXQ_back+0] = 	buffer[0];
	SDCP_RXQ[6*SDCP_RXQ_back+1] = 	buffer[1];
	SDCP_RXQ[6*SDCP_RXQ_back+2] = 	buffer[2];
	SDCP_RXQ[6*SDCP_RXQ_back+3] = 	buffer[3];
    SDCP_RXQ[6*SDCP_RXQ_back+4] = 	buffer[4];
    SDCP_RXQ[6*SDCP_RXQ_back+5] = 	buffer[5];

  }


  return(1);
}




/* Function to dequeue an encoded data frame from the transmit circular queue. */

uint8_t SDCP_RXDQ( uint8_t* buffer ){

   /* If the queue was not initialized, do nothing. */

   if( SDCP_RXQ_front == - 1 ){

		return(0);

   }

   /* Retrive data from queue. */

   buffer[0] = SDCP_RXQ[6*SDCP_RXQ_front+0];
   buffer[1] = SDCP_RXQ[6*SDCP_RXQ_front+1];
   buffer[2] = SDCP_RXQ[6*SDCP_RXQ_front+2];
   buffer[3] = SDCP_RXQ[6*SDCP_RXQ_front+3];
   buffer[4] = SDCP_RXQ[6*SDCP_RXQ_front+4];
   buffer[5] = SDCP_RXQ[6*SDCP_RXQ_front+5];

    /* Delete dequeued element. */

	SDCP_RXQ[6*SDCP_RXQ_front+0] = 	0;
	SDCP_RXQ[6*SDCP_RXQ_front+1] = 	0;
	SDCP_RXQ[6*SDCP_RXQ_front+2] = 	0;
	SDCP_RXQ[6*SDCP_RXQ_front+3] = 	0;
    SDCP_RXQ[6*SDCP_RXQ_front+4] = 	0;
    SDCP_RXQ[6*SDCP_RXQ_front+5] = 	0;

   /* If queue overflows, set queue front and queue back to -1. */

   if( SDCP_RXQ_front == SDCP_RXQ_back ){

	  SDCP_RXQ_front = SDCP_RXQ_back = -1;

   }else if( SDCP_RXQ_front == (SDCP_RXQ_SIZE - 1) ){  /* If front position exceeds queue size, go to start of queue.*/

      SDCP_RXQ_front = 0;

   }else{

      SDCP_RXQ_front++;

   }

   return(1);
}


/* Function to evaluate the block transfer FSM. */

void SDCP_BTR_FSM_update(){

  /* Check if block transfer mode is enabled. */

  if( TMC.TMC_segments.BTR_ENSTAT )
  {
	/* Check if transfer counter of block transfer mode has not overflowed. */

    if( SDCP_BTR_XCNTR < BTR1.BTR1_segments.NTR )
    {
      /* Check if parameter counter has not overflowed. */

      if( SDCP_BTR_PCNTR < BTR1.BTR1_segments.NPAR )
      {
        SDCP_BTR_currentPID = SDCP_BTR_PID[SDCP_BTR_PCNTR];
    	SDCP_BTR_PCNTR++;
      }
      else /* If parameter counter has overflowed. */
      {
    	SDCP_BTR_currentPID = SDCP_BTR_PID[0];
    	SDCP_BTR_PCNTR = 0; // Reset parameter counter.
    	SDCP_BTR_XCNTR++;   // Increment parameter counter.
      }
    }
    else /* If transfer counter has overflowed. */
    {
      SDCP_BTR_PCNTR = 0;
      SDCP_BTR_XCNTR = 0;

      TMC.TMC_segments.SFT_ENSTAT = 1;
      TMC.TMC_segments.FTOR_ENSTAT = 0;
      TMC.TMC_segments.BTR_ENSTAT = 0;
    }
  }
}




union IEEE754
{
  float     value;
  uint32_t  raw;
}IEEE754;


/* Function to write a parameter into a frame. */

void SDCP_WPF(SDCP_Param* parameter)
{
  /* Get instance of frame. */

  __DF frame = *( (__DF*)parameter->pFrame );

  /* Porting to local variable. */

  uint32_t  data       =  frame.__data;
  uint32_t  dataType   =  parameter->dataType;
  float     factor     =  parameter->factor;
  float 	offset     =  parameter->offset;
  float     value      =  parameter->value;
  uint32_t  bitLength  =  parameter->bitLength;
  uint32_t  startBit   =  parameter->startBit;

  /* Clear the bits in the frame associated with the current parameter. */

  uint32_t mask = 0;

  if( bitLength < 32 )
  {
    mask = ( ( 1UL << bitLength ) - 1 );
  }
  else
  {
    mask = 0xFFFFFFFFUL;
  }

  data &= ~( mask << startBit );

  /* Frame value = (to unsigned int){ ( value - offset ) / factor }. */

  if( factor != 0.0f )
  {
    value = ( ( value - offset ) / factor );
  }
  else
  {
    value = 0.0f;
  }

  /* Process based on data type. */

  uint32_t tempVal = 0;

  switch(dataType)
  {
    case(UNSIGNED):
	{
      tempVal = ( ((uint32_t)value) & mask );
    }
    break;

    case(SIGNED):
	{
      uint8_t sign = 0;	// Sign of 'value'. sign 1 --> negative, sign 0 --> Positive.

      /* Calculate absolute value. */

      if( value < 0.0f )
      {
        value = -value;
        sign = 1;
      }

      /* If sign is negative. */

      if( sign )
      {
    	if( bitLength < 32 )
    	{
          tempVal = ( ( 1UL << bitLength ) - ( (uint32_t)value & mask ) );
    	}
    	else
    	{
    	  tempVal = (int32_t)value;
    	}
      }
      else /* If sign is positive. */
      {
        tempVal = (uint32_t)value & mask;
      }
	}
    break;

    case(FLOAT):
	{
      IEEE754.value = value;
      tempVal = IEEE754.raw;
	}
    break;
  }

  /* Merge modified bits with current bits. */

  data |= ( tempVal << startBit );

  /* Store modified bits in frame. */

  (*((__DF*)(parameter->pFrame))).__data = data;
}

/* Function to read a parameter from a frame. */

void SDCP_RPF(SDCP_Param* parameter)
{
  /* Get instance of frame. */

  __DF frame = *( (__DF*)parameter->pFrame );

  /* Porting to local variable. */

  uint32_t  data       =  frame.__data;
  uint32_t  dataType   =  parameter->dataType;
  float     factor     =  parameter->factor;
  float 	offset     =  parameter->offset;
  float     value      =  0.0f;
  uint32_t  bitLength  =  parameter->bitLength;
  uint32_t  startBit   =  parameter->startBit;

  uint32_t rawValue = 0;
  uint32_t mask = 0;

  if( bitLength < 32 )
  {
    mask = (( 1UL << bitLength ) - 1);
  }
  else
  {
    mask = 0xFFFFFFFFUL;
  }

  rawValue = ( (data >> startBit) & mask );

  switch(dataType)
  {
    case(UNSIGNED):
	{
      value = ( ( factor * (float)rawValue ) + offset );
	}
    break;

    case(SIGNED):
	{
      uint32_t maxAbsValue = (( 1UL << (bitLength - 1) ) - 1);

      if( rawValue > maxAbsValue ) // When value of number > 2*(N_bits-1) value is -ve.
      {
        if( bitLength < 32 )
        {
          value = ( offset - factor * (float)( ( 1UL << bitLength ) - rawValue ) );
        }
        else
        {
          value = ( factor * (float)( (int32_t)rawValue ) + offset );
        }
      }
      else /* If number is positive. */
      {
        value = ( factor * (float)rawValue + offset );
      }
	}
    break;


    case(FLOAT):
	{
      IEEE754.raw = rawValue;
      value = (( factor * (float)IEEE754.value ) + offset);
	}
    break;
  }

  parameter->value = value;
}
