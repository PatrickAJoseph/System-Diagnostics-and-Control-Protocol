#define PM_nFrames    28
#define PM_nTransmitEvents 5

uint8_t PM_SMF_Tx_buffer[ 6 * PM_nFrames ];
uint8_t PM_SMF_Transmit_Counter[ PM_nTransmitEvents ];
uint32_t PM_SMF_Transmit_Event;
uint8_t PM_SMF_Transmit_Counter_Limits[ PM_nTransmitEvents ] = { 10,1,25,1,5 };
uint32_t pSMFs[PM_nFrames];




uint32_t PM_FIDs[PM_nFrames] = {

  GUI_FID,
  LED_CNTRL_FID,
  IMU_X_Raw_FID,
  IMU_Y_Raw_FID,
  IMU_Z_Raw_FID,
  IMU_Temp_FID,
  IMU_XACCEL_Processed_FID,
  IMU_YACCEL_Processed_FID,
  IMU_ZACCEL_Processed_FID,
  IMU_XGYRO_Processed_FID,
  IMU_YGYRO_Processed_FID,
  IMU_ZGYRO_Processed_FID,
  Roll_FID,
  Pitch_FID,
  Yaw_FID,
  CF_coeff_FID,
  IMU_settings_FID,
  IMU_CALIB_XACCEL_Offset_FID,
  IMU_CALIB_YACCEL_Offset_FID,
  IMU_CALIB_ZACCEL_Offset_FID,
  IMU_CALIB_XGYRO_Offset_FID,
  IMU_CALIB_YGYRO_Offset_FID,
  IMU_CALIB_ZGYRO_Offset_FID,
  IMU_CALIB_XGYRO_bias_drift_FID,
  IMU_CALIB_YGYRO_bias_drift_FID,
  IMU_CALIB_ZGYRO_bias_drift_FID,
  Task_counter_FID,
  END_FID

};



void PM_Init(){

  pSMFs[0] = (uint32_t)&GUI_SMF;
  pSMFs[1] = (uint32_t)&LED_CNTRL_SMF;
  pSMFs[2] = (uint32_t)&IMU_X_Raw_SMF;
  pSMFs[3] = (uint32_t)&IMU_Y_Raw_SMF;
  pSMFs[4] = (uint32_t)&IMU_Z_Raw_SMF;
  pSMFs[5] = (uint32_t)&IMU_Temp_SMF;
  pSMFs[6] = (uint32_t)&IMU_XACCEL_Processed_SMF;
  pSMFs[7] = (uint32_t)&IMU_YACCEL_Processed_SMF;
  pSMFs[8] = (uint32_t)&IMU_ZACCEL_Processed_SMF;
  pSMFs[9] = (uint32_t)&IMU_XGYRO_Processed_SMF;
  pSMFs[10] = (uint32_t)&IMU_YGYRO_Processed_SMF;
  pSMFs[11] = (uint32_t)&IMU_ZGYRO_Processed_SMF;
  pSMFs[12] = (uint32_t)&Roll_SMF;
  pSMFs[13] = (uint32_t)&Pitch_SMF;
  pSMFs[14] = (uint32_t)&Yaw_SMF;
  pSMFs[15] = (uint32_t)&CF_coeff_SMF;
  pSMFs[16] = (uint32_t)&IMU_settings_SMF;
  pSMFs[17] = (uint32_t)&IMU_CALIB_XACCEL_Offset_SMF;
  pSMFs[18] = (uint32_t)&IMU_CALIB_YACCEL_Offset_SMF;
  pSMFs[19] = (uint32_t)&IMU_CALIB_ZACCEL_Offset_SMF;
  pSMFs[20] = (uint32_t)&IMU_CALIB_XGYRO_Offset_SMF;
  pSMFs[21] = (uint32_t)&IMU_CALIB_YGYRO_Offset_SMF;
  pSMFs[22] = (uint32_t)&IMU_CALIB_ZGYRO_Offset_SMF;
  pSMFs[23] = (uint32_t)&IMU_CALIB_XGYRO_bias_drift_SMF;
  pSMFs[24] = (uint32_t)&IMU_CALIB_YGYRO_bias_drift_SMF;
  pSMFs[25] = (uint32_t)&IMU_CALIB_ZGYRO_bias_drift_SMF;
  pSMFs[26] = (uint32_t)&Task_counter_SMF;
  pSMFs[27] = (uint32_t)&END_SMF;



//  Application specific initializations for standard message frames
//  should be made after this section.

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


  /*  Transmit event 0 */                                    

  if( PM_SMF_Transmit_Event & ( 1 << 0 ) ){  

    SDCP_encode(&GUI_SMF,GUI_FID,0,buffer);
    PM_push_transmit_buffer(buffer,position);
    position += 6;


    SDCP_encode(&LED_CNTRL_SMF,LED_CNTRL_FID,0,buffer);
    PM_push_transmit_buffer(buffer,position);
    position += 6;
    }

  /*  Transmit event 1 */                                    

  if( PM_SMF_Transmit_Event & ( 1 << 1 ) ){  

    SDCP_encode(&IMU_X_Raw_SMF,IMU_X_Raw_FID,0,buffer);
    PM_push_transmit_buffer(buffer,position);
    position += 6;


    SDCP_encode(&IMU_Y_Raw_SMF,IMU_Y_Raw_FID,0,buffer);
    PM_push_transmit_buffer(buffer,position);
    position += 6;


    SDCP_encode(&IMU_Z_Raw_SMF,IMU_Z_Raw_FID,0,buffer);
    PM_push_transmit_buffer(buffer,position);
    position += 6;


    SDCP_encode(&IMU_XACCEL_Processed_SMF,IMU_XACCEL_Processed_FID,0,buffer);
    PM_push_transmit_buffer(buffer,position);
    position += 6;


    SDCP_encode(&IMU_YACCEL_Processed_SMF,IMU_YACCEL_Processed_FID,0,buffer);
    PM_push_transmit_buffer(buffer,position);
    position += 6;


    SDCP_encode(&IMU_ZACCEL_Processed_SMF,IMU_ZACCEL_Processed_FID,0,buffer);
    PM_push_transmit_buffer(buffer,position);
    position += 6;


    SDCP_encode(&IMU_XGYRO_Processed_SMF,IMU_XGYRO_Processed_FID,0,buffer);
    PM_push_transmit_buffer(buffer,position);
    position += 6;


    SDCP_encode(&IMU_YGYRO_Processed_SMF,IMU_YGYRO_Processed_FID,0,buffer);
    PM_push_transmit_buffer(buffer,position);
    position += 6;


    SDCP_encode(&IMU_ZGYRO_Processed_SMF,IMU_ZGYRO_Processed_FID,0,buffer);
    PM_push_transmit_buffer(buffer,position);
    position += 6;
    }

  /*  Transmit event 2 */                                    

  if( PM_SMF_Transmit_Event & ( 1 << 2 ) ){  

    SDCP_encode(&IMU_Temp_SMF,IMU_Temp_FID,0,buffer);
    PM_push_transmit_buffer(buffer,position);
    position += 6;
    }

  /*  Transmit event 3 */                                    

  if( PM_SMF_Transmit_Event & ( 1 << 3 ) ){  

    SDCP_encode(&IMU_X_Raw_SMF,IMU_X_Raw_FID,0,buffer);
    PM_push_transmit_buffer(buffer,position);
    position += 6;


    SDCP_encode(&IMU_Y_Raw_SMF,IMU_Y_Raw_FID,0,buffer);
    PM_push_transmit_buffer(buffer,position);
    position += 6;


    SDCP_encode(&IMU_Z_Raw_SMF,IMU_Z_Raw_FID,0,buffer);
    PM_push_transmit_buffer(buffer,position);
    position += 6;


    SDCP_encode(&IMU_XACCEL_Processed_SMF,IMU_XACCEL_Processed_FID,0,buffer);
    PM_push_transmit_buffer(buffer,position);
    position += 6;


    SDCP_encode(&IMU_YACCEL_Processed_SMF,IMU_YACCEL_Processed_FID,0,buffer);
    PM_push_transmit_buffer(buffer,position);
    position += 6;


    SDCP_encode(&IMU_ZACCEL_Processed_SMF,IMU_ZACCEL_Processed_FID,0,buffer);
    PM_push_transmit_buffer(buffer,position);
    position += 6;


    SDCP_encode(&IMU_XGYRO_Processed_SMF,IMU_XGYRO_Processed_FID,0,buffer);
    PM_push_transmit_buffer(buffer,position);
    position += 6;


    SDCP_encode(&IMU_YGYRO_Processed_SMF,IMU_YGYRO_Processed_FID,0,buffer);
    PM_push_transmit_buffer(buffer,position);
    position += 6;


    SDCP_encode(&IMU_ZGYRO_Processed_SMF,IMU_ZGYRO_Processed_FID,0,buffer);
    PM_push_transmit_buffer(buffer,position);
    position += 6;
    }

  /*  Transmit event 4 */                                    

  if( PM_SMF_Transmit_Event & ( 1 << 4 ) ){  

    SDCP_encode(&Roll_SMF,Roll_FID,0,buffer);
    PM_push_transmit_buffer(buffer,position);
    position += 6;


    SDCP_encode(&Pitch_SMF,Pitch_FID,0,buffer);
    PM_push_transmit_buffer(buffer,position);
    position += 6;


    SDCP_encode(&Yaw_SMF,Yaw_FID,0,buffer);
    PM_push_transmit_buffer(buffer,position);
    position += 6;
    }

PM_TransmitEvent = 0;                 

}                                                         