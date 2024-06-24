

#ifndef INCLUDE_SMFs_H
#define INCLUDE_SMFs_H

/*  Frame Name : GUI   */

#include "SDCP.h"

#define GUI_FID     1

#define SCRN_NO_FACTOR  1.000000f
#define SCRN_NO_OFFSET  0.000000f
#define SCRN_NO_DTYPE UNSIGNED
#define SCRN_NO_SBIT 0
#define SCRN_NO_BLEN 4

#define RENC_CLK_FACTOR  1.000000f
#define RENC_CLK_OFFSET  0.000000f
#define RENC_CLK_DTYPE UNSIGNED
#define RENC_CLK_SBIT 4
#define RENC_CLK_BLEN 1

#define RENC_ACLK_FACTOR  1.000000f
#define RENC_ACLK_OFFSET  0.000000f
#define RENC_ACLK_DTYPE UNSIGNED
#define RENC_ACLK_SBIT 5
#define RENC_ACLK_BLEN 1

#define RENC_SW_FACTOR  1.000000f
#define RENC_SW_OFFSET  0.000000f
#define RENC_SW_DTYPE UNSIGNED
#define RENC_SW_SBIT 6
#define RENC_SW_BLEN 1


union GUI_SMF
{
  uint32_t all;
  struct GUI_SMF_segments
  {

    uint32_t SCRN_NO : 4; // Factor = 1.000000, Offset = 0.000000
    uint32_t RENC_CLK : 1; // Factor = 1.000000, Offset = 0.000000
    uint32_t RENC_ACLK : 1; // Factor = 1.000000, Offset = 0.000000
    uint32_t RENC_SW : 1; // Factor = 1.000000, Offset = 0.000000
    uint32_t rotation_counter: 12;
    uint32_t switch_counter: 12;

  }GUI_SMF_segments;

}GUI_SMF;

/*  Frame Name : LED_CNTRL   */


#define LED_CNTRL_FID     2

#define LED_OVRRD_FACTOR  1.000000f
#define LED_OVRRD_OFFSET  0.000000f
#define LED_OVRRD_DTYPE UNSIGNED
#define LED_OVRRD_SBIT 0
#define LED_OVRRD_BLEN 1

#define XLED_mode_FACTOR  1.000000f
#define XLED_mode_OFFSET  0.000000f
#define XLED_mode_DTYPE UNSIGNED
#define XLED_mode_SBIT 1
#define XLED_mode_BLEN 3

#define YLED_mode_FACTOR  1.000000f
#define YLED_mode_OFFSET  0.000000f
#define YLED_mode_DTYPE UNSIGNED
#define YLED_mode_SBIT 4
#define YLED_mode_BLEN 3

#define ZLED_mode_FACTOR  1.000000f
#define ZLED_mode_OFFSET  0.000000f
#define ZLED_mode_DTYPE UNSIGNED
#define ZLED_mode_SBIT 7
#define ZLED_mode_BLEN 3

#define XLED_state_FACTOR  1.000000f
#define XLED_state_OFFSET  0.000000f
#define XLED_state_DTYPE UNSIGNED
#define XLED_state_SBIT 10
#define XLED_state_BLEN 1

#define YLED_state_FACTOR  1.000000f
#define YLED_state_OFFSET  0.000000f
#define YLED_state_DTYPE UNSIGNED
#define YLED_state_SBIT 11
#define YLED_state_BLEN 1

#define ZLED_state_FACTOR  1.000000f
#define ZLED_state_OFFSET  0.000000f
#define ZLED_state_DTYPE UNSIGNED
#define ZLED_state_SBIT 12
#define ZLED_state_BLEN 1

#define TLED_OVRRD_FACTOR  1.000000f
#define TLED_OVRRD_OFFSET  0.000000f
#define TLED_OVRRD_DTYPE UNSIGNED
#define TLED_OVRRD_SBIT 13
#define TLED_OVRRD_BLEN 1

#define TLED_mode_FACTOR  1.000000f
#define TLED_mode_OFFSET  0.000000f
#define TLED_mode_DTYPE UNSIGNED
#define TLED_mode_SBIT 14
#define TLED_mode_BLEN 1

#define XLED_Brightness_FACTOR  3.125000f
#define XLED_Brightness_OFFSET  0.000000f
#define XLED_Brightness_DTYPE UNSIGNED
#define XLED_Brightness_SBIT 15
#define XLED_Brightness_BLEN 5

#define YLED_Brightness_FACTOR  3.125000f
#define YLED_Brightness_OFFSET  0.000000f
#define YLED_Brightness_DTYPE UNSIGNED
#define YLED_Brightness_SBIT 20
#define YLED_Brightness_BLEN 5

#define ZLED_Brightness_FACTOR  3.125000f
#define ZLED_Brightness_OFFSET  0.000000f
#define ZLED_Brightness_DTYPE UNSIGNED
#define ZLED_Brightness_SBIT 25
#define ZLED_Brightness_BLEN 5


union LED_CNTRL_SMF
{
  uint32_t all;
  struct LED_CNTRL_SMF_segments
  {

    uint32_t LED_OVRRD : 1; // Factor = 1.000000, Offset = 0.000000
    uint32_t XLED_mode : 3; // Factor = 1.000000, Offset = 0.000000
    uint32_t YLED_mode : 3; // Factor = 1.000000, Offset = 0.000000
    uint32_t ZLED_mode : 3; // Factor = 1.000000, Offset = 0.000000
    uint32_t XLED_state : 1; // Factor = 1.000000, Offset = 0.000000
    uint32_t YLED_state : 1; // Factor = 1.000000, Offset = 0.000000
    uint32_t ZLED_state : 1; // Factor = 1.000000, Offset = 0.000000
    uint32_t TLED_OVRRD : 1; // Factor = 1.000000, Offset = 0.000000
    uint32_t TLED_mode : 1; // Factor = 1.000000, Offset = 0.000000
    uint32_t XLED_Brightness : 5; // Factor = 1.000000, Offset = 0.000000
    uint32_t YLED_Brightness : 5; // Factor = 1.000000, Offset = 0.000000
    uint32_t ZLED_Brightness : 5; // Factor = 1.000000, Offset = 0.000000

  }LED_CNTRL_SMF_segments;

}LED_CNTRL_SMF;

/*  Frame Name : IMU_X_Raw   */


#define IMU_X_Raw_FID     3

#define XACCEL_raw_FACTOR  1.000000f
#define XACCEL_raw_OFFSET  0.000000f
#define XACCEL_raw_DTYPE UNSIGNED
#define XACCEL_raw_SBIT 0
#define XACCEL_raw_BLEN 16

#define XGYRO_raw_FACTOR  1.000000f
#define XGYRO_raw_OFFSET  0.000000f
#define XGYRO_raw_DTYPE UNSIGNED
#define XGYRO_raw_SBIT 16
#define XGYRO_raw_BLEN 16


union IMU_X_Raw_SMF
{
  uint32_t all;
  struct IMU_X_Raw_SMF_segments
  {

    uint32_t XACCEL_raw : 16; // Factor = 1.000000, Offset = 0.000000
    uint32_t XGYRO_raw : 16; // Factor = 1.000000, Offset = 0.000000

  }IMU_X_Raw_SMF_segments;

}IMU_X_Raw_SMF;

/*  Frame Name : IMU_Y_Raw   */


#define IMU_Y_Raw_FID     4

#define YACCEL_raw_FACTOR  1.000000f
#define YACCEL_raw_OFFSET  0.000000f
#define YACCEL_raw_DTYPE UNSIGNED
#define YACCEL_raw_SBIT 0
#define YACCEL_raw_BLEN 16

#define YGYRO_raw_FACTOR  1.000000f
#define YGYRO_raw_OFFSET  0.000000f
#define YGYRO_raw_DTYPE UNSIGNED
#define YGYRO_raw_SBIT 16
#define YGYRO_raw_BLEN 16


union IMU_Y_Raw_SMF
{
  uint32_t all;
  struct IMU_Y_Raw_SMF_segments
  {

    uint32_t YACCEL_raw : 16; // Factor = 1.000000, Offset = 0.000000
    uint32_t YGYRO_raw : 16; // Factor = 1.000000, Offset = 0.000000

  }IMU_Y_Raw_SMF_segments;

}IMU_Y_Raw_SMF;

/*  Frame Name : IMU_Z_Raw   */


#define IMU_Z_Raw_FID     5

#define ZACCEL_raw_FACTOR  1.000000f
#define ZACCEL_raw_OFFSET  0.000000f
#define ZACCEL_raw_DTYPE UNSIGNED
#define ZACCEL_raw_SBIT 0
#define ZACCEL_raw_BLEN 16

#define ZGYRO_raw_FACTOR  1.000000f
#define ZGYRO_raw_OFFSET  0.000000f
#define ZGYRO_raw_DTYPE UNSIGNED
#define ZGYRO_raw_SBIT 16
#define ZGYRO_raw_BLEN 16


union IMU_Z_Raw_SMF
{
  uint32_t all;
  struct IMU_Z_Raw_SMF_segments
  {

    uint32_t ZACCEL_raw : 16; // Factor = 1.000000, Offset = 0.000000
    uint32_t ZGYRO_raw : 16; // Factor = 1.000000, Offset = 0.000000

  }IMU_Z_Raw_SMF_segments;

}IMU_Z_Raw_SMF;

/*  Frame Name : IMU_Temp   */


#define IMU_Temp_FID     6

#define IMU_Temp_Raw_FACTOR  1.000000f
#define IMU_Temp_Raw_OFFSET  0.000000f
#define IMU_Temp_Raw_DTYPE UNSIGNED
#define IMU_Temp_Raw_SBIT 0
#define IMU_Temp_Raw_BLEN 16

#define IMU_Temp_Processed_FACTOR  1.000000f
#define IMU_Temp_Processed_OFFSET  0.000000f
#define IMU_Temp_Processed_DTYPE UNSIGNED
#define IMU_Temp_Processed_SBIT 16
#define IMU_Temp_Processed_BLEN 16


union IMU_Temp_SMF
{
  uint32_t all;
  struct IMU_Temp_SMF_segments
  {

    uint32_t IMU_Temp_Raw : 16; // Factor = 1.000000, Offset = 0.000000
    uint32_t IMU_Temp_Processed : 16; // Factor = 1.000000, Offset = 0.000000

  }IMU_Temp_SMF_segments;

}IMU_Temp_SMF;

/*  Frame Name : IMU_XACCEL_Processed   */


#define IMU_XACCEL_Processed_FID     7

#define IMU_XACCEL_Processed_FACTOR  1.000000f
#define IMU_XACCEL_Processed_OFFSET  0.000000f
#define IMU_XACCEL_Processed_DTYPE SIGNED
#define IMU_XACCEL_Processed_SBIT 0
#define IMU_XACCEL_Processed_BLEN 32


union IMU_XACCEL_Processed_SMF
{
  uint32_t all;
  struct IMU_XACCEL_Processed_SMF_segments
  {

    uint32_t IMU_XACCEL_Processed : 32; // Factor = 1.000000, Offset = 0.000000

  }IMU_XACCEL_Processed_SMF_segments;

}IMU_XACCEL_Processed_SMF;

/*  Frame Name : IMU_YACCEL_Processed   */


#define IMU_YACCEL_Processed_FID     8

#define IMU_YACCEL_Processed_FACTOR  1.000000f
#define IMU_YACCEL_Processed_OFFSET  0.000000f
#define IMU_YACCEL_Processed_DTYPE SIGNED
#define IMU_YACCEL_Processed_SBIT 0
#define IMU_YACCEL_Processed_BLEN 32


union IMU_YACCEL_Processed_SMF
{
  uint32_t all;
  struct IMU_YACCEL_Processed_SMF_segments
  {

    uint32_t IMU_YACCEL_Processed : 32; // Factor = 1.000000, Offset = 0.000000

  }IMU_YACCEL_Processed_SMF_segments;

}IMU_YACCEL_Processed_SMF;

/*  Frame Name : IMU_ZACCEL_Processed   */


#define IMU_ZACCEL_Processed_FID     9

#define IMU_ZACCEL_Processed_FACTOR  1.000000f
#define IMU_ZACCEL_Processed_OFFSET  0.000000f
#define IMU_ZACCEL_Processed_DTYPE SIGNED
#define IMU_ZACCEL_Processed_SBIT 0
#define IMU_ZACCEL_Processed_BLEN 32


union IMU_ZACCEL_Processed_SMF
{
  uint32_t all;
  struct IMU_ZACCEL_Processed_SMF_segments
  {

    uint32_t IMU_ZACCEL_Processed : 32; // Factor = 1.000000, Offset = 0.000000

  }IMU_ZACCEL_Processed_SMF_segments;

}IMU_ZACCEL_Processed_SMF;

/*  Frame Name : IMU_XGYRO_Processed   */


#define IMU_XGYRO_Processed_FID     10

#define IMU_XGYRO_Processed_FACTOR  1.000000f
#define IMU_XGYRO_Processed_OFFSET  0.000000f
#define IMU_XGYRO_Processed_DTYPE SIGNED
#define IMU_XGYRO_Processed_SBIT 0
#define IMU_XGYRO_Processed_BLEN 32


union IMU_XGYRO_Processed_SMF
{
  uint32_t all;
  struct IMU_XGYRO_Processed_SMF_segments
  {

    uint32_t IMU_XGYRO_Processed : 32; // Factor = 1.000000, Offset = 0.000000

  }IMU_XGYRO_Processed_SMF_segments;

}IMU_XGYRO_Processed_SMF;

/*  Frame Name : IMU_YGYRO_Processed   */


#define IMU_YGYRO_Processed_FID     11

#define IMU_YGYRO_Processed_FACTOR  1.000000f
#define IMU_YGYRO_Processed_OFFSET  0.000000f
#define IMU_YGYRO_Processed_DTYPE SIGNED
#define IMU_YGYRO_Processed_SBIT 0
#define IMU_YGYRO_Processed_BLEN 32


union IMU_YGYRO_Processed_SMF
{
  uint32_t all;
  struct IMU_YGYRO_Processed_SMF_segments
  {

    uint32_t IMU_YGYRO_Processed : 32; // Factor = 1.000000, Offset = 0.000000

  }IMU_YGYRO_Processed_SMF_segments;

}IMU_YGYRO_Processed_SMF;

/*  Frame Name : IMU_ZGYRO_Processed   */


#define IMU_ZGYRO_Processed_FID     12

#define IMU_ZGYRO_Processed_FACTOR  1.000000f
#define IMU_ZGYRO_Processed_OFFSET  0.000000f
#define IMU_ZGYRO_Processed_DTYPE SIGNED
#define IMU_ZGYRO_Processed_SBIT 0
#define IMU_ZGYRO_Processed_BLEN 32


union IMU_ZGYRO_Processed_SMF
{
  uint32_t all;
  struct IMU_ZGYRO_Processed_SMF_segments
  {

    uint32_t IMU_ZGYRO_Processed : 32; // Factor = 1.000000, Offset = 0.000000

  }IMU_ZGYRO_Processed_SMF_segments;

}IMU_ZGYRO_Processed_SMF;

/*  Frame Name : Roll   */


#define Roll_FID     13

#define Roll_Accel_FACTOR  0.195600f
#define Roll_Accel_OFFSET  0.000000f
#define Roll_Accel_DTYPE SIGNED
#define Roll_Accel_SBIT 0
#define Roll_Accel_BLEN 10

#define Roll_Gyro_FACTOR  0.195600f
#define Roll_Gyro_OFFSET  0.000000f
#define Roll_Gyro_DTYPE SIGNED
#define Roll_Gyro_SBIT 10
#define Roll_Gyro_BLEN 10

#define Roll_CF_FACTOR  0.195600f
#define Roll_CF_OFFSET  0.000000f
#define Roll_CF_DTYPE SIGNED
#define Roll_CF_SBIT 20
#define Roll_CF_BLEN 10


union Roll_SMF
{
  uint32_t all;
  struct Roll_SMF_segments
  {

    uint32_t Roll_Accel : 10; // Factor = 0.195600, Offset = 0.000000
    uint32_t Roll_Gyro : 10; // Factor = 0.195600, Offset = 0.000000
    uint32_t Roll_CF : 10; // Factor = 0.195600, Offset = 0.000000

  }Roll_SMF_segments;

}Roll_SMF;

/*  Frame Name : Pitch   */


#define Pitch_FID     14

#define Pitch_Accel_FACTOR  0.195600f
#define Pitch_Accel_OFFSET  0.000000f
#define Pitch_Accel_DTYPE SIGNED
#define Pitch_Accel_SBIT 0
#define Pitch_Accel_BLEN 10

#define Pitch_Gyro_FACTOR  0.195600f
#define Pitch_Gyro_OFFSET  0.000000f
#define Pitch_Gyro_DTYPE SIGNED
#define Pitch_Gyro_SBIT 10
#define Pitch_Gyro_BLEN 10

#define Pitch_CF_FACTOR  0.195600f
#define Pitch_CF_OFFSET  0.000000f
#define Pitch_CF_DTYPE SIGNED
#define Pitch_CF_SBIT 20
#define Pitch_CF_BLEN 10


union Pitch_SMF
{
  uint32_t all;
  struct Pitch_SMF_segments
  {

    uint32_t Pitch_Accel : 10; // Factor = 0.195600, Offset = 0.000000
    uint32_t Pitch_Gyro : 10; // Factor = 0.195600, Offset = 0.000000
    uint32_t Pitch_CF : 10; // Factor = 0.195600, Offset = 0.000000

  }Pitch_SMF_segments;

}Pitch_SMF;

/*  Frame Name : Yaw   */


#define Yaw_FID     15

#define Yaw_Accel_FACTOR  0.195600f
#define Yaw_Accel_OFFSET  0.000000f
#define Yaw_Accel_DTYPE SIGNED
#define Yaw_Accel_SBIT 0
#define Yaw_Accel_BLEN 10

#define Yaw_Gyro_FACTOR  0.195600f
#define Yaw_Gyro_OFFSET  0.000000f
#define Yaw_Gyro_DTYPE SIGNED
#define Yaw_Gyro_SBIT 10
#define Yaw_Gyro_BLEN 10

#define Yaw_CF_FACTOR  0.195600f
#define Yaw_CF_OFFSET  0.000000f
#define Yaw_CF_DTYPE SIGNED
#define Yaw_CF_SBIT 20
#define Yaw_CF_BLEN 10


union Yaw_SMF
{
  uint32_t all;
  struct Yaw_SMF_segments
  {

    uint32_t Yaw_Accel : 10; // Factor = 0.195600, Offset = 0.000000
    uint32_t Yaw_Gyro : 10; // Factor = 0.195600, Offset = 0.000000
    uint32_t Yaw_CF : 10; // Factor = 0.195600, Offset = 0.000000

  }Yaw_SMF_segments;

}Yaw_SMF;

/*  Frame Name : CF_coeff   */


#define CF_coeff_FID     16

#define Roll_coeff_FACTOR  0.000977f
#define Roll_coeff_OFFSET  0.000000f
#define Roll_coeff_DTYPE UNSIGNED
#define Roll_coeff_SBIT 0
#define Roll_coeff_BLEN 10

#define Pitch_coeff_FACTOR  0.000977f
#define Pitch_coeff_OFFSET  0.000000f
#define Pitch_coeff_DTYPE UNSIGNED
#define Pitch_coeff_SBIT 10
#define Pitch_coeff_BLEN 10

#define Yaw_coeff_FACTOR  0.000977f
#define Yaw_coeff_OFFSET  0.000000f
#define Yaw_coeff_DTYPE UNSIGNED
#define Yaw_coeff_SBIT 20
#define Yaw_coeff_BLEN 10


union CF_coeff_SMF
{
  uint32_t all;
  struct CF_coeff_SMF_segments
  {

    uint32_t Roll_coeff : 10; // Factor = 0.000977, Offset = 0.000000
    uint32_t Pitch_coeff : 10; // Factor = 0.000977, Offset = 0.000000
    uint32_t Yaw_coeff : 10; // Factor = 0.000977, Offset = 0.000000

  }CF_coeff_SMF_segments;

}CF_coeff_SMF;

/*  Frame Name : IMU_settings   */


#define IMU_settings_FID     17

#define Calibrate_CS_FACTOR  1.000000f
#define Calibrate_CS_OFFSET  0.000000f
#define Calibrate_CS_DTYPE UNSIGNED
#define Calibrate_CS_SBIT 0
#define Calibrate_CS_BLEN 1

#define RPY_accel_EN_CS_FACTOR  1.000000f
#define RPY_accel_EN_CS_OFFSET  0.000000f
#define RPY_accel_EN_CS_DTYPE UNSIGNED
#define RPY_accel_EN_CS_SBIT 1
#define RPY_accel_EN_CS_BLEN 1

#define RPYA_gyro_EN_CS_FACTOR  1.000000f
#define RPYA_gyro_EN_CS_OFFSET  0.000000f
#define RPYA_gyro_EN_CS_DTYPE UNSIGNED
#define RPYA_gyro_EN_CS_SBIT 2
#define RPYA_gyro_EN_CS_BLEN 1

#define RPYA_CF_EN_CS_FACTOR  1.000000f
#define RPYA_CF_EN_CS_OFFSET  0.000000f
#define RPYA_CF_EN_CS_DTYPE UNSIGNED
#define RPYA_CF_EN_CS_SBIT 3
#define RPYA_CF_EN_CS_BLEN 1

#define GYRO_FS_FACTOR  1.000000f
#define GYRO_FS_OFFSET  0.000000f
#define GYRO_FS_DTYPE UNSIGNED
#define GYRO_FS_SBIT 4
#define GYRO_FS_BLEN 3

#define ACCEL_FS_FACTOR  1.000000f
#define ACCEL_FS_OFFSET  0.000000f
#define ACCEL_FS_DTYPE UNSIGNED
#define ACCEL_FS_SBIT 7
#define ACCEL_FS_BLEN 3

#define Sensor_Output_Mode_FACTOR  1.000000f
#define Sensor_Output_Mode_OFFSET  0.000000f
#define Sensor_Output_Mode_DTYPE UNSIGNED
#define Sensor_Output_Mode_SBIT 10
#define Sensor_Output_Mode_BLEN 2


#define write_accel_config_SBIT	12
#define write_gyro_config_SBIT	13
#define write_rpy_accel_config_SBIT 14
#define write_rpy_gyro_config_SBIT 15
#define write_rpy_filter_config_SBIT 16


union IMU_settings_SMF
{
  uint32_t all;
  struct IMU_settings_SMF_segments
  {

    uint32_t Calibrate_CS : 1; // Factor = 1.000000, Offset = 0.000000
    uint32_t RPY_accel_EN_CS : 1; // Factor = 1.000000, Offset = 0.000000
    uint32_t RPYA_gyro_EN_CS : 1; // Factor = 1.000000, Offset = 0.000000
    uint32_t RPYA_CF_EN_CS : 1; // Factor = 1.000000, Offset = 0.000000
    uint32_t GYRO_FS : 3; // Factor = 1.000000, Offset = 0.000000
    uint32_t ACCEL_FS : 3; // Factor = 1.000000, Offset = 0.000000
    uint32_t Sensor_Output_Mode : 2; // Factor = 1.000000, Offset = 0.000000
    uint32_t write_accel_config: 1;
    uint32_t write_gyro_config: 1;
    uint32_t write_ryp_accel_config: 1;
    uint32_t write_ryp_gyro_config: 1;
    uint32_t write_ryp_filter_config: 1;

  }IMU_settings_SMF_segments;

}IMU_settings_SMF;

/*  Frame Name : IMU_CALIB_XACCEL_Offset   */


#define IMU_CALIB_XACCEL_Offset_FID     18

#define X_accel_offset_FACTOR  1.000000f
#define X_accel_offset_OFFSET  0.000000f
#define X_accel_offset_DTYPE SIGNED
#define X_accel_offset_SBIT 0
#define X_accel_offset_BLEN 32


union IMU_CALIB_XACCEL_Offset_SMF
{
  uint32_t all;
  struct IMU_CALIB_XACCEL_Offset_SMF_segments
  {

    uint32_t X_accel_offset : 32; // Factor = 1.000000, Offset = 0.000000

  }IMU_CALIB_XACCEL_Offset_SMF_segments;

}IMU_CALIB_XACCEL_Offset_SMF;

/*  Frame Name : IMU_CALIB_YACCEL_Offset   */


#define IMU_CALIB_YACCEL_Offset_FID     19

#define Y_accel_offset_FACTOR  1.000000f
#define Y_accel_offset_OFFSET  0.000000f
#define Y_accel_offset_DTYPE SIGNED
#define Y_accel_offset_SBIT 0
#define Y_accel_offset_BLEN 32


union IMU_CALIB_YACCEL_Offset_SMF
{
  uint32_t all;
  struct IMU_CALIB_YACCEL_Offset_SMF_segments
  {

    uint32_t Y_accel_offset : 32; // Factor = 1.000000, Offset = 0.000000

  }IMU_CALIB_YACCEL_Offset_SMF_segments;

}IMU_CALIB_YACCEL_Offset_SMF;

/*  Frame Name : IMU_CALIB_ZACCEL_Offset   */


#define IMU_CALIB_ZACCEL_Offset_FID     20

#define Z_accel_offset_FACTOR  1.000000f
#define Z_accel_offset_OFFSET  0.000000f
#define Z_accel_offset_DTYPE SIGNED
#define Z_accel_offset_SBIT 0
#define Z_accel_offset_BLEN 32


union IMU_CALIB_ZACCEL_Offset_SMF
{
  uint32_t all;
  struct IMU_CALIB_ZACCEL_Offset_SMF_segments
  {

    uint32_t Z_accel_offset : 32; // Factor = 1.000000, Offset = 0.000000

  }IMU_CALIB_ZACCEL_Offset_SMF_segments;

}IMU_CALIB_ZACCEL_Offset_SMF;

/*  Frame Name : IMU_CALIB_XGYRO_Offset   */


#define IMU_CALIB_XGYRO_Offset_FID     21

#define X_gyro_offset_FACTOR  1.000000f
#define X_gyro_offset_OFFSET  0.000000f
#define X_gyro_offset_DTYPE SIGNED
#define X_gyro_offset_SBIT 0
#define X_gyro_offset_BLEN 32


union IMU_CALIB_XGYRO_Offset_SMF
{
  uint32_t all;
  struct IMU_CALIB_XGYRO_Offset_SMF_segments
  {

    uint32_t X_gyro_offset : 32; // Factor = 1.000000, Offset = 0.000000

  }IMU_CALIB_XGYRO_Offset_SMF_segments;

}IMU_CALIB_XGYRO_Offset_SMF;

/*  Frame Name : IMU_CALIB_YGYRO_Offset   */


#define IMU_CALIB_YGYRO_Offset_FID     22

#define Y_gyro_offset_FACTOR  1.000000f
#define Y_gyro_offset_OFFSET  0.000000f
#define Y_gyro_offset_DTYPE SIGNED
#define Y_gyro_offset_SBIT 0
#define Y_gyro_offset_BLEN 32


union IMU_CALIB_YGYRO_Offset_SMF
{
  uint32_t all;
  struct IMU_CALIB_YGYRO_Offset_SMF_segments
  {

    uint32_t Y_gyro_offset : 32; // Factor = 1.000000, Offset = 0.000000

  }IMU_CALIB_YGYRO_Offset_SMF_segments;

}IMU_CALIB_YGYRO_Offset_SMF;

/*  Frame Name : IMU_CALIB_ZGYRO_Offset   */


#define IMU_CALIB_ZGYRO_Offset_FID     23

#define Z_gyro_offset_FACTOR  1.000000f
#define Z_gyro_offset_OFFSET  0.000000f
#define Z_gyro_offset_DTYPE SIGNED
#define Z_gyro_offset_SBIT 0
#define Z_gyro_offset_BLEN 32


union IMU_CALIB_ZGYRO_Offset_SMF
{
  uint32_t all;
  struct IMU_CALIB_ZGYRO_Offset_SMF_segments
  {

    uint32_t Z_gyro_offset : 32; // Factor = 1.000000, Offset = 0.000000

  }IMU_CALIB_ZGYRO_Offset_SMF_segments;

}IMU_CALIB_ZGYRO_Offset_SMF;

/*  Frame Name : IMU_CALIB_XGYRO_bias_drift   */


#define IMU_CALIB_XGYRO_bias_drift_FID     24

#define x_gyro_bias_drift_FACTOR  1.000000f
#define x_gyro_bias_drift_OFFSET  0.000000f
#define x_gyro_bias_drift_DTYPE SIGNED
#define x_gyro_bias_drift_SBIT 0
#define x_gyro_bias_drift_BLEN 32


union IMU_CALIB_XGYRO_bias_drift_SMF
{
  uint32_t all;
  struct IMU_CALIB_XGYRO_bias_drift_SMF_segments
  {

    uint32_t x_gyro_bias_drift : 32; // Factor = 1.000000, Offset = 0.000000

  }IMU_CALIB_XGYRO_bias_drift_SMF_segments;

}IMU_CALIB_XGYRO_bias_drift_SMF;

/*  Frame Name : IMU_CALIB_YGYRO_bias_drift   */


#define IMU_CALIB_YGYRO_bias_drift_FID     25

#define y_gyro_bias_drift_FACTOR  1.000000f
#define y_gyro_bias_drift_OFFSET  0.000000f
#define y_gyro_bias_drift_DTYPE SIGNED
#define y_gyro_bias_drift_SBIT 0
#define y_gyro_bias_drift_BLEN 32


union IMU_CALIB_YGYRO_bias_drift_SMF
{
  uint32_t all;
  struct IMU_CALIB_YGYRO_bias_drift_SMF_segments
  {

    uint32_t y_gyro_bias_drift : 32; // Factor = 1.000000, Offset = 0.000000

  }IMU_CALIB_YGYRO_bias_drift_SMF_segments;

}IMU_CALIB_YGYRO_bias_drift_SMF;

/*  Frame Name : IMU_CALIB_ZGYRO_bias_drift   */


#define IMU_CALIB_ZGYRO_bias_drift_FID     26

#define z_gyro_bias_drift_FACTOR  1.000000f
#define z_gyro_bias_drift_OFFSET  0.000000f
#define z_gyro_bias_drift_DTYPE SIGNED
#define z_gyro_bias_drift_SBIT 0
#define z_gyro_bias_drift_BLEN 32


union IMU_CALIB_ZGYRO_bias_drift_SMF
{
  uint32_t all;
  struct IMU_CALIB_ZGYRO_bias_drift_SMF_segments
  {

    uint32_t z_gyro_bias_drift : 32; // Factor = 1.000000, Offset = 0.000000

  }IMU_CALIB_ZGYRO_bias_drift_SMF_segments;

}IMU_CALIB_ZGYRO_bias_drift_SMF;


/* Frame Name: Motion Detection Threshold; X axis. */

#define MDT_X_FID	27

#define X_angle_threshold_FACTOR	0.01000f
#define X_angle_threshold_OFFSET	0.00000f
#define X_angle_threshold_DTYPE     UNSIGNED
#define X_angle_threshold_SBIT 		0
#define X_angle_threshold_BLEN		16

#define X_vibration_threshold_FACTOR	1.00000f
#define X_vibration_threshold_OFFSET	0.00000f
#define X_vibration_threshold_DTYPE     UNSIGNED
#define X_vibration_threshold_SBIT 		16
#define X_vibration_threshold_BLEN		16

union MDT_X_SMF
{
  uint32_t all;

  struct MDT_X_SMF_segments
  {
	uint32_t angle_threshold : 16;
	uint32_t vibration_threshold : 16;

  }MDT_X_SMF_segments;

}MDT_X_SMF;


#define MDT_Y_FID	28

#define Y_angle_threshold_FACTOR	0.01000f
#define Y_angle_threshold_OFFSET	0.00000f
#define Y_angle_threshold_DTYPE     UNSIGNED
#define Y_angle_threshold_SBIT 		0
#define Y_angle_threshold_BLEN		16

#define Y_vibration_threshold_FACTOR	1.00000f
#define Y_vibration_threshold_OFFSET	0.00000f
#define Y_vibration_threshold_DTYPE     UNSIGNED
#define Y_vibration_threshold_SBIT 		16
#define Y_vibration_threshold_BLEN		16

union MDT_Y_SMF
{
  uint32_t all;

  struct MDT_Y_SMF_segments
  {
	uint32_t angle_threshold : 16;
	uint32_t vibration_threshold : 16;

  }MDT_Y_SMF_segments;

}MDT_Y_SMF;



#define MDT_Z_FID	29

#define Z_angle_threshold_FACTOR	0.01000f
#define Z_angle_threshold_OFFSET	0.00000f
#define Z_angle_threshold_DTYPE     UNSIGNED
#define Z_angle_threshold_SBIT 		0
#define Z_angle_threshold_BLEN		16

#define Z_vibration_threshold_FACTOR	1.00000f
#define Z_vibration_threshold_OFFSET	0.00000f
#define Z_vibration_threshold_DTYPE     UNSIGNED
#define Z_vibration_threshold_SBIT 		16
#define Z_vibration_threshold_BLEN		16

union MDT_Z_SMF
{
  uint32_t all;

  struct MDT_Z_SMF_segments
  {
	uint32_t angle_threshold : 16;
	uint32_t vibration_threshold : 16;

  }MDT_Z_SMF_segments;

}MDT_Z_SMF;


/* Frame Name: Motion Detection. */

#define Motion_Detection_FID	30

union Motion_Detection_SMF
{
  uint32_t all;

  struct Motion_Detection_SMF_segments
  {

    uint32_t X_angle_detected : 1;
	uint32_t X_vibration_detected: 1;
    uint32_t Y_angle_detected : 1;
	uint32_t Y_vibration_detected: 1;
    uint32_t Z_angle_detected : 1;
	uint32_t Z_vibration_detected: 1;

  }Motion_Detection_SMF_segments;

}Motion_Detection_SMF;

/* Frame Name: IMU calibration settings. */

#define IMU_calibration_FID	31

union IMU_calibration_SMF
{
  uint32_t all;

  struct IMU_calibration_SMF_segments
  {

    uint32_t interval : 8;
	uint32_t points   : 8;
    uint32_t axis     : 3;

  }IMU_calibration_SMF_segments;

}IMU_calibration_SMF;


/*  Frame Name : Task_counter   */

#define Task_counter_FID     127

#define Task_counter_FACTOR  1.000000f
#define Task_counter_OFFSET  0.000000f
#define Task_counter_DTYPE UNSIGNED
#define Task_counter_SBIT 0
#define Task_counter_BLEN 32


union Task_counter_SMF
{
  uint32_t all;
  struct Task_counter_SMF_segments
  {

    uint32_t Task_counter : 32; // Factor = 1.000000, Offset = 0.000000

  }Task_counter_SMF_segments;

}Task_counter_SMF;

/*  Frame Name : END   */

union END_SMF
{
  uint32_t all;
  struct END_SMF_segments
  {


  }END_SMF_segments;

}END_SMF;



/* Frame Name: Config_RW. */

#define Config_RW_FID		50
#define save_config_SBIT	0
#define load_config_SBIT	1

union Config_RW_SMF
{
  uint32_t all;

  struct Config_RW_SMF_segments
  {
	  uint32_t save_config: 1;
	  uint32_t load_config: 1;

  }Config_RW_SMF_segments;
}Config_RW_SMF;

#endif
