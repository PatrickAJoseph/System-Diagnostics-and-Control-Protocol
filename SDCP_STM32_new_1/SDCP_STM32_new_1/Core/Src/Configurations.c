
#include "Configurations.h"

void Config_write()
{

	  /* Save accelerometer offset values. */

	  Config.IMU_accel_offset[0] = imu.accelerometer_offset[0];
	  Config.IMU_accel_offset[1] = imu.accelerometer_offset[1];
	  Config.IMU_accel_offset[2] = imu.accelerometer_offset[2];

	  /* Save gyroscope offset values. */

	  Config.IMU_gyro_offset[0] = imu.gyroscope_offset[0];
	  Config.IMU_gyro_offset[1] = imu.gyroscope_offset[1];
	  Config.IMU_gyro_offset[2] = imu.gyroscope_offset[2];

	  /* Save complementary filter roll,pitch and yaw coefficients. */

	  Config.roll_coeff = Motion_Processing.roll_coeff;
	  Config.pitch_coeff = Motion_Processing.pitch_coeff;
	  Config.yaw_coeff = Motion_Processing.yaw_coeff;

	  /* Save accelerometer and gyroscope full-scale ranges. */

	  Config.IMU_settings.IMU_settings_parameters.accel_fs =
	  imu.sensor_configurations.sensor_configuration_segments.accel_fs_config;

	  Config.IMU_settings.IMU_settings_parameters.gyro_fs =
	  imu.sensor_configurations.sensor_configuration_segments.gyro_fs_config;

	  Config.IMU_settings.IMU_settings_parameters.enable_RPY_accel =
	  Motion_Processing.output_config.output_config_segments.RPY_accel_output_enabled;

	  Config.IMU_settings.IMU_settings_parameters.enable_RPY_gyro =
	  Motion_Processing.output_config.output_config_segments.RPY_gyro_output_enabled;

	  Config.IMU_settings.IMU_settings_parameters.enable_RPY_comp =
	  Motion_Processing.output_config.output_config_segments.RPY_filter_output_enabled;

	  Config.angle_threshold[0] = Motion_Processing.angle_threshold[0];
	  Config.angle_threshold[1] = Motion_Processing.angle_threshold[1];
	  Config.angle_threshold[2] = Motion_Processing.angle_threshold[2];
	  Config.vibration_threshold[0] = Motion_Processing.vibration_threshold[0];
	  Config.vibration_threshold[1] = Motion_Processing.vibration_threshold[1];
	  Config.vibration_threshold[2] = Motion_Processing.vibration_threshold[2];

	  Config.LED_config.LED_config_segments.LED1_mode = LED.LED_parameters.LED1_mode;
	  Config.LED_config.LED_config_segments.LED2_mode = LED.LED_parameters.LED2_mode;
	  Config.LED_config.LED_config_segments.LED3_mode = LED.LED_parameters.LED3_mode;

      Config.IMU_calibration.IMU_calibration_parameters.interval = imu.calibration_interval_ms;
      Config.IMU_calibration.IMU_calibration_parameters.points = imu.N_calibration_points;
      Config.IMU_calibration.IMU_calibration_parameters.axis = imu.calibration_axis;

 /* Save config. */

  uint32_t address = (uint32_t)&Config;
  uint32_t size    = (uint32_t)sizeof(Config);

  Flash_Operation_State_t state;

  state = Flash_Unlock();
  state = Flash_Sector_Erase();
  state = Flash_write(Flash_Start,address,size);
  state = Flash_Lock();
}

void Config_read()
{
  uint32_t address = (uint32_t)&Config;
  uint32_t size    = (uint32_t)sizeof(Config);

  Flash_Unlock();
  Flash_read(Flash_Start,address,size);
  Flash_Lock();
}

void Config_writeFirmwareInfo(char* fwVersion)
{
  uint8_t counter = 0;

  while( *fwVersion != '\0' )
  {
	Config.FW_version[counter] = *fwVersion;
    fwVersion++;
	counter++;
  }
}



