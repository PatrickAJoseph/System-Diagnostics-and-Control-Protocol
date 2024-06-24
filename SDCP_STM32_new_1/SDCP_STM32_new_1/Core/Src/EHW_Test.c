

#include "EHW_Test.h"

/* Function to initialize hardware test for rotary encoder. */

void EHW_RotaryEncoder_Test_Init()
{
  Nokia5110.updated = 0;
  Nokia5110.x_pos = 0;
  Nokia5110.y_pos = 0;

  for(int i = 0 ; i < 504; i++)
  {
    Nokia5110.byte[i] = 0;
  }

  Nokia5110_updateDisplay();

  //while(!Nokia5110.updated);

  //Nokia5110.updated = 0;
}

/* Function to perform hardware test for rotary encoder. */

void EHW_RotaryEncoder_Test()
{
  if(Rotary_Encoder.parameters.clockwise || Rotary_Encoder.parameters.anticlockwise || Rotary_Encoder.parameters.switch_state )
  {
	Nokia5110.x_pos = 0;
	Nokia5110.y_pos = 0;

	Nokia5110_printString("RCNT=");
	Nokia5110_printU16((uint16_t)Rotary_Encoder.parameters.rotation_counter);

    Nokia5110.x_pos = 0;
    Nokia5110.y_pos = 16;

	Nokia5110_printString("CLK=");
	Nokia5110_printU16((uint16_t)Rotary_Encoder.parameters.clockwise);

    Nokia5110.x_pos = 0;
    Nokia5110.y_pos = 24;

	Nokia5110_printString("ACLK=");
	Nokia5110_printU16((uint16_t)Rotary_Encoder.parameters.anticlockwise);

	Rotary_Encoder.parameters.clockwise = 0;
    Rotary_Encoder.parameters.anticlockwise = 0;

	Nokia5110.x_pos = 0;
	Nokia5110.y_pos = 8;

	Nokia5110_printString("SWCNTR=");
	Nokia5110_printU16((uint16_t)Rotary_Encoder.parameters.switch_counter);

	Nokia5110.x_pos = 0;
	Nokia5110.y_pos = 32;

	Nokia5110_printString("SW=");
	Nokia5110_printU16((uint16_t)Rotary_Encoder.parameters.switch_state);

	if( Nokia5110.updated )
	{
	 Nokia5110_updateDisplay();
	 Nokia5110.updated = 0;
	}

    Rotary_Encoder.parameters.switch_state = 0;
  }
}


void EHW_IMU_Test_Init()
{
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN_Msk;
  GPIOA->MODER |= ( 0x1 << ( 2 * 5 ) );
}


void EHW_IMU_Test()
{
  //MPU6050_readSensorData();
  uint8_t error = MPU6050_readAllSensorData();

  Nokia5110.x_pos = 0;
  Nokia5110.y_pos = 0;
  Nokia5110_printString("X_A=");
  Nokia5110_printI32(imu.accelerometer[X]);

  Nokia5110.x_pos = 0;
  Nokia5110.y_pos = 8;
  Nokia5110_printString("Y_A=");
  Nokia5110_printI32(imu.accelerometer[Y]);

  Nokia5110.x_pos = 0;
  Nokia5110.y_pos = 16;
  Nokia5110_printString("Z_A=");
  Nokia5110_printI32(imu.accelerometer[Z]);

  Nokia5110.x_pos = 0;
  Nokia5110.y_pos = 24;
  Nokia5110_printString("X_G=");
  Nokia5110_printI32(imu.gyroscope[X]);

  Nokia5110.x_pos = 0;
  Nokia5110.y_pos = 32;
  Nokia5110_printString("Y_G=");
  Nokia5110_printI32(imu.gyroscope[Y]);

  Nokia5110.x_pos = 0;
  Nokia5110.y_pos = 40;
  Nokia5110_printString("Z_G=");
  Nokia5110_printI32(imu.gyroscope[Z]);

  Nokia5110_updateDisplay();

  System_SysTick_delay_ms(250);

  if( error )
  {
    GPIOA->ODR |= ( 1 << 5 );
    //MPU6050_errorHandler();
  }
  else
  {
    GPIOA->ODR &= ~( 1 << 5 );
  }
}
