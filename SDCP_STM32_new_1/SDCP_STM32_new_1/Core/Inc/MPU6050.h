
#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_

#include "main.h"
#include "System.h"
#include "Timing_Measurement.h"

#include "SDCP.h"
#include "SMFs.h"
#include "HPF.h"

/* Self Test X register. */

#define SELF_TEST_X				13

#define XA_TEST_4_2_Pos			5
#define XA_TEST_4_2_Msk			( 0x3 << XA_TEST_4_2_Pos )
#define XG_TEST_4_0_Pos			0
#define XG_TEST_4_0_Msk			( 0x1F << XG_TEST_4_0_Pos )

/* Self Test Y register. */

#define SELF_TEST_Y				14

#define YA_TEST_4_2_Pos			5
#define YA_TEST_4_2_Msk			( 0x3 << YA_TEST_4_2_Pos )
#define YG_TEST_4_0_Pos			0
#define YG_TEST_4_0_Msk			( 0x1F << YG_TEST_4_0_Pos )

/* Self Test Z register. */

#define SELF_TEST_Z				15

#define ZA_TEST_4_2_Pos			5
#define ZA_TEST_4_2_Msk			( 0x3 << ZA_TEST_4_2_Pos )
#define ZG_TEST_4_0_Pos			0
#define ZG_TEST_4_0_Msk			( 0x1F << ZG_TEST_4_0_Pos )

/* Self Test A register. */

#define SELF_TEST_A				16

#define XA_TEST_1_0_Pos			4
#define XA_TEST_1_0_Msk			(0x3 << XA_TEST_1_0_Pos)
#define YA_TEST_1_0_Pos			2
#define YA_TEST_1_0_Msk			(0x3 << YA_TEST_1_0_Pos)
#define ZA_TEST_1_0_Pos			0
#define ZA_TEST_1_0_Msk			(0x3 << ZA_TEST_1_0_Pos)

/* Sample rate divider register. */

#define SMPLRT_DIV				25

/* Configuration register. */

#define CONFIG					26

#define DLPF_CONFIG_Pos			0
#define DLPF_CONFIG_Msk			(0x7 << DLPF_CONFIG_Pos)
#define EXT_SYNC_SET_Pos		3
#define EXT_SYNC_SET_Msk		(0x7 << EXT_SYNC_SET_Pos)

/* Gyroscope configuration register. */

#define GYRO_CONFIG				27

#define FS_SEL_Pos				3
#define FS_SEL_Msk				(0x3 << FS_SEL_Pos)
#define ZG_ST_Pos				5
#define ZG_ST_Msk				(0x1 << ZG_ST_Pos)
#define YG_ST_Pos				6
#define YG_ST_Msk				(0x1 << YG_ST_Pos)
#define XG_ST_Pos				7
#define XG_ST_Msk				(0x1 << XG_ST_Pos)

#define GYRO_FS_250_DPS			0
#define GYRO_FS_500_DPS			1
#define GYRO_FS_1000_DPS		2
#define GYRO_FS_2000_DPS		3

/* Accelerometer configuration register. */

#define ACCEL_CONFIG			28

#define AFS_SEL_Pos				3
#define AFS_SEL_Msk				(0x3 << AFS_SEL_Pos)
#define ZA_ST_Pos				5
#define ZA_ST_Msk				(0x1 << ZA_ST_Pos)
#define YA_ST_Pos				6
#define YA_ST_Msk				(0x1 << YA_ST_Pos)
#define XA_ST_Pos				7
#define XA_ST_Msk				(0x1 << XA_ST_Pos)

#define ACCEL_FS_2_G			0
#define ACCEL_FS_4_G			1
#define ACCEL_FS_8_G		    2
#define ACCEL_FS_16_G		    3

/* Interrupt pin configuration. */

#define INT_PIN_CFG				55

#define I2C_BYPASS_EN_Pos		1
#define I2C_BYPASS_EN_Msk		( 0x1 << I2C_BYPASS_EN_Pos )
#define FSYNC_INT_EN_Pos		2
#define FSYNC_INT_EN_Msk		( 0x1 << FSYNC_INT_EN_Pos )
#define FSYNC_INT_LEVEL_Pos		3
#define FSYNC_INT_LEVEL_Msk		( 0x1 << FSYNC_INT_LEVEL_Pos )
#define INT_RD_CLEAR_Pos		4
#define INT_RD_CLEAR_Msk		( 0x1 << INT_RD_CLEAR_Pos )
#define LATCH_INT_EN_Pos		5
#define LATCH_INT_EN_Msk		( 0x1 << LATCH_INT_EN_Pos )
#define INT_OPEN_Pos			6
#define INT_OPEN_Msk			( 0x1 << INT_OPEN_Pos )
#define INT_LEVEL_Pos			7
#define INT_LEVEL_Msk			( 0x1 << INT_LEVEL_Pos )

/* Interrupt enable. */

#define INT_ENABLE				56

#define DATA_RDY_EN_Pos			0
#define DATA_RDY_EN_Msk			( 0x1 << DATA_RDY_EN_Pos )
#define I2C_MST_INT_EN_Pos		3
#define I2C_MST_INT_EN_Msk		( 0x1 << I2C_MST_INT_EN_Pos )
#define FIFO_OFLOW_EN_Pos		4
#define FIFO_OFLOW_EN_Msk		( 0x1 << FIFO_OFLOW_EN_Pos )

/* Interrupt status. */

#define INT_STATUS				58

#define DATA_RDY_INT_Pos		0
#define DATA_RDY_INT_Msk		( 0x1 << DATA_RDY_INT_Pos )
#define I2C_MST_INT_Pos			3
#define I2C_MST_INT_Msk			( 0x1 << I2C_MST_INT_Pos )
#define FIFO_OFLOW_INT_Pos		4
#define FIFO_OFLOW_INT_Msk		( 0x1 << FIFO_OFLOW_INT_Pos )

/* Accelerometer X,Y and Z registers. */

#define ACCEL_XOUT_H			59
#define ACCEL_XOUT_L			60
#define ACCEL_YOUT_H			61
#define ACCEL_YOUT_L			62
#define ACCEL_ZOUT_H			63
#define ACCEL_ZOUT_L			64

/* Temperature registers. */

#define TEMP_OUT_H				65
#define TEMP_OUT_L				66

/* Gyroscope X,Y and Z registers. */

#define GYRO_XOUT_H				67
#define GYRO_XOUT_L				68
#define GYRO_YOUT_H				69
#define GYRO_YOUT_L				70
#define GYRO_ZOUT_H				71
#define GYRO_ZOUT_L				72

/* Signal path reset. */

#define SIGNAL_PATH_RESET		104

#define TEMP_RESET_Pos			0
#define TEMP_RESET_Msk			( 0x1 << TEMP_RESET_Pos )
#define ACCEL_RESET_Pos			1
#define ACCEL_RESET_Msk			( 0x1 << ACCEL_RESET_Pos )
#define GYRO_RESET_Pos			2
#define GYRO_RESET_Msk			( 0x1 << GYRO_RESET_Pos )

/* User control. */

#define USER_CTRL				105

#define SIG_COND_RESET_Pos		0
#define SIG_COND_RESET_Msk		( 0x1 << SIGN_COND_RESET_Pos )
#define I2C_MST_RESET_Pos		1
#define I2C_MST_RESET_Msk		( 0x1 << I2C_MST_RESET_Pos )
#define FIFO_RESET_Pos			2
#define FIFO_RESET_Msk			( 0x1 << FIFO_RESET_Pos )
#define I2C_IF_DIS_Pos			4
#define I2C_IF_DIS_Msk			( 0x1 << I2C_IF_DIS_Pos )
#define I2C_MST_EN_Pos			5
#define I2C_MST_EN_Msk			( 0x1 << I2C_MST_EN_Pos )
#define FIFO_EN_Pos				6
#define FIFO_EN_Msk				( 0x1 << FIFO_EN_Pos )

/* Power management 1. */

#define PWR_MGMT_1				107

#define CLKSEL_Pos				0
#define CLKSEL_Msk				( 0x7 << CLKSEL_Pos )
#define	TEMP_DIS_Pos			3
#define TEMP_DIS_Msk			( 0x1 << TEMP_DIS_Pos )
#define CYCLE_Pos				5
#define CYCLE_Msk				( 0x1 << CYCLE_Pos )
#define SLEEP_Pos				6
#define SLEEP_Msk				( 0x1 << SLEEP_Pos )
#define DEVICE_RESET_Pos		7
#define DEVICE_RESET_Msk		( 0x1 << DEVICE_RESET_Pos )

/* Power management 2. */

#define PWR_MGMT_2				108

#define STBY_ZG_Pos				0
#define STBY_ZG_Msk				( 0x1 << STBY_ZG_Pos )
#define STBY_YG_Pos				1
#define STBY_YG_Msk				( 0x1 << STBY_YG_Pos )
#define STBY_XG_Pos				2
#define STBY_XG_Msk				( 0x1 << STBY_XG_Pos )
#define STBY_ZA_Pos				3
#define STBY_ZA_Msk				( 0x1 << STBY_ZA_Pos )
#define STBY_YA_Pos				4
#define STBY_YA_Msk				( 0x1 << STBY_YA_Pos )
#define STBY_XA_Pos				5
#define STBY_XA_Msk				( 0x1 << STBY_XA_Pos )
#define LP_WAKE_CTRL_Pos		6
#define LP_WAKE_CTRL_Msk		( 0x3 << LP_WAKE_CTRL_Pos )

#define WHO_AM_I				117

typedef enum
{
	ACCEL_FS_2G,
	ACCEL_FS_4G,
	ACCEL_FS_8G,
	ACCEL_FS_16G
}ACCEL_FS;

typedef enum
{
	GYRO_FS_250DPS,
	GYRO_FS_500DPS,
	GYRO_FS_1000DPS,
	GYRO_FS_2000DPS
}GYRO_FS;

typedef enum
{
  X = 0,
  Y = 1,
  Z = 2
}AXES_t;

typedef enum
{
	X_Positive = 0,
	X_Negative = 1,
	Y_Positive = 2,
	Y_Negative = 3,
	Z_Positive = 4,
	Z_Negative = 5
}Calibration_Axis;

/* Structure for IMU sensor. */

struct imu
{
  uint32_t device_address;

  union disable
  {
	uint32_t all;

	struct sensors
	{
      uint32_t z_gyro : 1;
      uint32_t y_gyro : 1;
      uint32_t x_gyro : 1;
      uint32_t z_accel : 1;
      uint32_t y_accel : 1;
      uint32_t x_accel : 1;
      uint32_t reserved: 26;
	}sensors;

  }disable;

  union sensor_configurations
  {
	uint32_t all;

    struct sensor_configuration_segments
    {
		uint32_t accel_fs_config:3;
		uint32_t gyro_fs_config:3;
		uint32_t enable_DLPF:1;
		uint32_t sampling_rate:16;
		uint32_t reserved:9;
    }sensor_configuration_segments;

  }sensor_configurations;

  uint16_t accelerometer_raw[3];
  uint16_t gyroscope_raw[3];

  int32_t accelerometer[3];
  int32_t gyroscope[3];

  int32_t accelerometer_offset[3];
  int32_t gyroscope_offset[3];

  uint8_t N_calibration_points;
  uint8_t calibration_interval_ms;
  uint8_t calibration_axis;

}imu;

void I2C1_Init();
uint8_t I2C1_Start();
uint8_t I2C1_MasterReadRequest(uint8_t slaveID);
uint8_t I2C1_writeByte(uint8_t slaveID, uint8_t data);
uint8_t I2C1_Read(uint8_t slaveID, uint8_t nBytes, uint8_t* buffer);
uint8_t I2C1_Write(uint8_t slaveID, uint8_t nBytes, uint8_t* buffer);
uint8_t I2C1_Stop();

void MPU6050_Init();
void MPU6050_updateConfig();
void MPU6050_readSensorData();
uint8_t MPU6050_readAllSensorData();
void MPU6050_calibrate(Calibration_Axis axis, uint8_t nPoints);
uint8_t MPU6050_read_register( uint8_t device_address, uint8_t register_address );
uint8_t MPU6050_write_register( uint8_t device_address, uint8_t register_address, uint8_t data );
void MPU6050_errorHandler();
void Gyro_HPF_Init();

//#define GYRO_HPF_EN

#ifdef GYRO_HPF_EN

HPF Gx_filter;
HPF Gy_filter;
HPF Gz_filter;

#endif /* GYRO_HPF_EN */

#endif /* INC_MPU6050_H_ */
