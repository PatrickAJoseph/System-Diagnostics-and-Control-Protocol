
#ifndef INC_MOTION_PROCESSING_H_
#define INC_MOTION_PROCESSING_H_

#include "MPU6050.h"
#include "math.h"
#include "SMFs.h"
#include "Configurations.h"

#define MP_SAMPLE_TIME		0.01f
#define RAD_TO_DEG 			180.0f/M_PI

struct Motion_Processing
{

  float filter_roll;
  float filter_pitch;
  float filter_yaw;

  float roll_coeff;
  float pitch_coeff;
  float yaw_coeff;

  float accel_roll;
  float accel_pitch;
  float accel_yaw;

  float gyro_roll;
  float gyro_pitch;
  float gyro_yaw;

  float gx_previous;
  float gy_previous;
  float gz_previous;

  union output_config
  {
    uint32_t all;

	struct output_config_segments
	{
	  uint32_t RPY_accel_output_enabled: 1;
	  uint32_t RPY_gyro_output_enabled: 1;
	  uint32_t RPY_filter_output_enabled: 1;
	}output_config_segments;

  }output_config;

  float vibration_threshold[3];
  float angle_threshold[3];

  union motion_detection
  {
    uint32_t all;

    struct motion_detection_segments
    {
      uint32_t X_angle_detected : 1;
      uint32_t Y_angle_detected : 1;
      uint32_t Z_angle_detected : 1;

      uint32_t X_vibration_detected : 1;
      uint32_t Y_vibration_detected : 1;
      uint32_t Z_vibration_detected : 1;

    }motion_detection_segments;

  }motion_detection;

}Motion_Processing;

void MP_Init();
void MP_Update();

#endif /* INC_MOTION_PROCESSING_H_ */
