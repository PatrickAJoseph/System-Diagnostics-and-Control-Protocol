

#ifndef SDCP_H
#define SDCP_H

#include <stdint.h>

/* Typical parameter data types. */

#define UNSIGNED 0
#define SIGNED   1
#define FLOAT    2

/* Typedef for a SDCP frame parameter. */

typedef struct SDCP_Param
{
  void*    pFrame;   		/* Pointer to owning frame. */
  uint32_t dataType;		/* Parameter data type. */
  uint32_t startBit;		/* Start bit. */
  uint32_t bitLength;		/* Bit length. */
  float    factor;			/* Factor. */
  float    offset;			/* Offset. */
  float    value;           /* This is actual value of the parameter. */
}SDCP_Param;

/* System control and diagonostics protocol. */


#define SDCP_TXQ_SIZE				10
#define SDCP_RXQ_SIZE				10

#define SDCP_QS_OK					0
#define SDCP_QS_FULL				1
#define SDCP_QS_EMPTY				2

#define SDCP_BTR_buffer_size		100

uint8_t SDCP_TXQ[ 6 * SDCP_TXQ_SIZE ];	//	SDCP transmit circular queue.
uint8_t SDCP_RXQ[ 6 * SDCP_RXQ_SIZE ];	//	SDCP receive circular queue.

extern int8_t SDCP_TXQ_front;		//	SDCP transmit circular queue front.
extern int8_t SDCP_TXQ_back;		//	SDCP transmit circular queue back.
extern int8_t SDCP_RXQ_front;    	//  SDCP receive circular queue front.
extern int8_t SDCP_RXQ_back;     	//  SDCP receive circular queue back.

/* Variables for controlling block transfer FSM. */

uint8_t SDCP_BTR_PCNTR;			//	Block transfer: parameter counter.
uint16_t SDCP_BTR_XCNTR;        //  Block transfer: transfer counter.
uint8_t SDCP_BTR_currentPID;	//	Block transfer: current PID.

uint8_t SDCP_BTR_PID[9];			//	Block transfer: Parameters IDs to transfer.

/* Frame transfer on request frame structures. */

union FTOR1_struct{

  uint32_t all;

  struct FTOR1_segments{

  uint32_t FID_2 : 8;  //	Frame control ID2.
  uint32_t FID_1 : 8;  //	Frame control ID1.
  uint32_t FID_0 : 8;  //	Frame control ID0.
  uint32_t NF: 6;      //	Frame transfer on request ACK bit.
  uint32_t REQ: 1;     //	Frame transfer on request post request bit.
  uint32_t ACK: 1;     //	Frame transfer on request acknowledgement.

  }FTOR1_segments;

}FTOR1;

union FTOR2_struct{

  uint32_t all;

  struct FTOR2_segments{

    uint32_t FID_6 :8;		//	Frame control ID6.
    uint32_t FID_5 :8;		//	Frame control ID5.
    uint32_t FID_4 :8;		//	Frame control ID4.
    uint32_t FID_3 :8;  	//	Frame control ID3.

  }FTOR2_segments;

}FTOR2;


/* Block transfer control frame structure. */

union BTR1_struct{

  uint32_t all;

  struct BTR1_segments{

    uint32_t PID_0 : 8;        // Block transfer - parameter ID.
	uint32_t NTR   : 16;       // Block transfer - number of transfers.
	uint32_t NPAR  : 6;        // Block transfer - number of parameters.
 	uint32_t ACK   : 1;        // Block transfer - acknowledge.
    uint32_t REQ   : 1;        // Block transfer - request.

  }BTR1_segments;

}BTR1;

union BTR2_struct{

  uint32_t all;

  struct BTR2_segments{

    uint32_t PID_4 :8;		//	Parameter ID 4.
    uint32_t PID_3 :8;		//	Parameter ID 3.
    uint32_t PID_2 :8;		//	Parameter ID 2.
    uint32_t PID_1 :8;  	//	Parameter ID 1.

  }BTR2_segments;

}BTR2;

union BTR3_struct{

  uint32_t all;

  struct BTR3_segments{

    uint32_t PID_8 :8;		//	Parameter ID 8.
    uint32_t PID_7 :8;		//	Parameter ID 7.
    uint32_t PID_6 :8;		//	Parameter ID 6.
    uint32_t PID_5 :8;  	//	Parameter ID 5.

  }BTR3_segments;

}BTR3;


/* Structure for transfer mode control frame. */

union TMC_union{

  uint32_t all;

  struct TMC_segments{

    uint32_t MTEN_REQ :8;		//	Message transfer enabled request.
	uint32_t MTEN_ACK:8;		//	Message transfer enabled acknowledgement.

	uint32_t SFT_ENREQ:1;		//	Standard frame transfer enable request.
	uint32_t FTOR_ENREQ:1;		//	Frame transfer on request mode enable request.
	uint32_t BTR_ENREQ:1;		//	Block transfer enable request.
    uint32_t SFT_ENSTAT:1;		//	Standard frame transfer enabled status.
	uint32_t FTOR_ENSTAT:1;		//	Frame transfer on request mode enabled status.
	uint32_t BTR_ENSTAT:3;		//	Block transfer enabled status.

	uint32_t TMC_ACK:1;			//	Transfer mode change acknowledgement.
	uint32_t TMC_REQ:1;			//	Transfer mode change request.
	uint32_t TMS_ACK:1;			//	Transfer mode status request acknowledgement.
	uint32_t TMS_REQ:5;			//	Transfer mode status request.

  }TMC_segments;

}TMC;

/* Union typedef to typecast any union into a uint32_t integer. */

typedef union __DF{ uint32_t __data; }__DF;

/************	Header file functions.  ******************/

/*
*  @brief Function to encode a standard message frame (SMF).
*
*  This function takes in a pointer to a union containing data for a message frame, the frame ID
*  and the priority and encodes the frame data into a 6 byte array.
*
*  @parameter frame:	Pointer to a union containing frame data.
*  @parameter frameID:  An 8-bit identifier ranging from 0 to 127 when priority = 0
*                       and 0 to 119 when priority = 1.
*  @parameter priority: Priority of message as seen by the receiver. When priority is 0,
*                       the received frame is loaded at the end of the receivers Rx circular
*                       queue and if priority is 1, the received frame is loaded at the
*                       front of the receivers Rx circular queue.
*  @parameter buffer:   A 6-byte array of type uint8_t to load the encoded data into.
*
*/

void SDCP_encode(void* frame, uint8_t frameID, uint8_t priority, uint8_t* buffer);

/*
*  @brief Function to decode a message frame.
*
*  This function takes in the 6-byte array containing the encoded message frame
*  and the frame ID and decodes the received and encoded message frame.
*
*  @priority buffer:    A 6-byte array of uint8_t type containing the received/encoded message.
*  @priority frameID:   The frame ID of the frame indented to be decoded.
*  @priority frame:     Pointer to a union containing frame data.
*
*  @return Result whether decoding is successful (1) or not (0).
*
*/

/* NOTE: This function can be implemented by the user in the calling functions itself.
*        Use this functions to either improve program execution speed or optimize
*		 application size.
*/

uint8_t SDCP_decode( uint8_t* buffer, uint8_t frameID, void* frame );

/*
*
*  @brief Function to check if the checksum of received data is fine.
*
*  This function takes in the received frame and checks if checksum is fine.
*
*  @parameter buffer:   A 6-byte array of uint8_t type containing the received/encoded message.
*
*  @return Checksum verification result.
*
*/

uint8_t SDCP_checkCKSUM(uint8_t* buffer);

/*
*  @brief Function to load encoded frame into the transmit circular queue.
*
*  This function loads an encoded data into the transmit circular queue. The
*  priority of the encoded data frame is determined by the MSB bit of the MSB byte
*  of the encoded data frame.
*
*  @parameter frame	:	Pointer to a 6-byte uint8_t array containing the encoded data frame.
*
*  @return 0 of the queue is full, 1 otherwise.
*
*/

uint8_t SDCP_TXNQ( uint8_t* buffer );

/*
*  @brief Function to dequeue encoded frame from the transmit circular queue.
*
*  This function decodes an encoded data frame from the transmit circular queue.
*
*  @parameter frame	:	Pointer to a 6-byte uint8_t array containing the encoded data frame.
*
*  @return 0 of the queue is empty, 1 otherwise.
*
*/

uint8_t SDCP_TXDQ( uint8_t* buffer );

/*
*  @brief Function to load encoded frame into the receive circular queue.
*
*  This function loads an encoded data into the receive circular queue. The
*  priority of the encoded data frame is determined by the MSB bit of the MSB byte
*  of the encoded data frame.
*
*  @parameter frame	:	Pointer to a 6-byte uint8_t array containing the encoded data frame.
*
*  @return 0 of the queue is full, 1 otherwise.
*
*/

uint8_t SDCP_RXNQ( uint8_t* buffer );

/*
*  @brief Function to dequeue encoded frame from the receive circular queue.
*
*  This function decodes an encoded data frame from the receive circular queue.
*
*  @parameter frame	:	Pointer to a 6-byte uint8_t array containing the encoded data frame.
*
*  @return 0 of the queue is empty, 1 otherwise.
*
*/

uint8_t SDCP_RXDQ( uint8_t* buffer );

/*
 * @brief Function to handle block transfer events.
 *
 * */

void SDCP_BTR_FSM_update();

/*
 * @brief Function to write a parameter into a frame.
 *
 * */

void SDCP_WPF(SDCP_Param* parameter);

/*
 * @brief Function to read a parameter from a frame.
 *
 * */

void SDCP_RPF(SDCP_Param* parameter);


#endif /* SDCP_H */
