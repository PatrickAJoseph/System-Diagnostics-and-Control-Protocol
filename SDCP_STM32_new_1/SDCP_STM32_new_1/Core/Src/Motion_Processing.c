
#include "Motion_Processing.h"



/******************	HELPER FUNCTIONS.  ************************************************/


/* Helper functions to write values into comm. frame. */

static void writeToFrame_filterCoeff(float rollCoeff, float pitchCoeff, float yawCoeff)
{
  SDCP_Param parameter = {0};

  parameter.pFrame    = &CF_coeff_SMF;
  parameter.bitLength = Roll_coeff_BLEN;
  parameter.startBit  = Roll_coeff_SBIT;
  parameter.dataType  = Roll_coeff_DTYPE;
  parameter.factor    = Roll_coeff_FACTOR;
  parameter.offset    = Roll_coeff_OFFSET;
  parameter.value     = rollCoeff;

  SDCP_WPF(&parameter);

  parameter.pFrame    = &CF_coeff_SMF;
  parameter.bitLength = Pitch_coeff_BLEN;
  parameter.startBit  = Pitch_coeff_SBIT;
  parameter.dataType  = Pitch_coeff_DTYPE;
  parameter.factor    = Pitch_coeff_FACTOR;
  parameter.offset    = Pitch_coeff_OFFSET;
  parameter.value     = pitchCoeff;

  SDCP_WPF(&parameter);

  parameter.pFrame    = &CF_coeff_SMF;
  parameter.bitLength = Yaw_coeff_BLEN;
  parameter.startBit  = Yaw_coeff_SBIT;
  parameter.dataType  = Yaw_coeff_DTYPE;
  parameter.factor    = Yaw_coeff_FACTOR;
  parameter.offset    = Yaw_coeff_OFFSET;
  parameter.value     = yawCoeff;

  SDCP_WPF(&parameter);
}


static void writeToFrame_Roll(float accelRoll, float gyroRoll, float filterRoll)
{
  SDCP_Param parameter = {0};

  parameter.pFrame     =  &Roll_SMF;
  parameter.value      =  accelRoll;
  parameter.dataType   =  Roll_Accel_DTYPE;
  parameter.startBit   =  Roll_Accel_SBIT;
  parameter.bitLength  =  Roll_Accel_BLEN;
  parameter.factor     =  Roll_Accel_FACTOR;
  parameter.offset     =  Roll_Accel_OFFSET;

  SDCP_WPF(&parameter);

  parameter.pFrame     =  &Roll_SMF;
  parameter.value      =  gyroRoll;
  parameter.dataType   =  Roll_Gyro_DTYPE;
  parameter.startBit   =  Roll_Gyro_SBIT;
  parameter.bitLength  =  Roll_Gyro_BLEN;
  parameter.factor     =  Roll_Gyro_FACTOR;
  parameter.offset     =  Roll_Gyro_OFFSET;

  SDCP_WPF(&parameter);

  parameter.pFrame     =  &Roll_SMF;
  parameter.value      =  filterRoll;
  parameter.dataType   =  Roll_CF_DTYPE;
  parameter.startBit   =  Roll_CF_SBIT;
  parameter.bitLength  =  Roll_CF_BLEN;
  parameter.factor     =  Roll_CF_FACTOR;
  parameter.offset     =  Roll_CF_OFFSET;

  SDCP_WPF(&parameter);
}


static void writeToFrame_Pitch(float accelPitch, float gyroPitch, float filterPitch)
{
  SDCP_Param parameter = {0};

  parameter.pFrame     =  &Pitch_SMF;
  parameter.value      =  accelPitch;
  parameter.dataType   =  Pitch_Accel_DTYPE;
  parameter.startBit   =  Pitch_Accel_SBIT;
  parameter.bitLength  =  Pitch_Accel_BLEN;
  parameter.factor     =  Pitch_Accel_FACTOR;
  parameter.offset     =  Pitch_Accel_OFFSET;

  SDCP_WPF(&parameter);

  parameter.pFrame     =  &Pitch_SMF;
  parameter.value      =  gyroPitch;
  parameter.dataType   =  Pitch_Gyro_DTYPE;
  parameter.startBit   =  Pitch_Gyro_SBIT;
  parameter.bitLength  =  Pitch_Gyro_BLEN;
  parameter.factor     =  Pitch_Gyro_FACTOR;
  parameter.offset     =  Pitch_Gyro_OFFSET;

  SDCP_WPF(&parameter);

  parameter.pFrame     =  &Pitch_SMF;
  parameter.value      =  filterPitch;
  parameter.dataType   =  Pitch_CF_DTYPE;
  parameter.startBit   =  Pitch_CF_SBIT;
  parameter.bitLength  =  Pitch_CF_BLEN;
  parameter.factor     =  Pitch_CF_FACTOR;
  parameter.offset     =  Pitch_CF_OFFSET;

  SDCP_WPF(&parameter);
}

static void writeToFrame_Yaw(float accelYaw, float gyroYaw, float filterYaw)
{
  SDCP_Param parameter = {0};

  parameter.pFrame     =  &Yaw_SMF;
  parameter.value      =  accelYaw;
  parameter.dataType   =  Yaw_Accel_DTYPE;
  parameter.startBit   =  Yaw_Accel_SBIT;
  parameter.bitLength  =  Yaw_Accel_BLEN;
  parameter.factor     =  Yaw_Accel_FACTOR;
  parameter.offset     =  Yaw_Accel_OFFSET;

  SDCP_WPF(&parameter);

  parameter.pFrame     =  &Yaw_SMF;
  parameter.value      =  gyroYaw;
  parameter.dataType   =  Yaw_Gyro_DTYPE;
  parameter.startBit   =  Yaw_Gyro_SBIT;
  parameter.bitLength  =  Yaw_Gyro_BLEN;
  parameter.factor     =  Yaw_Gyro_FACTOR;
  parameter.offset     =  Yaw_Gyro_OFFSET;

  SDCP_WPF(&parameter);

  parameter.pFrame     =  &Yaw_SMF;
  parameter.value      =  filterYaw;
  parameter.dataType   =  Yaw_CF_DTYPE;
  parameter.startBit   =  Yaw_CF_SBIT;
  parameter.bitLength  =  Yaw_CF_BLEN;
  parameter.factor     =  Yaw_CF_FACTOR;
  parameter.offset     =  Yaw_CF_OFFSET;

  SDCP_WPF(&parameter);
}

/***************************	MAIN FUNCTIONS.	************************************/


/* Function to initialize the motion processing function. */

void MP_Init()
{
  /* Load roll, pitch and yaw settings from FLASH memory. */

  Motion_Processing.roll_coeff  = Config.roll_coeff;
  Motion_Processing.pitch_coeff = Config.pitch_coeff;
  Motion_Processing.yaw_coeff   = Config.yaw_coeff;

  /* Load motion processing output parameters from FLASH memory. */

  Motion_Processing.output_config.output_config_segments.RPY_accel_output_enabled =
  Config.IMU_settings.IMU_settings_parameters.enable_RPY_accel;

  Motion_Processing.output_config.output_config_segments.RPY_gyro_output_enabled =
  Config.IMU_settings.IMU_settings_parameters.enable_RPY_gyro;

  Motion_Processing.output_config.output_config_segments.RPY_filter_output_enabled =
  Config.IMU_settings.IMU_settings_parameters.enable_RPY_comp;

  /* Load motion processing parameters from FLASH memory. */

  Motion_Processing.angle_threshold[X] = Config.angle_threshold[X];
  Motion_Processing.angle_threshold[Y] = Config.angle_threshold[Y];
  Motion_Processing.angle_threshold[Z] = Config.angle_threshold[Z];
  Motion_Processing.vibration_threshold[X] = Config.vibration_threshold[X];
  Motion_Processing.vibration_threshold[Y] = Config.vibration_threshold[Y];
  Motion_Processing.vibration_threshold[Z] = Config.vibration_threshold[Z];

  /* Load initial configurations into message frame. */

  CF_coeff_SMF.CF_coeff_SMF_segments.Roll_coeff =
  Config.roll_coeff;
  CF_coeff_SMF.CF_coeff_SMF_segments.Pitch_coeff =
  Config.pitch_coeff;
  CF_coeff_SMF.CF_coeff_SMF_segments.Yaw_coeff =
  Config.yaw_coeff;

  MDT_X_SMF.MDT_X_SMF_segments.angle_threshold = (float)Config.angle_threshold[X]/(float)X_angle_threshold_FACTOR;
  MDT_X_SMF.MDT_X_SMF_segments.vibration_threshold = Config.vibration_threshold[X];
  MDT_Y_SMF.MDT_Y_SMF_segments.angle_threshold = Config.angle_threshold[Y]/(float)Y_angle_threshold_FACTOR;
  MDT_Y_SMF.MDT_Y_SMF_segments.vibration_threshold = Config.vibration_threshold[Y];
  MDT_Z_SMF.MDT_Z_SMF_segments.angle_threshold = Config.angle_threshold[Z]/(float)Z_angle_threshold_FACTOR;
  MDT_Z_SMF.MDT_Z_SMF_segments.vibration_threshold = Config.vibration_threshold[Z];

  Motion_Detection_SMF.all = 0;
  Motion_Processing.motion_detection.all = 0;

  /* Clear all previously stored values. */

  Motion_Processing.filter_roll  = 0.0f;
  Motion_Processing.filter_pitch = 0.0f;
  Motion_Processing.filter_yaw	 = 0.0f;

  Motion_Processing.gyro_roll   = 0.0f;
  Motion_Processing.gyro_pitch  = 0.0f;
  Motion_Processing.gyro_yaw    = 0.0f;

  Motion_Processing.accel_roll  = 0.0f;
  Motion_Processing.accel_pitch = 0.0f;
  Motion_Processing.accel_yaw   = 0.0f;

  Motion_Processing.gx_previous = 0.0f;
  Motion_Processing.gy_previous = 0.0f;
  Motion_Processing.gz_previous = 0.0f;

  /* Write settigns to frame. */

  writeToFrame_filterCoeff( Motion_Processing.roll_coeff,
		                    Motion_Processing.pitch_coeff,
							Motion_Processing.yaw_coeff );

  writeToFrame_Roll( Motion_Processing.accel_roll,
		             Motion_Processing.gyro_roll,
					 Motion_Processing.filter_roll );

  writeToFrame_Pitch( Motion_Processing.accel_pitch,
		              Motion_Processing.gyro_pitch,
					  Motion_Processing.filter_pitch );

  writeToFrame_Yaw(  Motion_Processing.accel_yaw,
		             Motion_Processing.gyro_yaw,
					 Motion_Processing.filter_yaw );
}


void MP_Update()
{
  float temp = 0.0f;


  /* Update roll, pitch and yaw values based on gyroscope value. */

  float gx = ( 0.001f * (float)imu.gyroscope[X] );
  float gy = ( 0.001f * (float)imu.gyroscope[Y] );
  float gz = ( 0.001f * (float)imu.gyroscope[Z] );

  temp = ( ( gx + Motion_Processing.gx_previous ) * MP_SAMPLE_TIME * 0.5f);
  Motion_Processing.gyro_roll += temp;

  temp = ( ( gy + Motion_Processing.gy_previous ) * MP_SAMPLE_TIME * 0.5f);
  Motion_Processing.gyro_pitch += temp;

  temp = ( ( gz + Motion_Processing.gz_previous ) * MP_SAMPLE_TIME * 0.5f);
  Motion_Processing.gyro_yaw += temp;

  Motion_Processing.gx_previous = gx;
  Motion_Processing.gy_previous = gy;
  Motion_Processing.gz_previous = gz;

  /* Update roll, pitch and yaw values based on accelerometer values. */

  float a_x = (((float)imu.accelerometer[X])/1000.0f);
  float a_y = (((float)imu.accelerometer[Y])/1000.0f);
  float a_z = (((float)imu.accelerometer[Z])/1000.0f);

  /* tan_phi = a_y / a_z. */

  if( a_z == 0.0f )
  {
	  a_z = 0.0001f;
  }

  Motion_Processing.accel_roll = RAD_TO_DEG * atan2f(a_y,a_z);

  /* tan_theta (tan pitch) = a_x / sqrt(a_y*a_y + a_z*a_z) */

  temp = a_y*a_y + a_z*a_z;

  if(temp == 0.0f)
  {
	  temp = 0.001f;
  }

  temp = sqrtf(temp);

  Motion_Processing.accel_pitch = -RAD_TO_DEG * atan2f(a_x,temp);

  /* Once calculating roll,pitch and yaw using accelerometer and gyroscope data,
   * proceed to do sensor fusion. */

  temp = Motion_Processing.roll_coeff;
  Motion_Processing.filter_roll = ( ( temp * Motion_Processing.accel_roll ) + ( 1.0f - temp ) * Motion_Processing.gyro_roll );

  temp = Motion_Processing.pitch_coeff;
  Motion_Processing.filter_pitch = ( ( temp * Motion_Processing.accel_pitch ) + ( 1.0f - temp ) * Motion_Processing.gyro_pitch );

  temp = Motion_Processing.yaw_coeff;
  Motion_Processing.filter_yaw = ( ( temp * Motion_Processing.accel_yaw ) + ( 1.0f - temp ) * Motion_Processing.gyro_yaw );

  /* Write updated values to frame. */

  writeToFrame_Roll( Motion_Processing.accel_roll,
		             Motion_Processing.gyro_roll,
					 Motion_Processing.filter_roll );

  writeToFrame_Pitch( Motion_Processing.accel_pitch,
		              Motion_Processing.gyro_pitch,
					  Motion_Processing.filter_pitch );

  writeToFrame_Yaw(  Motion_Processing.accel_yaw,
		             Motion_Processing.gyro_yaw,
					 Motion_Processing.filter_yaw );

  /* Threshold detection. */

  Motion_Processing.motion_detection.motion_detection_segments.X_vibration_detected =
  ( abs(imu.gyroscope[X]) > Motion_Processing.vibration_threshold[X] );

  Motion_Processing.motion_detection.motion_detection_segments.Y_vibration_detected =
  ( abs(imu.gyroscope[Y]) > Motion_Processing.vibration_threshold[Y] );

  Motion_Processing.motion_detection.motion_detection_segments.Z_vibration_detected =
  ( abs(imu.gyroscope[Z]) > Motion_Processing.vibration_threshold[Z] );

  Motion_Processing.motion_detection.motion_detection_segments.X_angle_detected =
  ( fabs(Motion_Processing.filter_roll) > Motion_Processing.angle_threshold[X] );

  Motion_Processing.motion_detection.motion_detection_segments.Y_angle_detected =
  ( fabs(Motion_Processing.filter_pitch) > Motion_Processing.angle_threshold[Y] );

  Motion_Processing.motion_detection.motion_detection_segments.Z_angle_detected =
  ( fabs(Motion_Processing.filter_yaw) > Motion_Processing.angle_threshold[Z] );

  /* Load motion detection results into message frames. */

  Motion_Detection_SMF.Motion_Detection_SMF_segments.X_angle_detected =
  Motion_Processing.motion_detection.motion_detection_segments.X_angle_detected;

  Motion_Detection_SMF.Motion_Detection_SMF_segments.Y_angle_detected =
  Motion_Processing.motion_detection.motion_detection_segments.Y_angle_detected;

  Motion_Detection_SMF.Motion_Detection_SMF_segments.Z_angle_detected =
  Motion_Processing.motion_detection.motion_detection_segments.Z_angle_detected;

  Motion_Detection_SMF.Motion_Detection_SMF_segments.X_vibration_detected =
  Motion_Processing.motion_detection.motion_detection_segments.X_vibration_detected;

  Motion_Detection_SMF.Motion_Detection_SMF_segments.Y_vibration_detected =
  Motion_Processing.motion_detection.motion_detection_segments.Y_vibration_detected;

  Motion_Detection_SMF.Motion_Detection_SMF_segments.Z_vibration_detected =
  Motion_Processing.motion_detection.motion_detection_segments.Z_vibration_detected;

  /* Update state of LEDs. */

  if( !LED_CNTRL_SMF.LED_CNTRL_SMF_segments.LED_OVRRD )
  {


  /* Angle detection. */

  if( LED.LED_parameters.LED1_mode == Angle_detection )
  {
	 LED.LED_parameters.LED1_state =
	 Motion_Processing.motion_detection.motion_detection_segments.X_angle_detected;
  }
  if( LED.LED_parameters.LED2_mode == Angle_detection )
  {
	 LED.LED_parameters.LED2_state =
	 Motion_Processing.motion_detection.motion_detection_segments.Y_angle_detected;
  }
  if( LED.LED_parameters.LED3_mode == Angle_detection )
  {
	 LED.LED_parameters.LED3_state =
	 Motion_Processing.motion_detection.motion_detection_segments.Z_angle_detected;
  }

  /* Vibration detection. */

  if( LED.LED_parameters.LED1_mode == Vibration_detection )
  {
	 LED.LED_parameters.LED1_state =
	 Motion_Processing.motion_detection.motion_detection_segments.X_vibration_detected;
  }
  if( LED.LED_parameters.LED2_mode == Vibration_detection )
  {
	 LED.LED_parameters.LED2_state =
	 Motion_Processing.motion_detection.motion_detection_segments.Y_vibration_detected;
  }
  if( LED.LED_parameters.LED3_mode == Vibration_detection )
  {
	 LED.LED_parameters.LED3_state =
	 Motion_Processing.motion_detection.motion_detection_segments.Z_vibration_detected;
  }


  /* Obtain sensor full-scale values. */

  uint32_t accel_fs_value;
  uint32_t gyro_fs_value;

  if(imu.sensor_configurations.sensor_configuration_segments.accel_fs_config == ACCEL_FS_2G)
  {
    accel_fs_value = 2000;
  }
  if(imu.sensor_configurations.sensor_configuration_segments.accel_fs_config == ACCEL_FS_4G)
  {
    accel_fs_value = 4000;
  }
  if(imu.sensor_configurations.sensor_configuration_segments.accel_fs_config == ACCEL_FS_8G)
  {
    accel_fs_value = 8000;
  }
  if(imu.sensor_configurations.sensor_configuration_segments.accel_fs_config == ACCEL_FS_16G)
  {
    accel_fs_value = 16000;
  }

  /* Get gyroscope full scale value. */

  if(imu.sensor_configurations.sensor_configuration_segments.gyro_fs_config == GYRO_FS_250DPS)
  {
    gyro_fs_value = 250000;
  }
  if(imu.sensor_configurations.sensor_configuration_segments.gyro_fs_config == GYRO_FS_500DPS)
  {
    gyro_fs_value = 500000;
  }
  if(imu.sensor_configurations.sensor_configuration_segments.gyro_fs_config == GYRO_FS_1000DPS)
  {
    gyro_fs_value = 1000000;
  }
  if(imu.sensor_configurations.sensor_configuration_segments.gyro_fs_config == GYRO_FS_2000DPS)
  {
    gyro_fs_value = 2000000;
  }


  /* Accelerometer measurement. */

  if( LED.LED_parameters.LED1_mode == Accelerometer_measurement )
  {
    LED.LED_parameters.LED1_duty = ( fabs( 100.0f * (float)imu.accelerometer[X] ) / (float)accel_fs_value );
  }
  if( LED.LED_parameters.LED2_mode == Accelerometer_measurement )
  {
    LED.LED_parameters.LED2_duty = ( fabs( 100.0f * (float)imu.accelerometer[Y] ) / (float)accel_fs_value );
  }
  if( LED.LED_parameters.LED3_mode == Accelerometer_measurement )
  {
    LED.LED_parameters.LED3_duty = ( fabs( 100.0f * (float)imu.accelerometer[Z] ) / (float)accel_fs_value );
  }

  /* Gyroscope measurement. */

  if( LED.LED_parameters.LED1_mode == Gyroscope_measurement )
  {
    LED.LED_parameters.LED1_duty = ( fabs( 100.0f * (float)imu.gyroscope[X] ) / (float)gyro_fs_value );
  }
  if( LED.LED_parameters.LED2_mode == Gyroscope_measurement )
  {
    LED.LED_parameters.LED2_duty = ( fabs( 100.0f * (float)imu.gyroscope[Y] ) / (float)gyro_fs_value );
  }
  if( LED.LED_parameters.LED3_mode == Gyroscope_measurement )
  {
    LED.LED_parameters.LED3_duty = ( fabs( 100.0f * (float)imu.gyroscope[Z] ) / (float)gyro_fs_value );
  }
  }

  //LED_Update();
}

