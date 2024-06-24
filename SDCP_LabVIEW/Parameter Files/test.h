

/*  Frame Name : GUI   */


#define GUI_FID     1
#define SCRN_NO_FACTOR  1.000000f
#define SCRN_NO_OFFSET  0.000000f
#define RENC_CLK_FACTOR  1.000000f
#define RENC_CLK_OFFSET  0.000000f
#define RENC_ACLK_FACTOR  1.000000f
#define RENC_ACLK_OFFSET  0.000000f
#define RENC_SW_FACTOR  1.000000f
#define RENC_SW_OFFSET  0.000000f


union GUI_SMF
{
  uint32_t all;
  struct GUI_SMF_segments
  {

    uint32_t SCRN_NO : 4; // Factor = 1.000000, Offset = 0.000000
    uint32_t RENC_CLK : 1; // Factor = 1.000000, Offset = 0.000000
    uint32_t RENC_ACLK : 1; // Factor = 1.000000, Offset = 0.000000
    uint32_t RENC_SW : 1; // Factor = 1.000000, Offset = 0.000000

  }GUI_SMF_segments;

}GUI_SMF;

/*  Frame Name : LED_CNTRL   */


#define LED_CNTRL_FID     2
#define LED_OVRRD_FACTOR  1.000000f
#define LED_OVRRD_OFFSET  0.000000f
#define XLED_PWM_Mode_FACTOR  1.000000f
#define XLED_PWM_Mode_OFFSET  0.000000f
#define YLED_PWM_Mode_FACTOR  1.000000f
#define YLED_PWM_Mode_OFFSET  0.000000f
#define ZLED_PWM_Mode_FACTOR  1.000000f
#define ZLED_PWM_Mode_OFFSET  0.000000f
#define XLED_state_FACTOR  1.000000f
#define XLED_state_OFFSET  0.000000f
#define YLED_state_FACTOR  1.000000f
#define YLED_state_OFFSET  0.000000f
#define ZLED_state_FACTOR  1.000000f
#define ZLED_state_OFFSET  0.000000f
#define TLED_OVRRD_FACTOR  1.000000f
#define TLED_OVRRD_OFFSET  0.000000f
#define TLED_mode_FACTOR  1.000000f
#define TLED_mode_OFFSET  0.000000f
#define XLED_Brightness_FACTOR  1.000000f
#define XLED_Brightness_OFFSET  0.000000f
#define YLED_Brightness_FACTOR  1.000000f
#define YLED_Brightness_OFFSET  0.000000f
#define ZLED_Brightness_FACTOR  1.000000f
#define ZLED_Brightness_OFFSET  0.000000f


union LED_CNTRL_SMF
{
  uint32_t all;
  struct LED_CNTRL_SMF_segments
  {

    uint32_t LED_OVRRD : 1; // Factor = 1.000000, Offset = 0.000000
    uint32_t XLED_PWM_Mode : 1; // Factor = 1.000000, Offset = 0.000000
    uint32_t YLED_PWM_Mode : 1; // Factor = 1.000000, Offset = 0.000000
    uint32_t ZLED_PWM_Mode : 1; // Factor = 1.000000, Offset = 0.000000
    uint32_t XLED_state : 1; // Factor = 1.000000, Offset = 0.000000
    uint32_t YLED_state : 1; // Factor = 1.000000, Offset = 0.000000
    uint32_t ZLED_state : 1; // Factor = 1.000000, Offset = 0.000000
    uint32_t TLED_OVRRD : 1; // Factor = 1.000000, Offset = 0.000000
    uint32_t TLED_mode : 1; // Factor = 1.000000, Offset = 0.000000
    uint32_t XLED_Brightness : 7; // Factor = 1.000000, Offset = 0.000000
    uint32_t YLED_Brightness : 7; // Factor = 1.000000, Offset = 0.000000
    uint32_t ZLED_Brightness : 7; // Factor = 1.000000, Offset = 0.000000

  }LED_CNTRL_SMF_segments;

}LED_CNTRL_SMF;

/*  Frame Name : IMU_X_Raw   */


#define IMU_X_Raw_FID     3
#define XACCEL_raw_FACTOR  1.000000f
#define XACCEL_raw_OFFSET  0.000000f
#define XGYRO_raw_FACTOR  1.000000f
#define XGYRO_raw_OFFSET  0.000000f


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
#define YGYRO_raw_FACTOR  1.000000f
#define YGYRO_raw_OFFSET  0.000000f


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
#define ZGYRO_raw_FACTOR  1.000000f
#define ZGYRO_raw_OFFSET  0.000000f


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
#define IMU_Temp_Processed_FACTOR  1.000000f
#define IMU_Temp_Processed_OFFSET  0.000000f


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
#define Roll_Gyro_FACTOR  0.195600f
#define Roll_Gyro_OFFSET  0.000000f
#define Roll_CF_FACTOR  0.195600f
#define Roll_CF_OFFSET  0.000000f


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
#define Pitch_Gyro_FACTOR  0.195600f
#define Pitch_Gyro_OFFSET  0.000000f
#define Pitch_CF_FACTOR  0.195600f
#define Pitch_CF_OFFSET  0.000000f


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
#define Yaw_Gyro_FACTOR  0.195600f
#define Yaw_Gyro_OFFSET  0.000000f
#define Yaw_CF_FACTOR  0.195600f
#define Yaw_CF_OFFSET  0.000000f


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
#define Pitch_coeff_FACTOR  0.000977f
#define Pitch_coeff_OFFSET  0.000000f
#define Yaw_coeff_FACTOR  0.000977f
#define Yaw_coeff_OFFSET  0.000000f


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
#define RPY_accel_EN_CS_FACTOR  1.000000f
#define RPY_accel_EN_CS_OFFSET  0.000000f
#define RPYA_gyro_EN_CS_FACTOR  1.000000f
#define RPYA_gyro_EN_CS_OFFSET  0.000000f
#define RPYA_CF_EN_CS_FACTOR  1.000000f
#define RPYA_CF_EN_CS_OFFSET  0.000000f
#define GYRO_FS_FACTOR  1.000000f
#define GYRO_FS_OFFSET  0.000000f
#define ACCEL_FS_FACTOR  1.000000f
#define ACCEL_FS_OFFSET  0.000000f
#define Sensor_Output_Mode_FACTOR  1.000000f
#define Sensor_Output_Mode_OFFSET  0.000000f


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

  }IMU_settings_SMF_segments;

}IMU_settings_SMF;

/*  Frame Name : IMU_CALIB_XACCEL_Offset   */


#define IMU_CALIB_XACCEL_Offset_FID     18
#define X_accel_offset_FACTOR  1.000000f
#define X_accel_offset_OFFSET  0.000000f


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


union IMU_CALIB_ZGYRO_bias_drift_SMF
{
  uint32_t all;
  struct IMU_CALIB_ZGYRO_bias_drift_SMF_segments
  {

    uint32_t z_gyro_bias_drift : 32; // Factor = 1.000000, Offset = 0.000000

  }IMU_CALIB_ZGYRO_bias_drift_SMF_segments;

}IMU_CALIB_ZGYRO_bias_drift_SMF;

/*  Frame Name : Task_counter   */


#define Task_counter_FID     127
#define Task_counter_FACTOR  1.000000f
#define Task_counter_OFFSET  0.000000f


union Task_counter_SMF
{
  uint32_t all;
  struct Task_counter_SMF_segments
  {

    uint32_t Task_counter : 32; // Factor = 1.000000, Offset = 0.000000

  }Task_counter_SMF_segments;

}Task_counter_SMF;

/*  Frame Name : END   */


#define END_FID     0


union END_SMF
{
  uint32_t all;
  struct END_SMF_segments
  {


  }END_SMF_segments;

}END_SMF;