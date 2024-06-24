
#ifndef INC_CONFIGURATIONS_H_
#define INC_CONFIGURATIONS_H_

#include "Application.h"
#include "Flash.h"


/* Configuration parameters. */

struct Config
{
  float roll_coeff;  /* Roll angle calculation filter coefficient ( 0 to 1 ). */
  float pitch_coeff; /* Pitch angle calculation filter coefficient ( 0 to 1 ). */
  float yaw_coeff;   /* Yaw angle calculation filter coefficient ( 0 to 1 ).*/

  // roll_coeff, pitch_coeff, yaw_coeff: 0x08040000 - 0x0804000B.

  /* IMU setting register. */

  // IMU settings register: 0x08040000C - 0x0804000F.

  union IMU_settings
  {
    uint32_t all;

    struct IMU_settings_parameters
    {
      /* Enable row, pitch and yaw calculation using data from accelerometer only .*/
      uint32_t enable_RPY_accel : 1;

      /* Enable row, pitch and yaw calculation using data from gyroscope only. */
      uint32_t enable_RPY_gyro  : 1;

      /* Enable row, pitch and yaw calculation using complementary filter. */
      uint32_t enable_RPY_comp : 1;

      /* Accelerometer full-scale value settings. */

      uint32_t accel_fs : 3;

      /* Gyroscope full-scale value settings. */

      uint32_t gyro_fs : 3;

      /* Reserved bit. */

      uint32_t reserved : 23;

    }IMU_settings_parameters;

  }IMU_settings;

  /* IMU offset data. */

  float IMU_accel_offset[3];   // 0x08040010 - 0x0804001B.
  float IMU_gyro_offset[3];	   // 0x0804001C - 0x08040027.

  /* IMU gyroscope bias drift per hour. */

  float IMU_gyro_dph[3];  // 0x08040028 - 0x08040033.

  /* Firmware version. */

  char FW_version[16]; // 0x08040034 - 0x08040044.

  /* Vibration detection threshold. */

  float vibration_threshold[3];

  /* Angle detection threshold. */

  float angle_threshold[3];

  /* LED configurations. */

  union LED_config
  {
	  uint32_t all;

	  struct LED_config_segments
	  {
	    uint32_t LED1_mode: 3;
	    uint32_t LED2_mode: 3;
	    uint32_t LED3_mode: 3;
	  }LED_config_segments;

  }LED_config;

  /* IMU calibration settings. */

  union IMU_calibration
  {
	  uint32_t all;

	  struct IMU_calibration_parameters
	  {

		  uint8_t points;
		  uint8_t interval;
          uint8_t axis;

	  }IMU_calibration_parameters;

  }IMU_calibration;

}Config;


void Config_write();
void Config_read();
void Config_writeFirmwareInfo(char* fwVersion);

#endif /* INC_CONFIGURATIONS_H_ */
