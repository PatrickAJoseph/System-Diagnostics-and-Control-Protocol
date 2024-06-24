
#include "Protocol_Manager.h"

#define PM_nFrames    33
#define PM_nTransmitEvents 5

uint8_t PM_SMF_Transmit_Buffer[ 6 * PM_nFrames ];
uint8_t PM_SMF_Transmit_Counter[ PM_nTransmitEvents ];
uint8_t PM_SMF_Transmit_Counter_Limit[ PM_nTransmitEvents ] = { 10,1,25,1,5 };
uint32_t pSMFs[PM_nFrames];

uint32_t PM_SMFPRI[4];  // Standard message frame priority.

static uint8_t verifyChecksum(uint8_t* buffer)
{
  uint16_t checksum = 0;

  checksum += (uint16_t)buffer[0];
  checksum += (uint16_t)buffer[1];
  checksum += (uint16_t)buffer[2];
  checksum += (uint16_t)buffer[3];
  checksum += (uint16_t)buffer[4];
  checksum += (uint16_t)buffer[5];

  return((uint8_t)((checksum%255) == 0));
}

static uint8_t obtainChecksum(uint8_t* buffer)
{
  uint16_t checksum = 0;

  checksum += (uint16_t)buffer[0];
  checksum += (uint16_t)buffer[1];
  checksum += (uint16_t)buffer[2];
  checksum += (uint16_t)buffer[3];
  checksum += (uint16_t)buffer[4];
  checksum = ( 255 - (checksum%255) );

  return(checksum);
}

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
  MDT_X_FID,
  MDT_Y_FID,
  MDT_Z_FID,
  Motion_Detection_FID,
  IMU_calibration_FID,
  Task_counter_FID

};

void PM_setMsgFramePriority(uint8_t frame_ID, PM_SMF_Priority priority)
{
  uint32_t priorityMask = ( 1UL << (frame_ID % 32) );

  /* Clear current priority bit. */

  PM_SMFPRI[ frame_ID >> 5 ] &= ~priorityMask;

  /* Set priority value. */

  PM_SMFPRI[ frame_ID >> 5 ] |= ( priority << ( frame_ID % 32 ) );
}

PM_SMF_Priority PM_getMsgFramePriority(uint8_t frame_ID)
{
  uint32_t index = (frame_ID % 32);

  return( (PM_SMF_Priority)( ( PM_SMFPRI[ frame_ID >> 5 ] & ( 1 << index ) ) >> index ) );
}

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
  pSMFs[26] = (uint32_t)&MDT_X_SMF;
  pSMFs[27] = (uint32_t)&MDT_Y_SMF;
  pSMFs[28] = (uint32_t)&MDT_Z_SMF;
  pSMFs[29] = (uint32_t)&Motion_Detection_SMF;
  pSMFs[30] = (uint32_t)&IMU_calibration_SMF;
  pSMFs[31] = (uint32_t)&Task_counter_SMF;

  /* Set frame priorities of all frames to low. */

  PM_SMFPRI[0] = 0;
  PM_SMFPRI[1] = 0;
  PM_SMFPRI[2] = 0;
  PM_SMFPRI[3] = 0;

  /* Setting transfer mode to standard message frame transfer. */

  TMC.TMC_segments.BTR_ENSTAT = 0;
  TMC.TMC_segments.FTOR_ENSTAT = 0;
  TMC.TMC_segments.SFT_ENSTAT = 1;

//  Application specific initializations for standard message frames
//  should be made after this section.


  IMU_settings_SMF.IMU_settings_SMF_segments.GYRO_FS
  = imu.sensor_configurations.sensor_configuration_segments.gyro_fs_config;

  IMU_settings_SMF.IMU_settings_SMF_segments.ACCEL_FS
  = imu.sensor_configurations.sensor_configuration_segments.accel_fs_config;
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

void PM_SMF_Tx_Handler()
{
  /* Scheduler counter. */

  uint32_t PM_SMF_Transmit_Event = 0;

  for( int i = 0 ; i < PM_nTransmitEvents ; i++ )
  {
    PM_SMF_Transmit_Counter[i]++;

    if(PM_SMF_Transmit_Counter[i] >= PM_SMF_Transmit_Counter_Limit[i] )
    {
      PM_SMF_Transmit_Counter[i] = 0;
	  PM_SMF_Transmit_Event |= (1 << i);
    }
  }


  /* Load transmit buffer. */

  uint8_t buffer[6];   // Temporary buffer to frame values.
  uint16_t position = 0;
  PM_SMF_Priority priority = LOW;


  /*  Transmit event 0 */

  if( PM_SMF_Transmit_Event & ( 1 << 0 ) ){

	priority = PM_getMsgFramePriority(GUI_FID);
    SDCP_encode(&GUI_SMF,GUI_FID,priority,buffer);
    PM_push_transmit_buffer(buffer,position);
    position += 6;

    priority = PM_getMsgFramePriority(LED_CNTRL_FID);
    SDCP_encode(&LED_CNTRL_SMF,LED_CNTRL_FID,priority,buffer);
    PM_push_transmit_buffer(buffer,position);
    position += 6;

    priority = PM_getMsgFramePriority(Motion_Detection_FID);
    SDCP_encode(&Motion_Detection_SMF,Motion_Detection_FID,priority,buffer);
    PM_push_transmit_buffer(buffer,position);
    position += 6;
    }

  /*  Transmit event 1 */

  if( PM_SMF_Transmit_Event & ( 1 << 1 ) ){

	priority = PM_getMsgFramePriority(IMU_X_Raw_FID);

	SDCP_encode(&IMU_X_Raw_SMF,IMU_X_Raw_FID,priority,buffer);
    PM_push_transmit_buffer(buffer,position);
    position += 6;

	priority = PM_getMsgFramePriority(IMU_Y_Raw_FID);

    SDCP_encode(&IMU_Y_Raw_SMF,IMU_Y_Raw_FID,priority,buffer);
    PM_push_transmit_buffer(buffer,position);
    position += 6;

	priority = PM_getMsgFramePriority(IMU_Z_Raw_FID);

    SDCP_encode(&IMU_Z_Raw_SMF,IMU_Z_Raw_FID,priority,buffer);
    PM_push_transmit_buffer(buffer,position);
    position += 6;

	priority = PM_getMsgFramePriority(IMU_XACCEL_Processed_FID);

    SDCP_encode(&IMU_XACCEL_Processed_SMF,IMU_XACCEL_Processed_FID,priority,buffer);
    PM_push_transmit_buffer(buffer,position);
    position += 6;

	priority = PM_getMsgFramePriority(IMU_YACCEL_Processed_FID);

    SDCP_encode(&IMU_YACCEL_Processed_SMF,IMU_YACCEL_Processed_FID,priority,buffer);
    PM_push_transmit_buffer(buffer,position);
    position += 6;

	priority = PM_getMsgFramePriority(IMU_ZACCEL_Processed_FID);

    SDCP_encode(&IMU_ZACCEL_Processed_SMF,IMU_ZACCEL_Processed_FID,priority,buffer);
    PM_push_transmit_buffer(buffer,position);
    position += 6;

	priority = PM_getMsgFramePriority(IMU_XGYRO_Processed_FID);

    SDCP_encode(&IMU_XGYRO_Processed_SMF,IMU_XGYRO_Processed_FID,priority,buffer);
    PM_push_transmit_buffer(buffer,position);
    position += 6;

	priority = PM_getMsgFramePriority(IMU_YGYRO_Processed_FID);

    SDCP_encode(&IMU_YGYRO_Processed_SMF,IMU_YGYRO_Processed_FID,priority,buffer);
    PM_push_transmit_buffer(buffer,position);
    position += 6;

	priority = PM_getMsgFramePriority(IMU_ZGYRO_Processed_FID);

    SDCP_encode(&IMU_ZGYRO_Processed_SMF,IMU_ZGYRO_Processed_FID,priority,buffer);
    PM_push_transmit_buffer(buffer,position);
    position += 6;
    }

  /*  Transmit event 2 */

  if( PM_SMF_Transmit_Event & ( 1 << 2 ) ){

    priority = PM_getMsgFramePriority(IMU_Temp_FID);

    SDCP_encode(&IMU_Temp_SMF,IMU_Temp_FID,priority,buffer);
    PM_push_transmit_buffer(buffer,position);
    position += 6;
    }

  /*  Transmit event 4 */

  if( PM_SMF_Transmit_Event & ( 1 << 4 ) ){

	priority = PM_getMsgFramePriority(Roll_FID);

    SDCP_encode(&Roll_SMF,Roll_FID,priority,buffer);
    PM_push_transmit_buffer(buffer,position);
    position += 6;

	priority = PM_getMsgFramePriority(Pitch_FID);

    SDCP_encode(&Pitch_SMF,Pitch_FID,priority,buffer);
    PM_push_transmit_buffer(buffer,position);
    position += 6;

	priority = PM_getMsgFramePriority(Yaw_FID);

    SDCP_encode(&Yaw_SMF,Yaw_FID,priority,buffer);
    PM_push_transmit_buffer(buffer,position);
    position += 6;
    }

  PM_SMF_Transmit_Event = 0;

  UART_DMA_Transmit((uint8_t*)PM_SMF_Transmit_Buffer, position);
}


void PM_Rx_Handler()
{
  uint8_t buffer[6] = {1,1,1,1,1,1};

  uint8_t queueStatus = 0;

  queueStatus = SDCP_RXDQ(buffer);

  /* Check for 6-byte 0 synchronization frame. */

  if(    !buffer[0] && !buffer[1] && !buffer[2]
      && !buffer[3] && !buffer[4] && !buffer[5] && queueStatus )
  {
    buffer[0] = 0; buffer[1] = 0; buffer[2] = 0;
    buffer[3] = 0; buffer[4] = 0; buffer[5] = 0;

    while(!UART.UART_parameters.DMA_Tx_completed);
    UART_DMA_Transmit(buffer,6);
    while(!UART.UART_parameters.DMA_Tx_completed);
    return;
  }

  /* Standard message frames have frame IDs from 1 to 240. */

  if( buffer[0] < 248 )
  {
	 PM_SMF_Rx_Handler(buffer);
  }

  /* Handler for frame transfer on request. */

  if( ( buffer[0] == 248 ) || (buffer[0] == 249) )
  {
	  PM_FTOR_Handler(buffer);
  }

  /* Handler for block transfer control frame. */

  if( ( buffer[0] == 250 ) || ( buffer[0] == 251 ) || ( buffer[0] == 252 ) )
  {
	  PM_BTR_Rx_Handler(buffer);
  }

  /* Handler for transfer mode request and control frame. */

  if( buffer[0] == 254 )
  {
	  PM_TMC_Rx_Handler(buffer);
  }
}

void PM_SMF_Rx_Handler(uint8_t* buffer)
{
  /* Verify checksum of received message frame. */

  if( verifyChecksum(buffer) )
  {
	/* Case for handling LED update events. */

	if( ( buffer[0] & 127 ) == LED_CNTRL_FID )
	{
	  /* Decode the given message frame. */

	   SDCP_decode(buffer,LED_CNTRL_FID,&LED_CNTRL_SMF);
	   asm("NOP");

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
	     LED.LED_parameters.LED1_duty = (float)LED_CNTRL_SMF.LED_CNTRL_SMF_segments.XLED_Brightness * XLED_Brightness_FACTOR;
	     LED.LED_parameters.LED2_duty = (float)LED_CNTRL_SMF.LED_CNTRL_SMF_segments.YLED_Brightness * YLED_Brightness_FACTOR;
	     LED.LED_parameters.LED3_duty = (float)LED_CNTRL_SMF.LED_CNTRL_SMF_segments.ZLED_Brightness * ZLED_Brightness_FACTOR;
	   }

	   LED_Update();

	}  /* End LED_CNTRL SMF handler. */


	/* Case for handling IMU control commands. */

	if( ( buffer[0] & 127 ) == IMU_settings_FID )
	{
		/* Store current values in the message frame. */

		uint32_t __temp = IMU_settings_SMF.all;

		/* Decode the message frame contents. */

		SDCP_decode(buffer,IMU_settings_FID,&IMU_settings_SMF);

		/* If the write accelerometer configuration bit is enabled. */

		if(IMU_settings_SMF.IMU_settings_SMF_segments.write_accel_config)
		{
			imu.sensor_configurations.sensor_configuration_segments.accel_fs_config =
		    IMU_settings_SMF.IMU_settings_SMF_segments.ACCEL_FS;

			IMU_settings_SMF.IMU_settings_SMF_segments.write_accel_config = 0;

			/* Clear configuration bits. */

			__temp &= ~( ( ( 1UL << ACCEL_FS_BLEN ) - 1 ) << ACCEL_FS_SBIT );

			/* Update accelerometer full-scale configuration bits. */

			__temp |= ( (uint32_t)IMU_settings_SMF.IMU_settings_SMF_segments.ACCEL_FS << ACCEL_FS_SBIT );
		}

		/* If the write gyroscope configuration bit is enabled. */

		if(IMU_settings_SMF.IMU_settings_SMF_segments.write_gyro_config)
		{
			imu.sensor_configurations.sensor_configuration_segments.gyro_fs_config =
		    IMU_settings_SMF.IMU_settings_SMF_segments.GYRO_FS;

			IMU_settings_SMF.IMU_settings_SMF_segments.write_gyro_config = 0;

			/* Clear configuration bits. */

			__temp &= ~( ( ( 1UL << GYRO_FS_BLEN ) - 1 ) << GYRO_FS_SBIT );

			/* Update gyroscope full-scale configuration bits. */

			__temp |= ( (uint32_t)IMU_settings_SMF.IMU_settings_SMF_segments.GYRO_FS << GYRO_FS_SBIT );
		}

		/* If calibration bit is set, perform calibration. */

		if(IMU_settings_SMF.IMU_settings_SMF_segments.Calibrate_CS)
		{
			__disable_irq();
			MPU6050_calibrate(imu.calibration_axis,imu.N_calibration_points);
			MP_Init();
			__enable_irq();
			IMU_settings_SMF.IMU_settings_SMF_segments.Calibrate_CS = 0;
		}

		if(IMU_settings_SMF.IMU_settings_SMF_segments.write_ryp_accel_config)
		{
			Motion_Processing.output_config.output_config_segments.RPY_accel_output_enabled =
			IMU_settings_SMF.IMU_settings_SMF_segments.write_ryp_accel_config;
		}

		if(IMU_settings_SMF.IMU_settings_SMF_segments.write_ryp_gyro_config)
		{
			Motion_Processing.output_config.output_config_segments.RPY_gyro_output_enabled =
			IMU_settings_SMF.IMU_settings_SMF_segments.write_ryp_gyro_config;
		}

		if(IMU_settings_SMF.IMU_settings_SMF_segments.write_ryp_filter_config)
		{
			Motion_Processing.output_config.output_config_segments.RPY_filter_output_enabled =
			IMU_settings_SMF.IMU_settings_SMF_segments.write_ryp_filter_config;
		}

		/* Clear all write configuration bits. */

		__temp &= ~( 1UL << write_accel_config_SBIT );
		__temp &= ~( 1UL << write_gyro_config_SBIT );
		__temp &= ~( 1UL << write_rpy_accel_config_SBIT );
		__temp &= ~( 1UL << write_rpy_gyro_config_SBIT );
		__temp &= ~( 1UL << write_rpy_filter_config_SBIT );

		IMU_settings_SMF.all = __temp;

	}/* END: IMU_settings handler. */


	/* Handler for complementary filter coefficients. */

	if( (buffer[0] & 127) == CF_coeff_FID )
	{
		SDCP_decode(buffer,CF_coeff_FID,&CF_coeff_SMF);

		SDCP_Param tempParameter;

		tempParameter.bitLength = Roll_coeff_BLEN;
		tempParameter.dataType  = Roll_coeff_DTYPE;
		tempParameter.factor    = Roll_coeff_FACTOR;
		tempParameter.offset    = Roll_coeff_OFFSET;
		tempParameter.pFrame    = &CF_coeff_SMF;
		tempParameter.startBit  = Roll_coeff_SBIT;

		SDCP_RPF(&tempParameter);
		Motion_Processing.roll_coeff = tempParameter.value;

		tempParameter.bitLength = Pitch_coeff_BLEN;
		tempParameter.dataType  = Pitch_coeff_DTYPE;
		tempParameter.factor    = Pitch_coeff_FACTOR;
		tempParameter.offset    = Pitch_coeff_OFFSET;
		tempParameter.pFrame    = &CF_coeff_SMF;
		tempParameter.startBit  = Pitch_coeff_SBIT;

		SDCP_RPF(&tempParameter);
		Motion_Processing.pitch_coeff = tempParameter.value;

		tempParameter.bitLength = Yaw_coeff_BLEN;
		tempParameter.dataType  = Yaw_coeff_DTYPE;
		tempParameter.factor    = Yaw_coeff_FACTOR;
		tempParameter.offset    = Yaw_coeff_OFFSET;
		tempParameter.pFrame    = &CF_coeff_SMF;
		tempParameter.startBit  = Yaw_coeff_SBIT;

		SDCP_RPF(&tempParameter);
		Motion_Processing.yaw_coeff = tempParameter.value;

	}/* END: CF_coeff SMF handler. */


	/* Handler for configuration read, write. */

	if( ( buffer[0] & 127 ) == Config_RW_FID )
	{
	  SDCP_decode(buffer,Config_RW_FID,&Config_RW_SMF);

	  if(Config_RW_SMF.Config_RW_SMF_segments.save_config)
	  {
		  __disable_irq();

		  /* Write configurations. */

		  Config_write();

		  __enable_irq();
	  }
	}

	/* Handler for motion detection parameters. */

	if( ( buffer[0] & 127 ) == MDT_X_FID )
	{
	  SDCP_decode(buffer,MDT_X_FID,&MDT_X_SMF);

	  Motion_Processing.angle_threshold[X] = MDT_X_SMF.MDT_X_SMF_segments.angle_threshold*X_angle_threshold_FACTOR;
      Motion_Processing.vibration_threshold[X] = MDT_X_SMF.MDT_X_SMF_segments.vibration_threshold;
	}

	if( ( buffer[0] & 127 ) == MDT_Y_FID )
	{
	  SDCP_decode(buffer,MDT_Y_FID,&MDT_Y_SMF);

	  Motion_Processing.angle_threshold[Y] = MDT_Y_SMF.MDT_Y_SMF_segments.angle_threshold*X_angle_threshold_FACTOR;
      Motion_Processing.vibration_threshold[Y] = MDT_Y_SMF.MDT_Y_SMF_segments.vibration_threshold;
	}

	if( ( buffer[0] & 127 ) == MDT_Z_FID )
	{
	  SDCP_decode(buffer,MDT_Z_FID,&MDT_Z_SMF);

	  Motion_Processing.angle_threshold[Z] = MDT_Z_SMF.MDT_Z_SMF_segments.angle_threshold*X_angle_threshold_FACTOR;
      Motion_Processing.vibration_threshold[Z] = MDT_Z_SMF.MDT_Z_SMF_segments.vibration_threshold;
	}

	if( ( buffer[0] & 127 ) == IMU_calibration_FID )
	{
	  SDCP_decode(buffer,IMU_calibration_FID,&IMU_calibration_SMF);

	  imu.N_calibration_points = IMU_calibration_SMF.IMU_calibration_SMF_segments.points;
	  imu.calibration_axis = IMU_calibration_SMF.IMU_calibration_SMF_segments.axis;
	  imu.calibration_interval_ms = IMU_calibration_SMF.IMU_calibration_SMF_segments.interval;
	}

	/* Handler for GUI control parameters. */

	if( ( buffer[0] & 127 ) == GUI_FID )
	{
	  SDCP_decode(buffer,GUI_FID,&GUI_SMF);

	  Rotary_Encoder.parameters.clockwise = GUI_SMF.GUI_SMF_segments.RENC_CLK;
	  Rotary_Encoder.parameters.anticlockwise = GUI_SMF.GUI_SMF_segments.RENC_ACLK;
	  Rotary_Encoder.parameters.switch_state = GUI_SMF.GUI_SMF_segments.RENC_SW;
	}
  }
}


void PM_FTOR_Handler(uint8_t* buffer)
{
  /* Case for FTOR1 message frame. */

  if( buffer[0] == 248 )
  {
	uint8_t txBuffer[6];
	uint8_t rxBuffer[6];

	/* Get request field data. */

    FTOR1.all = ( ( (uint32_t)buffer[1] << 24 )
    						   | ( (uint32_t)buffer[2] << 16 )
							   | ( (uint32_t)buffer[3] << 8 )
							   | ( (uint32_t)buffer[4] << 0 ) );

    /* Clear the RQST bit and set the ACK bit. */

    FTOR1.FTOR1_segments.REQ = 0;
    FTOR1.FTOR1_segments.ACK = 1;

    txBuffer[0] = 248;
    txBuffer[1] = ((FTOR1.all >> 24) & 0xFF);
    txBuffer[2] = ((FTOR1.all >> 16) & 0xFF);
    txBuffer[3] = ((FTOR1.all >> 8) & 0xFF);
    txBuffer[4] = ((FTOR1.all >> 0) & 0xFF);
    txBuffer[5] = obtainChecksum(txBuffer);

    /* Wait for current DMA transfer to complete and then proceed
     * forward to send ACK frame. */

    while(!UART.UART_parameters.DMA_Tx_completed);
    UART_DMA_Transmit(txBuffer,6);
    while(!UART.UART_parameters.DMA_Tx_completed);


    uint8_t bytesToTransmit[42] = {0};  // 42 bytes allocates since maximum of
    							  // 7 frames can be transmitted on
    							  // request.

    uint8_t numberOfFrameRequests = FTOR1.FTOR1_segments.NF;

    /* Load the frame IDs to transmit. */

    uint8_t frame_ID[7];

    frame_ID[0] = FTOR1.FTOR1_segments.FID_0;
    frame_ID[1] = FTOR1.FTOR1_segments.FID_1;
    frame_ID[2] = FTOR1.FTOR1_segments.FID_2;
    frame_ID[3] = FTOR2.FTOR2_segments.FID_3;
    frame_ID[4] = FTOR2.FTOR2_segments.FID_4;
    frame_ID[5] = FTOR2.FTOR2_segments.FID_5;
    frame_ID[6] = FTOR2.FTOR2_segments.FID_6;

    /* Fill in buffer to transmit. */

    int index = -1;
    uint8_t tempBuffer[6];
    uint16_t pointerToBuffer = 0;

    for(int i = 0 ; i < numberOfFrameRequests ; i++ )
    {
      /* Search for structure corresponding to current frame ID.*/

      for(int j = 0 ; j < PM_nFrames ; j++ )
      {
        if( PM_FIDs[j] == frame_ID[i] )
        {
        	index = j;
        }
      }

      /* If the frame is found, proceed. */

      if( index != -1 )
      {
        SDCP_encode( (void*)pSMFs[index], PM_FIDs[index], 0, tempBuffer);

        bytesToTransmit[pointerToBuffer+0] = tempBuffer[0];
        bytesToTransmit[pointerToBuffer+1] = tempBuffer[1];
        bytesToTransmit[pointerToBuffer+2] = tempBuffer[2];
        bytesToTransmit[pointerToBuffer+3] = tempBuffer[3];
        bytesToTransmit[pointerToBuffer+4] = tempBuffer[4];
        bytesToTransmit[pointerToBuffer+5] = tempBuffer[5];

        pointerToBuffer += 6;
      }
    }

    //while(!UART.UART_parameters.DMA_Tx_completed);
    UART_DMA_Transmit(bytesToTransmit,6*numberOfFrameRequests);
    while(!UART.UART_parameters.DMA_Tx_completed);
  }
  else if( buffer[0] == 249 )
  {
    /* FTOR2 handler. */

	FTOR2.all = ( ( (uint32_t)buffer[1] << 24 )
	    					   | ( (uint32_t)buffer[2] << 16 )
							   | ( (uint32_t)buffer[3] << 8 )
							   | ( (uint32_t)buffer[4] << 0 ) );
  }

  return;
}


void PM_TMC_Rx_Handler(uint8_t* buffer)
{
	  uint8_t tbfr[6];

	  /* Decode transfer mode control frame from the received frame. */
	  /* Note that decode should be done only for read/write bits only. */

	  TMC.TMC_segments.TMS_REQ = (( buffer[1] & ( 1 << 3 ) ) >> 3);
	  TMC.TMC_segments.TMC_REQ = (( buffer[1] & ( 1 << 1 ) ) >> 1);

	  TMC.TMC_segments.BTR_ENREQ = (( buffer[2] & ( 1 << 2 ) ) >> 2);
	  TMC.TMC_segments.FTOR_ENREQ = (( buffer[2] & ( 1 << 1 ) ) >> 1);
	  TMC.TMC_segments.SFT_ENREQ = (( buffer[2] & ( 1 << 0 ) ) >> 0);

	  TMC.TMC_segments.MTEN_REQ = (( buffer[3] & ( 1 << 0 ) ) >> 0);

	  /* Check for transfer mode status requests. */

	  if(TMC.TMC_segments.TMS_REQ)
	  {
		  /* Reset the request bit and set acknowledgement bit. */

		  TMC.TMC_segments.TMC_REQ = 0;
		  TMC.TMC_segments.TMC_ACK = 0;
		  TMC.TMC_segments.TMS_REQ = 0;
		  TMC.TMC_segments.TMS_ACK = 1;

		  tbfr[0] = 254;
		  tbfr[1] = ((((uint32_t)TMC.all) & (uint32_t)((uint32_t)0xFFUL << 24)) >> 24);
		  tbfr[2] = ((((uint32_t)TMC.all) & (uint32_t)((uint32_t)0xFFUL << 16)) >> 16);
		  tbfr[3] = ((((uint32_t)TMC.all) & (uint32_t)((uint32_t)0xFFUL << 8)) >> 8);
		  tbfr[4] = ((((uint32_t)TMC.all) & (uint32_t)((uint32_t)0xFFUL << 0)) >> 0);
		  tbfr[5] = obtainChecksum(tbfr);

	      while(!UART.UART_parameters.DMA_Tx_completed);
	      UART_DMA_Transmit(tbfr,6);
	      while(!UART.UART_parameters.DMA_Tx_completed);

	      TMC.TMC_segments.TMS_ACK = 0;
	  }

	  /* Check for transfer mode control request. */
	  /* When this request is made, the mode of transfer is updated
	   * to the transfer mode requested by the sender. */

	  if(TMC.TMC_segments.TMC_REQ)
	  {
	    TMC.TMC_segments.BTR_ENSTAT = TMC.TMC_segments.BTR_ENREQ;
	    TMC.TMC_segments.FTOR_ENSTAT = TMC.TMC_segments.FTOR_ENREQ;
	    TMC.TMC_segments.SFT_ENSTAT = TMC.TMC_segments.SFT_ENREQ;

	    TMC.TMC_segments.TMS_REQ = 0;
	    TMC.TMC_segments.TMS_ACK = 0;
	    TMC.TMC_segments.TMC_REQ = 0;
	    TMC.TMC_segments.TMC_ACK = 1;

		tbfr[0] = 254;
		tbfr[1] = ((((uint32_t)TMC.all) & (uint32_t)((uint32_t)0xFFUL << 24)) >> 24);
		tbfr[2] = ((((uint32_t)TMC.all) & (uint32_t)((uint32_t)0xFFUL << 16)) >> 16);
		tbfr[3] = ((((uint32_t)TMC.all) & (uint32_t)((uint32_t)0xFFUL << 8)) >> 8);
		tbfr[4] = ((((uint32_t)TMC.all) & (uint32_t)((uint32_t)0xFFUL << 0)) >> 0);
		tbfr[5] = obtainChecksum(tbfr);

	    while(!UART.UART_parameters.DMA_Tx_completed);
	    UART_DMA_Transmit(tbfr,6);
	    while(!UART.UART_parameters.DMA_Tx_completed);

	    TMC.TMC_segments.TMC_ACK = 0;
	    TMC.TMC_segments.BTR_ENREQ = 0;
	    TMC.TMC_segments.FTOR_ENREQ = 0;
	    TMC.TMC_segments.SFT_ENREQ = 0;
	  }
}


void PM_BTR_Rx_Handler(uint8_t* buffer)
{
  uint8_t txBuffer[6];

  /* Register 250 is BTR1 register. */

  if( buffer[0] == 250 )
  {
	uint32_t __data = 0;

	__data |= ( ((uint32_t)buffer[1]) << 24 );
	__data |= ( ((uint32_t)buffer[2]) << 16 );
	__data |= ( ((uint32_t)buffer[3]) << 8 );
	__data |= ( ((uint32_t)buffer[4]) << 0 );

	BTR1.all = __data;

    if( BTR1.BTR1_segments.REQ)
    {
    	/* Set acknowledgement bit and reset the request bit. */

    	BTR1.BTR1_segments.ACK = 1;
    	BTR1.BTR1_segments.REQ = 0;

    	TMC.TMC_segments.BTR_ENSTAT = 1;
    	TMC.TMC_segments.FTOR_ENSTAT = 0;
    	TMC.TMC_segments.SFT_ENSTAT = 0;

    	__data = BTR1.all;

    	txBuffer[0] = 250;
    	txBuffer[1] = ( ( ((uint32_t)__data) & ( 0xFFUL << 24 ) ) >> 24 );
    	txBuffer[2] = ( ( ((uint32_t)__data) & ( 0xFFUL << 16 ) ) >> 16 );
    	txBuffer[3] = ( ( ((uint32_t)__data) & ( 0xFFUL << 8 ) ) >> 8 );
    	txBuffer[4] = ( ( ((uint32_t)__data) & ( 0xFFUL << 0 ) ) >> 0 );
    	txBuffer[5] = obtainChecksum(txBuffer);

	    while(!UART.UART_parameters.DMA_Tx_completed);
	    UART_DMA_Transmit(txBuffer,6);
	    while(!UART.UART_parameters.DMA_Tx_completed);

    	SDCP_BTR_PID[0] = BTR1.BTR1_segments.PID_0;

        /* Updating FSM to load the parameter ID of the first parameter
         * to be transmitted. The current parameter ID of the parameter
         * to be transferred is accessed by the variable SDCP_BTR_currentPID. */

    	/* The parameter and transfer counters must be reset. */

    	SDCP_BTR_PCNTR = 0;
    	SDCP_BTR_XCNTR = 0;

    	TMC.TMC_segments.BTR_ENSTAT = 1;
    	TMC.TMC_segments.FTOR_ENSTAT = 0;
    	TMC.TMC_segments.SFT_ENSTAT = 0;

    	asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
    	asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
    	asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
    	asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");

    	SDCP_BTR_currentPID = SDCP_BTR_PID[SDCP_BTR_PCNTR];
    	SDCP_BTR_PCNTR = 0;
    	SDCP_BTR_XCNTR = 0;

    	SDCP_BTR_FSM_update();


        asm("NOP");
        asm("NOP");
    }
  }

  /* Register 251 is BTR2 register. */

  if( buffer[0] == 251 )
  {
	  BTR2.BTR2_segments.PID_1 = buffer[1];
	  BTR2.BTR2_segments.PID_2 = buffer[2];
	  BTR2.BTR2_segments.PID_3 = buffer[3];
	  BTR2.BTR2_segments.PID_4 = buffer[4];

	  SDCP_BTR_PID[1] = buffer[1];
	  SDCP_BTR_PID[2] = buffer[2];
	  SDCP_BTR_PID[3] = buffer[3];
	  SDCP_BTR_PID[4] = buffer[4];
  }

  /* Register 252 is BTR3 register. */

  if( buffer[0] == 252 )
  {
	  BTR3.BTR3_segments.PID_5 = buffer[1];
	  BTR3.BTR3_segments.PID_6 = buffer[2];
	  BTR3.BTR3_segments.PID_7 = buffer[3];
	  BTR3.BTR3_segments.PID_8 = buffer[4];

	  SDCP_BTR_PID[5] = buffer[1];
	  SDCP_BTR_PID[6] = buffer[2];
	  SDCP_BTR_PID[7] = buffer[3];
	  SDCP_BTR_PID[8] = buffer[4];
  }
}



void PM_BTR_Tx_Handler()
{
  SDCP_BTR_buffer_position = 0;

  if( SDCP_BTR_currentPID == 1 )
  {
    SDCP_BTR_push( &(imu.accelerometer[X]), I32 );
    SDCP_BTR_FSM_update();
    asm("NOP");
  }
  if( SDCP_BTR_currentPID == 2 )
  {
    SDCP_BTR_push( &(imu.accelerometer[Y]), I32 );
    SDCP_BTR_FSM_update();
  }
  if( SDCP_BTR_currentPID == 3 )
  {
    SDCP_BTR_push( &(imu.accelerometer[Z]), I32 );
    SDCP_BTR_FSM_update();
  }
  if( SDCP_BTR_currentPID == 4 )
  {
    SDCP_BTR_push( &(imu.gyroscope[X]), I32 );
    SDCP_BTR_FSM_update();
  }
  if( SDCP_BTR_currentPID == 5 )
  {
    SDCP_BTR_push( &(imu.gyroscope[Y]), I32 );
    SDCP_BTR_FSM_update();
  }
  if( SDCP_BTR_currentPID == 6 )
  {
    SDCP_BTR_push( &(imu.gyroscope[Z]), I32 );
    SDCP_BTR_FSM_update();
  }

  if( SDCP_BTR_buffer_position != 0 )
  {
    UART_DMA_Transmit(SDCP_BTR_buffer,SDCP_BTR_buffer_position);
  }
}


void Protocol_Manager()
{
	/* Proceed to transmit messages. Only if the standard frame transmit
	 * bit is set in the transfer mode control register, transit the
	 * standard message frames. */

	if(TMC.TMC_segments.SFT_ENSTAT)
	{
	  PM_SMF_Tx_Handler();
	}
	else if(TMC.TMC_segments.BTR_ENSTAT)
	{
	  PM_BTR_Tx_Handler();
	}

	/* First, handle the received messages. */

	PM_Rx_Handler();

}