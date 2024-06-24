

#include "MPU6050.h"
#include "Configurations.h"

#define I2C_TIMEOUT_MS		100

void I2C1_Init()
{
	/* I2C1 GPIO initializations. */

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN_Msk;

	GPIOB->MODER    |= ( 0x2 << ( 2 * 8 ) )			//	PB8 : (I2C1_SCL) AFR
					|  ( 0x2 << ( 2 * 9 ) );		//	PB9 : (I2C1_SDA) AFR

	GPIOB->OTYPER	|=	( 1 << 8 ) | ( 1 << 9 );	//	Enable open drain configuration.

	GPIOB->OSPEEDR |= ( 0x3 << ( 2 * 8 ) ) | ( 0x3 << ( 2 * 9 ) );

	GPIOB->AFR[1] 	|= ( 0x4 << ( 4 * 0 ) );		//	PB8 : I2C1_SCL.
	GPIOB->AFR[1] 	|= ( 0x4 << ( 4 * 1 ) );		//	PB9 : I2C1_SDA.

	GPIOB->PUPDR |= ( 0x1 << ( 2 * 8 ) ) | ( 0x1 << ( 2 * 9 ) );	//	Enable pull-up resistors.

	/* I2C1 peripheral initializations. */

	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN_Msk;

	/*	Reset I2C peripheral. */

	I2C1->CR1 |= I2C_CR1_SWRST_Msk;

	I2C1->CR1 &= ~I2C_CR1_SWRST_Msk;

	/* Set peripheral clock frequency to 40 MHz. */

	I2C1->CR2 |= (40UL << I2C_CR2_FREQ_Pos);

	/* Set I2C peripheral in full speed mode and set duty cycle to 16:9 mode. */

	I2C1->CCR |= ( I2C_CCR_FS_Msk | I2C_CCR_DUTY_Msk );

	/* Select I2C clock frequency. */
	/* For full-scale mode, T_HIGH = 9 * CLK * T_BASE and T_LOW = 16 * CLK * T_BASE.
	 * Hence, T_I2C = T_HIGH + T_LOW = 25 * CLK * T_BASE.
	 * For I2C clock frequency of 375 kHz and T_BASE as 40 MHz,
	 *      (1/375kHz) = 25 * CLK * ( 1 / 40MHz ) --> CLK = (40MHz/375kHz)*(1/25) = 4.266
	 * Now CCR value is approximated to 5 as CCR = 4 gives 400 kHz which is close to
	 * the maximum fast-mode I2C clock frequency limit. With CCR = 5, the obtained clock frequency
	 * is 320 kHz which is well below the 400kHz frequency margin.
	 *
	 * It was observed that 320 kHz clock frequency caused issues with communication. With CCR = 6,
	 * the I2C clock frequency is 266.66 kHz.
	 * */

	I2C1->CCR |= (6UL << I2C_CCR_CCR_Pos);

	/* Configure rise time to 1 us --> t_RISE = t_periperal * ( 1 + T_RISE ). */

	I2C1->TRISE = 40;

	/*	Enable I2C peripheral. */

	I2C1->CR1 |= I2C_CR1_PE_Msk;
}


uint8_t MPU6050_read_register( uint8_t device_address, uint8_t register_address )
{
	uint8_t temp = 0;

	temp = temp;				//	To avoid warning from compiler.

	uint8_t data = 0;


	/*	Enable ACK bit. */

	//I2C1->CR1 |= I2C_CR1_ACK_Msk;

	/* Start transmission. */

	I2C1->CR1 |= I2C_CR1_START_Msk;

	/*	Wait until transmission of the start bit is done. */

	while( ( I2C1->SR1 & I2C_SR1_SB_Msk ) == 0 );

	/* Write data register with address. */

	I2C1->DR = device_address << 1;

	/*	Wait until address is transmitted. */

	while( ( I2C1->SR1 & I2C_SR1_ADDR_Msk ) == 0 );

	temp = I2C1->SR1 | I2C1->SR2;

	/* Wait until data register is empty. */

	while( ( I2C1->SR1 & I2C_SR1_TXE_Msk ) == 0 );

	/* Write register address. */

	I2C1->DR = register_address;

	/* Wait until byte is transferred. */

	while( ( I2C1->SR1 & I2C_SR1_BTF_Msk ) == 0 );

	/* Generate start condition. */

	I2C1->CR1 |= I2C_CR1_START_Msk;

	/*	Wait until transmission of the start bit is done. */

	while( ( I2C1->SR1 & I2C_SR1_SB_Msk ) == 0 );

	/* Write data register with address. */

	I2C1->DR = (device_address << 1) + 1;

	/*	Wait until address is transmitted. */

	while( ( I2C1->SR1 & I2C_SR1_ADDR_Msk ) == 0 );

	temp = I2C1->SR1 | I2C1->SR2;

	/* Wait until data received. */

	while( ( I2C1->SR1 & I2C_SR1_RXNE_Msk ) == 0 );

	//while( ( I2C1->SR1 & I2C_SR1_BTF_Msk ) == 0 );

	/* Read data. */

	data = I2C1->DR;

	/* Generate NACK condition. */

	I2C1->CR1 &= ~I2C_CR1_ACK_Msk;

	temp = I2C1->SR1 | I2C1->SR2;

	I2C1->CR1 |= I2C_CR1_STOP_Msk;

	return(data);
}


uint8_t I2C1_Start()
{
  uint32_t startTime = 0;
  uint32_t currentTime = 0;
  uint8_t timeoutStatus = 0;

  //Ticks_setTicks(Ticks_1ms);

  /* Enable acknowledgement bit. */

  I2C1->CR1 |= I2C_CR1_ACK_Msk;

  /* Generate start condition. */

  I2C1->CR1 |= I2C_CR1_START_Msk;

  /* Wait until start bit is transferred. */

  startTime = Ticks_TicksToUs(TIM5->CNT);

  /* Wait until SB flag is set. */

  while( !(I2C1->SR1 & I2C_SR1_SB_Msk) && ( currentTime < (1000 * I2C_TIMEOUT_MS) ) )
  {
    currentTime = ( Ticks_TicksToUs(TIM5->CNT) - startTime );
  }

  if( currentTime >= ( 1000 * I2C_TIMEOUT_MS ) )
  {
    timeoutStatus = 1;
  }
  else
  {
    timeoutStatus = 0;
  }

  return(timeoutStatus);
}

uint8_t I2C1_MasterReadRequest(uint8_t slaveID)
{
  uint32_t startTime = 0;
  uint32_t currentTime = 0;
  uint8_t timeoutStatus = 0;
  uint8_t tempU8 = 0;

  /* Generate I2C start condition. */

  I2C1_Start();

  /* Load 7-bit address. */

  I2C1->DR = ( (slaveID << 1 ) | 0x01 );

  /* Wait until the address bit is set. */

  startTime = Ticks_TicksToUs(TIM5->CNT);

  while( !(I2C1->SR1 & I2C_SR1_ADDR_Msk) && ( currentTime < (I2C_TIMEOUT_MS*1000) ) )
  {
    currentTime = ( Ticks_TicksToUs(TIM5->CNT) - startTime );
  }

  /* Clear address bit by reading SR1 and SR2 registers. */

  //tempU8 = ( I2C1->SR1 | I2C1->SR2 );

  if( currentTime >= (I2C_TIMEOUT_MS*1000) )
  {
    return(1);
  }

  return(0);
}


uint8_t I2C1_Poll_RxNE()
{
  uint32_t startTime = Ticks_TicksToUs(TIM5->CNT);
  uint32_t currentTime = 0;

  while( !( I2C1->SR1 & I2C_SR1_RXNE_Msk ) && ( currentTime < ( 1000 * I2C_TIMEOUT_MS ) ) )
  {
    currentTime = ( Ticks_TicksToUs(TIM5->CNT) - startTime );
  }

  if( currentTime >= ( 1000 * I2C_TIMEOUT_MS ) )
  {
    return(0x01);
  }

  return(0x00);
}


uint8_t I2C1_Poll_BTF()
{
  uint32_t startTime = Ticks_TicksToUs(TIM5->CNT);
  uint32_t currentTime = 0;

  while( !( I2C1->SR1 & I2C_SR1_BTF_Msk ) && ( currentTime < ( 1000 * I2C_TIMEOUT_MS ) ) )
  {
    currentTime = ( Ticks_TicksToUs(TIM5->CNT) - startTime );
  }

  if( currentTime >= ( 1000 * I2C_TIMEOUT_MS ) )
  {
    return(0x01);
  }

  return(0x00);
}

uint8_t I2C1_Read(uint8_t slaveID, uint8_t nBytes, uint8_t* buffer)
{
  uint8_t tempU8 = 0;

  /* Check if the peripheral is enabled. */

  if( I2C1->CR1 & I2C_CR1_PE_Msk )
  {

  }
  else
  {
    I2C1->CR1 |= I2C_CR1_PE_Msk;
  }

  /* Clear POS bit in CR1 register. */

  I2C1->CR1 &= (~I2C_CR1_POS_Msk);

  /* Request master read request. */

  if( I2C1_MasterReadRequest(slaveID) )
  {
    return(0x01);  /* Timeout event for I2C master read request. */
  }

  /* If number of bytes is zero, immediately generate stop condition. */

  if( nBytes == 0 )
  {
	/* Clear address bit by reading SR1 followed by SR2. */

	tempU8 = ( I2C1->SR1 | I2C1->SR2 );

    I2C1->CR1 |= I2C_CR1_STOP_Msk;
  }
  else if( nBytes == 1 )
  {
	/* Clear acknowlegment bit. */

	I2C1->CR1 &= (~I2C_CR1_ACK_Msk);

	/* Clear address flag by reading SR2 register followed by SR1 register. */

	tempU8 = ( I2C1->SR1 | I2C1->SR2 );

	/* Generate stop condition. */

	I2C1->CR1 |= I2C_CR1_STOP_Msk;
  }
  else if( nBytes == 2 )
  {
    /* Clear acknowlegment bit. */

	I2C1->CR1 &= (~I2C_CR1_ACK_Msk);

	/* Enable POS bit. */

	I2C1->CR1 |= I2C_CR1_POS_Msk;

	/* Clear address bit. */

	tempU8 = ( I2C1->SR1 | I2C1->SR2 );
  }
  else if( nBytes > 2 )
  {
	/* Enable acknowledgement bit. */

    I2C1->CR1 |= I2C_CR1_ACK_Msk;

    /* Clear address bit. */

    tempU8 = ( I2C1->SR1 | I2C1->SR2 );
  }


  uint8_t position = 0;

  while( nBytes )
  {
    /* Case 1: Number of bytes to be received is lesser than or equal to 3. */

	if( nBytes <= 3 )
	{
	  /* Case 1(a): Number of bytes to be received equals 1. */

      if( nBytes == 1 )
      {
    	/* Check if receive buffer is not empty. Check for timeout also. */

        if( I2C1_Poll_RxNE() )
        {
          return(0x01);
        }

        /* Read byte from the data register. */

        buffer[position] = (I2C1->DR);

        /* Increment buffer position and decrement the number of bytes to be read. */

        position++;
        nBytes--;

      }/* End of case 1(a). */
      else if( nBytes == 2 )   /* Case 1(b): Number of bytes to be received equals 2. */
      {
        /* Poll the byte transfer complete bit. */

        if( I2C1_Poll_BTF() )
        {
          return(0x01);  // Timeout.
        }

        /* Generate stop condition. */

        I2C1->CR1 |= I2C_CR1_STOP_Msk;

        /* Read data from buffer. */

        buffer[position] = (I2C1->DR);

        /* Increase buffer position and decrement number of bytes to receive. */

        position++;
        nBytes--;

        /* Read once more from the data register. */

        buffer[position] = (I2C1->DR);

        /* Increment position counter and decrement number of bytes to be received. */

        position++;
        nBytes--;

      }/* End of case 1(b). */
      else if( nBytes == 3 )     /* If number of pending bytes to be read is 3. */
      {
        /* Poll the byte transfer complete flag. */

    	if( I2C1_Poll_BTF() )
    	{
    	  return(0x01);
    	}

    	/* Disable acknowledgement bit. */

    	I2C1->CR1 &= (~I2C_CR1_ACK_Msk);

    	/* Read data register and update position and counter. */

    	buffer[position] = (I2C1->DR);

    	position++;
    	nBytes--;

    	/* Poll byte transfer complete bit. */

    	if( I2C1_Poll_BTF() )
    	{
    	  return(0x01);
    	}

    	/* Generate stop condition. */

    	I2C1->CR1 |= I2C_CR1_STOP_Msk;

    	/* Read the data register twice. */

    	buffer[position] = (I2C1->DR);  position++; nBytes--;
    	buffer[position] = (I2C1->DR);  position++; nBytes--;
      }/* End of case 1(b).*/

	}/* End of case 1. */
	else if( nBytes > 3 )  /* Case 2: Number of bytes to be received > 3. */
	{
	  /* Poll RxNE bit */

      if( I2C1_Poll_RxNE() )
      {
        return(0x01);
      }

      /* Read data register. */

      buffer[position] = (I2C1->DR); position++; nBytes--;

      /* Check if byte transfer complete bit is set. */

      if( I2C1->SR1 & I2C_SR1_BTF_Msk )
      {
    	/* If number of pending bytes to be received is 3, generate NACK. */

    	if( nBytes == 3 )
    	{
    	  I2C1->CR1 &= (~I2C_CR1_ACK_Msk);
    	}

    	/* Read data register. */

    	buffer[position] = (I2C1->DR); position++; nBytes--;
      }
	}

  }/* End of I2C data reading. */

  return(0x00);

}/* End of function. */


uint8_t I2C1_writeByte(uint8_t slaveID, uint8_t data)
{
	uint32_t startTime = 0;
	uint32_t currentTime = 0;
    uint8_t timeoutStatus = 0;


	/**** Start transmission. ****/

	uint8_t temp = 0;

	temp = temp;		//	to avoid warning from compiler.

	/*	Enable ACK bit. */

	I2C1->CR1 |= I2C_CR1_ACK_Msk;

	/* Start transmission. */

	I2C1->CR1 |= I2C_CR1_START_Msk;


	startTime = Ticks_TicksToUs(TIM5->CNT);

	/*	Wait until transmission of the start bit is done. */

	while( ( ( I2C1->SR1 & I2C_SR1_SB_Msk ) == 0 ) && ( currentTime < ( 1000 * I2C_TIMEOUT_MS ) ) )
	{
	  currentTime = ( Ticks_TicksToUs(TIM5->CNT) - startTime );
	}

	if( currentTime >= ( 1000 * I2C_TIMEOUT_MS ) )
	{
	  timeoutStatus = 1;
	}

	/**** Connect to device. ****/

	/* Write data register with address. */

	I2C1->DR = (slaveID << 1);

	/*	Wait until address is transmitted. */

	//while( ( I2C1->SR1 & I2C_SR1_ADDR_Msk ) == 0 );

	while( ( ( I2C1->SR1 & I2C_SR1_ADDR_Msk ) == 0 ) && ( currentTime < ( 1000 * I2C_TIMEOUT_MS ) ) )
	{
	  currentTime = ( Ticks_TicksToUs(TIM5->CNT) - startTime );
	}

	if( currentTime >= ( 1000 * I2C_TIMEOUT_MS ) )
	{
	  timeoutStatus = 1;
	}


	temp = I2C1->SR1 | I2C1->SR2;


	/**** Send register address. ****/

	//while( ( I2C1->SR1 & I2C_SR1_TXE_Msk ) == 0 );

	while( ( ( I2C1->SR1 & I2C_SR1_TXE_Msk ) == 0 ) && ( currentTime < ( 1000 * I2C_TIMEOUT_MS ) ) )
	{
	  currentTime = ( Ticks_TicksToUs(TIM5->CNT) - startTime );
	}

	if( currentTime >= ( 1000 * I2C_TIMEOUT_MS ) )
	{
	  timeoutStatus = 1;
	}


	/* Write data. */

	I2C1->DR = data;

	/* Wait until data register is empty. */

	//while( ( I2C1->SR1 & I2C_SR1_BTF_Msk ) == 0 );

	while( ( ( I2C1->SR1 & I2C_SR1_BTF_Msk ) == 0 ) && ( currentTime < ( 1000 * I2C_TIMEOUT_MS ) ) )
	{
	  currentTime = ( Ticks_TicksToUs(TIM5->CNT) - startTime );
	}

	if( currentTime >= ( 1000 * I2C_TIMEOUT_MS ) )
	{
	  timeoutStatus = 1;
	}


	/*	Wait for acknowledgement from slave. */

	//while( ( I2C1->CR1 & I2C_CR1_ACK_Msk ) == 0 );

	while( ( ( I2C1->CR1 & I2C_CR1_ACK_Msk ) == 0 ) && ( currentTime < ( 1000 * I2C_TIMEOUT_MS ) ) )
	{
	  currentTime = ( Ticks_TicksToUs(TIM5->CNT) - startTime );
	}

	if( currentTime >= ( 1000 * I2C_TIMEOUT_MS ) )
	{
	  timeoutStatus = 1;
	}


	/* Clear the ACK bit. */

	I2C1->CR1 &= (~I2C_CR1_ACK_Msk);

	/* Generate STOP condition. */

	I2C1->CR1 |= I2C_CR1_STOP_Msk;

	return(timeoutStatus);
}

uint8_t MPU6050_read_registers( uint8_t device_address, uint8_t register_address,
		                        uint8_t nBytes, uint8_t* buffer)
{
	uint8_t temp = 0;

	temp = temp;				//	To avoid warning from compiler.

	uint8_t data = 0;

    /* Clear Pos bit. */

	I2C1->CR1 &= (~I2C_CR1_POS_Msk);

	/*	Enable ACK bit. */

	I2C1->CR1 |= I2C_CR1_ACK_Msk;

	/* Start transmission. */

	I2C1->CR1 |= I2C_CR1_START_Msk;

	/*	Wait until transmission of the start bit is done. */

	while( !( I2C1->SR1 & I2C_SR1_SB_Msk ) );

	/* Write data register with address with read operation. */

	I2C1->DR = ( (device_address << 1) | 0x01 );

	/*	Wait until address is transmitted. */

	while( ( I2C1->SR1 & I2C_SR1_ADDR_Msk ) == 0 );

	/* Clear address flag. */

	temp = I2C1->SR1 | I2C1->SR2;

    if( nBytes == 0 )
    {
      while( !(I2C1->SR1 & I2C_SR1_ADDR_Msk) );
      I2C1->CR1 |= I2C_CR1_STOP_Msk;
    }
    else if( nBytes == 1 )
    {

    }

	/* Wait until data register is empty. */

	while( ( I2C1->SR1 & I2C_SR1_TXE_Msk ) == 0 );

	/* Write register address. */

	I2C1->DR = register_address;

	/* Wait until byte is transferred. */

	while( ( I2C1->SR1 & I2C_SR1_BTF_Msk ) == 0 );

	/* Generate start condition. */

	I2C1->CR1 |= I2C_CR1_START_Msk;

	/*	Wait until transmission of the start bit is done. */

	while( ( I2C1->SR1 & I2C_SR1_SB_Msk ) == 0 );

	/* Read from device. */

	I2C1->DR = ((device_address << 1) + 1);

	/*	Wait until address is transmitted. */

	while( ( I2C1->SR1 & I2C_SR1_ADDR_Msk ) == 0 );

	temp = I2C1->SR1 | I2C1->SR2;

	uint8_t counter = 0;

	while( counter < (nBytes-1) )
	{
	  /* Wait until data received. */

	  while( ( I2C1->SR1 & I2C_SR1_RXNE_Msk ) == 0 );

	  /* Read data. */

	  buffer[counter] = I2C1->DR;

	  //temp = I2C1->SR1 | I2C1->SR2;

	  //I2C1->CR1 |= I2C_CR1_START_Msk;

	  I2C1->CR1 |= I2C_CR1_ACK_Msk;

	  counter++;
	}

	while( ( I2C1->SR1 & I2C_SR1_RXNE_Msk ) == 0 );

	/* Read data. */

	buffer[counter] = I2C1->DR;

	/* Generate NACK condition. */

	I2C1->CR1 &= ~I2C_CR1_ACK_Msk;

	temp = I2C1->SR1 | I2C1->SR2;

	I2C1->CR1 |= I2C_CR1_STOP_Msk;

	return(data);
}

uint8_t MPU6050_write_register( uint8_t device_address, uint8_t register_address, uint8_t data )
{
	/**** Start transmission. ****/

	uint8_t temp = 0;

	temp = temp;		//	to avoid warning from compiler.

	/*	Enable ACK bit. */

	I2C1->CR1 |= I2C_CR1_ACK_Msk;

	/* Start transmission. */

	I2C1->CR1 |= I2C_CR1_START_Msk;

	/*	Wait until transmission of the start bit is done. */

	while( ( I2C1->SR1 & I2C_SR1_SB_Msk ) == 0 );

	/**** Connect to device. ****/

	/* Write data register with address. */

	I2C1->DR = device_address << 1;

	/*	Wait until address is transmitted. */

	while( ( I2C1->SR1 & I2C_SR1_ADDR_Msk ) == 0 );

	temp = I2C1->SR1 | I2C1->SR2;


	/**** Send register address. ****/

	while( ( I2C1->SR1 & I2C_SR1_TXE_Msk ) == 0 );

	/* Write register address. */

	I2C1->DR = register_address;

	/* Wait until data register is empty. */

	while( ( I2C1->SR1 & I2C_SR1_BTF_Msk ) == 0 );

	/**** Send data ****/

	while( ( I2C1->SR1 & I2C_SR1_TXE_Msk ) == 0 );

	/* Write data into register. */

	I2C1->DR = data;

	/* Wait until data register is empty. */

	while( ( I2C1->SR1 & I2C_SR1_BTF_Msk ) == 0 );

	/*	Wait for acknowledgement from slave. */

	while( ( I2C1->CR1 & I2C_CR1_ACK_Msk ) == 0 );

	/* Generate STOP condition. */

	I2C1->CR1 |= I2C_CR1_STOP_Msk;

	return(data);
}


void MPU6050_updateConfig()
{
  /* If any one of the accelerometers are configured, set the full scale range. */

  if( (!imu.disable.sensors.x_accel) || (!imu.disable.sensors.y_accel) || (!imu.disable.sensors.z_accel)  )
  {
    MPU6050_write_register( imu.device_address, ACCEL_CONFIG,
	((uint8_t)imu.sensor_configurations.sensor_configuration_segments.accel_fs_config) << AFS_SEL_Pos );
  }

  /* If any one of the gyroscopes are configured, set the full scale range. */

  if( (!imu.disable.sensors.x_gyro) || (!imu.disable.sensors.y_gyro) || (!imu.disable.sensors.z_gyro) )
  {
    MPU6050_write_register( imu.device_address, GYRO_CONFIG,
    ((uint8_t)imu.sensor_configurations.sensor_configuration_segments.gyro_fs_config) << FS_SEL_Pos );
  }

  uint16_t tempU16 = 0;

  /* Avoid zero sampling rate and set it to minimum sampling rate. */

  if( imu.sensor_configurations.sensor_configuration_segments.sampling_rate == 0 )
  {
    imu.sensor_configurations.sensor_configuration_segments.sampling_rate = 10;
  }

  /* If DLPF is enabled, the sampling rate will be reduced to 1000 Hz from 8000 Hz. */

  if( imu.sensor_configurations.sensor_configuration_segments.enable_DLPF )
  {
    tempU16 = (1000/(uint16_t)imu.sensor_configurations.sensor_configuration_segments.sampling_rate) - 1;
  }
  else
  {
    tempU16 = (8000/(uint16_t)imu.sensor_configurations.sensor_configuration_segments.sampling_rate) - 1;
  }

  MPU6050_write_register( imu.device_address, SMPLRT_DIV, tempU16 );
}

void MPU6050_Init()
{
  uint8_t __temp = 0;

  I2C1_Init();

  /* Set I2C device address to 0x68. */

  imu.device_address = 0x68;

  /* Keep all sensors enabled. */

  imu.disable.sensors.x_accel = 0;
  imu.disable.sensors.y_accel = 0;
  imu.disable.sensors.z_accel = 0;
  imu.disable.sensors.x_gyro = 0;
  imu.disable.sensors.y_gyro = 0;
  imu.disable.sensors.z_gyro = 0;

  /* Mention sensor full-scale values. */

  imu.sensor_configurations.sensor_configuration_segments.accel_fs_config =
  Config.IMU_settings.IMU_settings_parameters.accel_fs;
  imu.sensor_configurations.sensor_configuration_segments.gyro_fs_config =
  Config.IMU_settings.IMU_settings_parameters.gyro_fs;

  imu.N_calibration_points = Config.IMU_calibration.IMU_calibration_parameters.points;
  imu.calibration_interval_ms = Config.IMU_calibration.IMU_calibration_parameters.interval;
  imu.calibration_axis = Config.IMU_calibration.IMU_calibration_parameters.axis;

  IMU_settings_SMF.IMU_settings_SMF_segments.ACCEL_FS =
  Config.IMU_settings.IMU_settings_parameters.accel_fs;

  IMU_settings_SMF.IMU_settings_SMF_segments.GYRO_FS =
  Config.IMU_settings.IMU_settings_parameters.gyro_fs;

  IMU_calibration_SMF.IMU_calibration_SMF_segments.axis = imu.calibration_axis;
  IMU_calibration_SMF.IMU_calibration_SMF_segments.interval = imu.calibration_interval_ms;
  IMU_calibration_SMF.IMU_calibration_SMF_segments.points = imu.N_calibration_points;

  /* Enable DLPF. */

  imu.sensor_configurations.sensor_configuration_segments.enable_DLPF = 1;

  /* Set sensor offset values to zero. */

  imu.accelerometer_offset[X] = 0;
  imu.accelerometer_offset[Y] = 0;
  imu.accelerometer_offset[Z] = 0;
  imu.gyroscope_offset[X] = 0;
  imu.gyroscope_offset[Y] = 0;
  imu.gyroscope_offset[Z] = 0;

  /************	Write initial configurations into device.    ******************/

  /* Wake device from sleep mode. */

  MPU6050_write_register( imu.device_address, PWR_MGMT_1 , 0 );

  /* Write sensor enable and disable configurations. */

  __temp = imu.disable.all;

  //System_SysTick_delay_ms(100);

  MPU6050_write_register( imu.device_address, PWR_MGMT_2, __temp );

  System_SysTick_delay_ms(100);

  //MPU6050_updateConfig();

  MPU6050_calibrate(imu.calibration_axis,imu.N_calibration_points);

  System_SysTick_delay_ms(100);

  #ifdef GYRO_HPF_EN
    Gyro_HPF_Init();
  #endif
}


void MPU6050_readSensorData()
{
  uint16_t tempU16 = 0;
  int32_t temp_I32 = 0;
  uint8_t reg_H = 0;
  uint8_t reg_L = 0;
  int32_t gyro_fs_value = 0;
  int32_t accel_fs_value = 0;

  /* Get accelerometer full scale value. */

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

  /* Process data only for the enabled sensors. */

  if( !imu.disable.sensors.x_accel )
  {
    reg_H = MPU6050_read_register( imu.device_address, ACCEL_XOUT_H );
    reg_L = MPU6050_read_register( imu.device_address, ACCEL_XOUT_L );

    tempU16 = ( (uint16_t)reg_L | ( ((uint16_t)reg_H) << 8 ) );

    temp_I32 = ( ((int32_t)((int16_t)tempU16)) );
    temp_I32 = (( temp_I32 * accel_fs_value ) / 32767);

    imu.accelerometer[X] = ( temp_I32 - (int32_t)imu.accelerometer_offset[X] );
  }
  if( !imu.disable.sensors.y_accel )
  {
    reg_H = MPU6050_read_register( imu.device_address, ACCEL_YOUT_H );
    reg_L = MPU6050_read_register( imu.device_address, ACCEL_YOUT_L );

    tempU16 = ( (uint16_t)reg_L | ( ((uint16_t)reg_H) << 8 ) );

    temp_I32 = ( ((int32_t)((int16_t)tempU16)) );
    temp_I32 = (( temp_I32 * accel_fs_value ) / 32767);

    imu.accelerometer[Y] = ( temp_I32 - (int32_t)imu.accelerometer_offset[Y] );
  }
  if( !imu.disable.sensors.z_accel )
  {
    reg_H = MPU6050_read_register( imu.device_address, ACCEL_ZOUT_H );
    reg_L = MPU6050_read_register( imu.device_address, ACCEL_ZOUT_L );

    tempU16 = ( (uint16_t)reg_L | ( ((uint16_t)reg_H) << 8 ) );

    temp_I32 = ( ((int32_t)((int16_t)tempU16)) );
    temp_I32 = ( ( temp_I32 * accel_fs_value ) / 32767 );

    imu.accelerometer[Z] = (temp_I32 - (int32_t)imu.accelerometer_offset[Z]);
  }


  /** Gyroscope sensors. **/

  if( !imu.disable.sensors.x_gyro )
  {
    reg_H = MPU6050_read_register( imu.device_address, GYRO_XOUT_H );
    reg_L = MPU6050_read_register( imu.device_address, GYRO_XOUT_L );

    tempU16 = ( (uint16_t)reg_L | ( ((uint16_t)reg_H) << 8 ) );

    temp_I32 = ( ((int32_t)((int16_t)tempU16)) );
    temp_I32 = ( ( temp_I32 * gyro_fs_value ) / 32767 );

    imu.gyroscope[X] = (temp_I32 - (int32_t)imu.gyroscope_offset[X]);
  }
  if( !imu.disable.sensors.y_gyro )
  {
    reg_H = MPU6050_read_register( imu.device_address, GYRO_YOUT_H );
    reg_L = MPU6050_read_register( imu.device_address, GYRO_YOUT_L );

    tempU16 = ( (uint16_t)reg_L | ( ((uint16_t)reg_H) << 8 ) );

    temp_I32 = ( ((int32_t)((int16_t)tempU16))  );
    temp_I32 = ( ( temp_I32 * gyro_fs_value ) / 32767 );

    imu.gyroscope[Y] = (temp_I32 - (int32_t)imu.gyroscope_offset[Y]);
  }
  if( !imu.disable.sensors.z_gyro )
  {
    reg_H = MPU6050_read_register( imu.device_address, GYRO_ZOUT_H );
    reg_L = MPU6050_read_register( imu.device_address, GYRO_ZOUT_L );

    tempU16 = ( (uint16_t)reg_L | ( ((uint16_t)reg_H) << 8 ) );

    temp_I32 = ( ((int32_t)((int16_t)tempU16)) );
    temp_I32 = ( ( temp_I32 * gyro_fs_value ) / 32767 ) ;

    imu.gyroscope[Z] = ( temp_I32 - (int32_t)imu.gyroscope_offset[Z] );
  }
}

uint8_t MPU6050_readAllSensorData()
{
  uint8_t buffer[14];

  int32_t accel_fs_value = 0;
  int32_t gyro_fs_value = 0;
  uint8_t error_w = 0;
  uint8_t error_r = 0;

  error_w = I2C1_writeByte( imu.device_address, ACCEL_XOUT_H);
  error_r = I2C1_Read( imu.device_address, 14, buffer );

  /* buffer[0:1]   : Accelerometer X High and Low
   * buffer[2:3]   : Accelerometer Y High and Low
   * buffer[4:5]   : Accelerometer Z High and Low
   * buffer[6:7]   : Temperature High and Low
   * buffer[8:9]   : Gyroscope X High and Low.
   * buffer[10:11] : Gyroscope Y High and Low.
   * buffer[12:13] : Gyroscope Z High and Low.
   * */

  imu.accelerometer_raw[X] = ((((uint16_t)buffer[0]) << 8) + (uint16_t)buffer[1] );
  imu.accelerometer_raw[Y] = ((((uint16_t)buffer[2]) << 8) + (uint16_t)buffer[3] );
  imu.accelerometer_raw[Z] = ((((uint16_t)buffer[4]) << 8) + (uint16_t)buffer[5] );

  imu.gyroscope_raw[X] = ((((uint16_t)buffer[8]) << 8) + (uint16_t)buffer[9] );
  imu.gyroscope_raw[Y] = ((((uint16_t)buffer[10]) << 8) + (uint16_t)buffer[11] );
  imu.gyroscope_raw[Z] = ((((uint16_t)buffer[12]) << 8) + (uint16_t)buffer[13] );

  /* Obtain sensor full-scale values. */

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


  uint16_t tempU16 = 0;
  int32_t  tempI32 = 0;

  if( !(error_r|error_w) )
  {
  tempU16 = ( (((uint16_t)buffer[0]) << 8) | (uint16_t)buffer[1] );
  tempI32 = ( accel_fs_value * (int32_t)((int16_t)tempU16) );
  tempI32 = (tempI32/32767);
  imu.accelerometer[X] = ( tempI32 - imu.accelerometer_offset[X] );

  tempU16 = ( (((uint16_t)buffer[2]) << 8) | (uint16_t)buffer[3] );
  tempI32 = ( accel_fs_value * (int32_t)((int16_t)tempU16) );
  tempI32 = (tempI32/32767);
  imu.accelerometer[Y] = ( tempI32 - imu.accelerometer_offset[Y] );

  tempU16 = ( (((uint16_t)buffer[4]) << 8) | (uint16_t)buffer[5] );
  tempI32 = ( accel_fs_value * (int32_t)((int16_t)tempU16) );
  tempI32 = (tempI32/32767);
  imu.accelerometer[Z] = ( tempI32 - imu.accelerometer_offset[Z] );

  tempU16 = ( (((uint16_t)buffer[8]) << 8) | (uint16_t)buffer[9] );
  tempI32 = ( gyro_fs_value * (int32_t)((int16_t)tempU16) );
  tempI32 = (tempI32/32767);
  imu.gyroscope[X] = ( tempI32 - imu.gyroscope_offset[X] );

  tempU16 = ( (((uint16_t)buffer[10]) << 8) | (uint16_t)buffer[11] );
  tempI32 = ( gyro_fs_value * (int32_t)((int16_t)tempU16) );
  tempI32 = (tempI32/32767);
  imu.gyroscope[Y] = ( tempI32 - imu.gyroscope_offset[Y] );

  tempU16 = ( (((uint16_t)buffer[12]) << 8) | (uint16_t)buffer[13] );
  tempI32 = ( gyro_fs_value * (int32_t)((int16_t)tempU16) );
  tempI32 = (tempI32/32767);
  imu.gyroscope[Z] = ( tempI32 - imu.gyroscope_offset[Z] );
  }

  /* High-pass filter functionality for gyroscope. */

  #ifdef GYRO_HPF_EN

    imu.gyroscope[X] = HPF_Update(&Gx_filter,imu.gyroscope[X]);
    imu.gyroscope[Y] = HPF_Update(&Gy_filter,imu.gyroscope[Y]);
    imu.gyroscope[Z] = HPF_Update(&Gz_filter,imu.gyroscope[Z]);

  #endif /* GYRO_HPF_EN. */

  /* Write data into communication frames. */

  IMU_X_Raw_SMF.IMU_X_Raw_SMF_segments.XACCEL_raw = (uint16_t)imu.accelerometer_raw[X];
  IMU_X_Raw_SMF.IMU_X_Raw_SMF_segments.XGYRO_raw  = (uint16_t)imu.gyroscope_raw[X];
  IMU_Y_Raw_SMF.IMU_Y_Raw_SMF_segments.YACCEL_raw = (uint16_t)imu.accelerometer_raw[Y];
  IMU_Y_Raw_SMF.IMU_Y_Raw_SMF_segments.YGYRO_raw  = (uint16_t)imu.gyroscope_raw[Y];
  IMU_Z_Raw_SMF.IMU_Z_Raw_SMF_segments.ZACCEL_raw = (uint16_t)imu.accelerometer_raw[Z];
  IMU_Z_Raw_SMF.IMU_Z_Raw_SMF_segments.ZGYRO_raw  = (uint16_t)imu.gyroscope_raw[Z];

  IMU_XACCEL_Processed_SMF.all = imu.accelerometer[X];
  IMU_YACCEL_Processed_SMF.all = imu.accelerometer[Y];
  IMU_ZACCEL_Processed_SMF.all = imu.accelerometer[Z];
  IMU_XGYRO_Processed_SMF.all  = imu.gyroscope[X];
  IMU_YGYRO_Processed_SMF.all  = imu.gyroscope[Y];
  IMU_ZGYRO_Processed_SMF.all  = imu.gyroscope[Z];

  return(error_w | error_r);
}

void MPU6050_calibrate(Calibration_Axis axis, uint8_t nPoints)
{
  MPU6050_updateConfig();

  /* For calibration, keep the IMU sensor completely motionless. */

  int32_t x_accel_offset = 0;
  int32_t y_accel_offset = 0;
  int32_t z_accel_offset = 0;
  int32_t x_gyro_offset = 0;
  int32_t y_gyro_offset = 0;
  int32_t z_gyro_offset = 0;

  /* Set offset register values to zero. */

  imu.accelerometer_offset[X] = 0;
  imu.accelerometer_offset[Y] = 0;
  imu.accelerometer_offset[Z] = 0;

  imu.gyroscope_offset[X] = 0;
  imu.gyroscope_offset[Y] = 0;
  imu.gyroscope_offset[Z] = 0;

  /* Accumulate and add sensor data. */

  for( int i = 0 ; i < nPoints ; i++ )
  {
	MPU6050_readSensorData();

	x_accel_offset += ( (int32_t)imu.accelerometer[X] );
	y_accel_offset += ( (int32_t)imu.accelerometer[Y] );
	z_accel_offset += ( (int32_t)imu.accelerometer[Z] );

	/* Gyroscope registers. */

	x_gyro_offset += ( (int32_t)imu.gyroscope[X] );
	y_gyro_offset += ( (int32_t)imu.gyroscope[Y] );
	z_gyro_offset += ( (int32_t)imu.gyroscope[Z] );

	System_SysTick_delay_ms(imu.calibration_interval_ms);
  }

  /* Load gyroscope data. */

  imu.gyroscope_offset[X] = ( ( x_gyro_offset ) / (int32_t)nPoints );
  imu.gyroscope_offset[Y] = ( ( y_gyro_offset ) / (int32_t)nPoints );
  imu.gyroscope_offset[Z] = ( ( z_gyro_offset ) / (int32_t)nPoints );

  /* Load accelerometer offset. */

  imu.accelerometer_offset[X] = ( ( x_accel_offset ) / (int32_t)nPoints );
  imu.accelerometer_offset[Y] = ( ( y_accel_offset ) / (int32_t)nPoints );
  imu.accelerometer_offset[Z] = ( ( z_accel_offset ) / (int32_t)nPoints );

  switch(axis)
  {
    case(X_Positive):
	{
      imu.accelerometer_offset[X] -= 1000;
	}
    break;

    case(X_Negative):
	{
      imu.accelerometer_offset[X] += 1000;
	}
    break;

    case(Y_Positive):
	{
      imu.accelerometer_offset[Y] -= 1000;
	}
    break;

    case(Y_Negative):
	{
      imu.accelerometer_offset[Y] += 1000;
	}
    break;

    case(Z_Positive):
	{
      imu.accelerometer_offset[Z] -= 1000;
	}
    break;

    case(Z_Negative):
	{
      imu.accelerometer_offset[Z] += 1000;
	}
    break;
  }

  /* Put characters in the communication frame. */

  IMU_CALIB_XACCEL_Offset_SMF.all = imu.accelerometer_offset[X];
  IMU_CALIB_YACCEL_Offset_SMF.all = imu.accelerometer_offset[Y];
  IMU_CALIB_ZACCEL_Offset_SMF.all = imu.accelerometer_offset[Z];
  IMU_CALIB_XGYRO_Offset_SMF.all  = imu.gyroscope_offset[X];
  IMU_CALIB_YGYRO_Offset_SMF.all  = imu.gyroscope_offset[Y];
  IMU_CALIB_ZGYRO_Offset_SMF.all  = imu.gyroscope_offset[Z];
}


void MPU6050_errorHandler()
{
  RCC->APB1RSTR |= RCC_APB1RSTR_I2C1RST_Msk;

  System_SysTick_delay_ms(100);

  I2C1_Init();
  MPU6050_Init();
}

/* Function to initialize the high-pass filters for gyroscopes. */

#ifdef GYRO_HPF_EN

void Gyro_HPF_Init()
{
  /* Initialize filters for gyroscopes. */

  HPF_Init(&Gx_filter,100.0f,0.01f);
  HPF_Init(&Gy_filter,100.0f,0.25f);
  HPF_Init(&Gz_filter,100.0f,0.25f);

}

#endif /* USE_GYRO_HPF */
