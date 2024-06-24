
#include "LCD_GUI.h"

void LCD_GUI_Init()
{
  LCD_GUI_screen_number = LCD_GUI_Main_Initialize;

  Nokia5110_Init();

  Nokia5110.updated = 0;
  Nokia5110.x_pos = 0;
  Nokia5110.y_pos = 0;

  for(int i = 0 ; i < 504; i++)
  {
    Nokia5110.byte[i] = 0x00;
  }

  Nokia5110.x_pos = 8;
  Nokia5110.y_pos = 8;

  Nokia5110_printString("IMU Tester");


  Nokia5110.x_pos = 8;
  Nokia5110.y_pos = 16;

  Nokia5110_printString("Version 1.0");

  Nokia5110.x_pos = 12;
  Nokia5110.y_pos = 32;

  Nokia5110_printString("[Press SW");

  Nokia5110.x_pos = 12;
  Nokia5110.y_pos = 40;

  Nokia5110_printString("to start]");


  Nokia5110_updateDisplay();
}


/* Configuration screen. */

static void LCD_GUI_Config_Screen()
{
  Nokia5110_clear();

  Nokia5110.x_pos = 0;
  Nokia5110.y_pos = 0;

  Nokia5110_printString("[   CONFIG   ]");

  Nokia5110.x_pos = 8;
  Nokia5110.y_pos = 16;

  Nokia5110_printString("IMU");

  Nokia5110.x_pos = 8;
  Nokia5110.y_pos = 24;

  Nokia5110_printString("Comp. Filt.");

  Nokia5110.x_pos = 8;
  Nokia5110.y_pos = 32;

  Nokia5110_printString("Tilt dtcn.");
}

static void LCD_GUI_Config_Screen_A()
{
  Nokia5110_clear();

  Nokia5110.x_pos = 0;
  Nokia5110.y_pos = 0;

  Nokia5110_printString("[   CONFIG   ]");

  Nokia5110.x_pos = 0;
  Nokia5110.y_pos = 16;

  Nokia5110_printString(">IMU");

  Nokia5110.x_pos = 8;
  Nokia5110.y_pos = 24;

  Nokia5110_printString("Comp. Filt.");

  Nokia5110.x_pos = 8;
  Nokia5110.y_pos = 32;

  Nokia5110_printString("Tilt dtcn.");
}


static void LCD_GUI_Config_Screen_IMU_settings_A()
{
	  Nokia5110_clear();

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 0;

	  Nokia5110_printString("[    IMU     ]");

	  Nokia5110.x_pos = 0; Nokia5110.y_pos = 8;
	  Nokia5110_printString(">Accel FS:");

	  Nokia5110.x_pos = 0; Nokia5110.y_pos = 16;

	  switch( imu.sensor_configurations.sensor_configuration_segments.accel_fs_config)
	  {
	    case(ACCEL_FS_2_G):{ Nokia5110_printString("2 G"); }break;
	    case(ACCEL_FS_4_G):{ Nokia5110_printString("4 G"); }break;
	    case(ACCEL_FS_8_G):{ Nokia5110_printString("8 G"); }break;
	    case(ACCEL_FS_16_G):{ Nokia5110_printString("16 G"); }break;
	  }

	  Nokia5110.x_pos = 8; Nokia5110.y_pos = 24;
	  Nokia5110_printString("Gyro FS:");

	  Nokia5110.x_pos = 0; Nokia5110.y_pos = 32;

	  switch( imu.sensor_configurations.sensor_configuration_segments.gyro_fs_config)
	  {
	    case(GYRO_FS_250_DPS):{ Nokia5110_printString("250 DPS"); }break;
	    case(GYRO_FS_500_DPS):{ Nokia5110_printString("500 DPS"); }break;
	    case(GYRO_FS_1000_DPS):{ Nokia5110_printString("1000 DPS"); }break;
	    case(GYRO_FS_2000_DPS):{ Nokia5110_printString("2000 DPS"); }break;
	  }
}


static void LCD_GUI_Config_Screen_IMU_settings_B()
{
	  Nokia5110_clear();

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 0;

	  Nokia5110_printString("[    IMU     ]");

	  Nokia5110.x_pos = 8; Nokia5110.y_pos = 8;
	  Nokia5110_printString("Accel FS:");

	  Nokia5110.x_pos = 0; Nokia5110.y_pos = 16;

	  switch( imu.sensor_configurations.sensor_configuration_segments.accel_fs_config)
	  {
	    case(ACCEL_FS_2_G):{ Nokia5110_printString("2 G"); }break;
	    case(ACCEL_FS_4_G):{ Nokia5110_printString("4 G"); }break;
	    case(ACCEL_FS_8_G):{ Nokia5110_printString("8 G"); }break;
	    case(ACCEL_FS_16_G):{ Nokia5110_printString("16 G"); }break;
	  }

	  Nokia5110.x_pos = 0; Nokia5110.y_pos = 24;
	  Nokia5110_printString(">Gyro FS:");

	  Nokia5110.x_pos = 0; Nokia5110.y_pos = 32;

	  switch( imu.sensor_configurations.sensor_configuration_segments.gyro_fs_config)
	  {
	    case(GYRO_FS_250_DPS):{ Nokia5110_printString("250 DPS"); }break;
	    case(GYRO_FS_500_DPS):{ Nokia5110_printString("500 DPS"); }break;
	    case(GYRO_FS_1000_DPS):{ Nokia5110_printString("1000 DPS"); }break;
	    case(GYRO_FS_2000_DPS):{ Nokia5110_printString("2000 DPS"); }break;
	  }
}


static void LCD_GUI_Config_Screen_IMU_settings_C()
{
	  Nokia5110_clear();

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 0;

	  Nokia5110_printString("[    IMU     ]");

	  Nokia5110.x_pos = 0; Nokia5110.y_pos = 8;
	  Nokia5110_printString(">Calib. Pts. :");

	  Nokia5110.x_pos = 0; Nokia5110.y_pos = 16;

      Nokia5110_printU8(imu.N_calibration_points);

	  Nokia5110.x_pos = 8; Nokia5110.y_pos = 24;
	  Nokia5110_printString("Calib. int.:");

	  Nokia5110.x_pos = 0; Nokia5110.y_pos = 32;
      Nokia5110_printU8(imu.calibration_interval_ms);
}


static void LCD_GUI_Config_Screen_IMU_settings_D()
{
	  Nokia5110_clear();

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 0;

	  Nokia5110_printString("[    IMU     ]");

	  Nokia5110.x_pos = 8; Nokia5110.y_pos = 8;
	  Nokia5110_printString("Calib. Pts. :");

	  Nokia5110.x_pos = 0; Nokia5110.y_pos = 16;

      Nokia5110_printU8(imu.N_calibration_points);

	  Nokia5110.x_pos = 0; Nokia5110.y_pos = 24;
	  Nokia5110_printString(">Calib. int.:");

	  Nokia5110.x_pos = 0; Nokia5110.y_pos = 32;
      Nokia5110_printU8(imu.calibration_interval_ms);
}


static void LCD_GUI_Config_Screen_IMU_settings_E()
{
	  Nokia5110_clear();

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 0;

	  Nokia5110_printString("[    IMU     ]");

	  Nokia5110.x_pos = 0; Nokia5110.y_pos = 8;
	  Nokia5110_printString(">Calib. axis.:");

	  Nokia5110.x_pos = 0; Nokia5110.y_pos = 16;

	  switch(imu.calibration_axis)
	  {
	    case(X_Positive):{ Nokia5110_printString("X positive"); }break;
	    case(X_Negative):{ Nokia5110_printString("X negative"); }break;
	    case(Y_Positive):{ Nokia5110_printString("Y positive"); }break;
	    case(Y_Negative):{ Nokia5110_printString("Y negative"); }break;
	    case(Z_Positive):{ Nokia5110_printString("Z positive"); }break;
	    case(Z_Negative):{ Nokia5110_printString("Z negative"); }break;
	  }

	  Nokia5110.x_pos = 8; Nokia5110.y_pos = 24;
	  Nokia5110_printString("Return");
}


static void LCD_GUI_Config_Screen_IMU_settings_F()
{
	  Nokia5110_clear();

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 0;

	  Nokia5110_printString("[    IMU     ]");

	  Nokia5110.x_pos = 8; Nokia5110.y_pos = 8;
	  Nokia5110_printString("Calib. axis.:");

	  Nokia5110.x_pos = 0; Nokia5110.y_pos = 16;

	  switch(imu.calibration_axis)
	  {
	    case(X_Positive):{ Nokia5110_printString("X positive"); }break;
	    case(X_Negative):{ Nokia5110_printString("X negative"); }break;
	    case(Y_Positive):{ Nokia5110_printString("Y positive"); }break;
	    case(Y_Negative):{ Nokia5110_printString("Y negative"); }break;
	    case(Z_Positive):{ Nokia5110_printString("Z positive"); }break;
	    case(Z_Negative):{ Nokia5110_printString("Z negative"); }break;
	  }

	  Nokia5110.x_pos = 0; Nokia5110.y_pos = 24;
	  Nokia5110_printString(">Return");
}



static void LCD_GUI_Config_Screen_B()
{
  Nokia5110_clear();

  Nokia5110.x_pos = 0;
  Nokia5110.y_pos = 0;

  Nokia5110_printString("[   CONFIG   ]");

  Nokia5110.x_pos = 8;
  Nokia5110.y_pos = 16;

  Nokia5110_printString("IMU");

  Nokia5110.x_pos = 0;
  Nokia5110.y_pos = 24;

  Nokia5110_printString(">Comp. Filt.");

  Nokia5110.x_pos = 8;
  Nokia5110.y_pos = 32;

  Nokia5110_printString("Tilt dtcn.");
}

static void LCD_GUI_Config_Screen_CF_settings_A()
{
	  Nokia5110_clear();

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 0;

	  Nokia5110_printString("[ COMP FILT  ]");

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 8;

	  Nokia5110_printString(">Roll coeff:");

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 16;

	  Nokia5110_printFloat(Motion_Processing.roll_coeff,3);

	  Nokia5110.x_pos = 8;
	  Nokia5110.y_pos = 24;

	  Nokia5110_printString("Pitch coeff.");

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 32;

	  Nokia5110_printFloat(Motion_Processing.pitch_coeff,3);
}


static void LCD_GUI_Config_Screen_CF_settings_B()
{
	  Nokia5110_clear();

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 0;

	  Nokia5110_printString("[ COMP FILT  ]");

	  Nokia5110.x_pos = 8;
	  Nokia5110.y_pos = 8;

	  Nokia5110_printString("Roll coeff:");

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 16;

	  Nokia5110_printFloat(Motion_Processing.roll_coeff,3);

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 24;

	  Nokia5110_printString(">Pitch coeff.");

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 32;

	  Nokia5110_printFloat(Motion_Processing.pitch_coeff,3);
}


static void LCD_GUI_Config_Screen_CF_settings_C()
{
	  Nokia5110_clear();

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 0;

	  Nokia5110_printString("[ COMP FILT  ]");

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 8;

	  Nokia5110_printString(">Yaw coeff.:");

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 16;

	  Nokia5110_printFloat(Motion_Processing.yaw_coeff,3);

	  Nokia5110.x_pos = 8;
	  Nokia5110.y_pos = 24;

	  Nokia5110_printString("Return");
}


static void LCD_GUI_Config_Screen_CF_settings_D()
{
	  Nokia5110_clear();

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 0;

	  Nokia5110_printString("[ COMP FILT  ]");

	  Nokia5110.x_pos = 8;
	  Nokia5110.y_pos = 8;

	  Nokia5110_printString("Yaw coeff.:");

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 16;

	  Nokia5110_printFloat(Motion_Processing.yaw_coeff,3);

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 24;

	  Nokia5110_printString(">Return");
}


static void LCD_GUI_Config_Screen_Tilt_Detection_settings_A()
{
	  Nokia5110_clear();

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 0;

	  Nokia5110_printString("[Tilt  Detect]");

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 8;

	  Nokia5110_printString(">X_angle:");

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 16;

	  Nokia5110_printFloat(Motion_Processing.angle_threshold[X],1);

	  Nokia5110.x_pos = 8;
	  Nokia5110.y_pos = 24;

	  Nokia5110_printString("Y_angle:");

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 32;

	  Nokia5110_printFloat(Motion_Processing.angle_threshold[Y],1);
}


static void LCD_GUI_Config_Screen_Tilt_Detection_settings_B()
{
	  Nokia5110_clear();

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 0;

	  Nokia5110_printString("[Tilt  Detect]");

	  Nokia5110.x_pos = 8;
	  Nokia5110.y_pos = 8;

	  Nokia5110_printString("X_angle:");

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 16;

	  Nokia5110_printFloat(Motion_Processing.angle_threshold[X],1);

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 24;

	  Nokia5110_printString(">Y_angle:");

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 32;

	  Nokia5110_printFloat(Motion_Processing.angle_threshold[Y],1);
}


static void LCD_GUI_Config_Screen_Tilt_Detection_settings_C()
{
	  Nokia5110_clear();

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 0;

	  Nokia5110_printString("[Tilt  Detect]");

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 8;

	  Nokia5110_printString(">Z_angle:");

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 16;

	  Nokia5110_printFloat(Motion_Processing.angle_threshold[Z],1);

	  Nokia5110.x_pos = 8;
	  Nokia5110.y_pos = 24;

	  Nokia5110_printString("Return");
}


static void LCD_GUI_Config_Screen_Tilt_Detection_settings_D()
{
	  Nokia5110_clear();

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 0;

	  Nokia5110_printString("[Tilt  Detect]");

	  Nokia5110.x_pos = 8;
	  Nokia5110.y_pos = 8;

	  Nokia5110_printString("Z_angle:");

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 16;

	  Nokia5110_printFloat(Motion_Processing.angle_threshold[Z],1);

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 24;

	  Nokia5110_printString(">Return");
}


static void LCD_GUI_Config_Screen_Vibration_Detection_settings_A()
{
	  Nokia5110_clear();

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 0;

	  Nokia5110_printString("[ VBR DETECT ]");

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 8;

	  Nokia5110_printString(">X_velocity:");

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 16;

	  Nokia5110_printFloat(Motion_Processing.vibration_threshold[X],1);

	  Nokia5110.x_pos = 8;
	  Nokia5110.y_pos = 24;

	  Nokia5110_printString("Y_velocity:");

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 32;

	  Nokia5110_printFloat(Motion_Processing.vibration_threshold[Y],1);
}


static void LCD_GUI_Config_Screen_Vibration_Detection_settings_B()
{
	  Nokia5110_clear();

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 0;

	  Nokia5110_printString("[ VBR DETECT ]");

	  Nokia5110.x_pos = 8;
	  Nokia5110.y_pos = 8;

	  Nokia5110_printString("X_velocity:");

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 16;

	  Nokia5110_printFloat(Motion_Processing.vibration_threshold[X],1);

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 24;

	  Nokia5110_printString(">Y_velocity:");

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 32;

	  Nokia5110_printFloat(Motion_Processing.vibration_threshold[Y],1);
}


static void LCD_GUI_Config_Screen_Vibration_Detection_settings_C()
{
	  Nokia5110_clear();

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 0;

	  Nokia5110_printString("[ VBR DETECT ]");

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 8;

	  Nokia5110_printString(">Z_velocity.:");

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 16;

	  Nokia5110_printFloat(Motion_Processing.vibration_threshold[Z],1);

	  Nokia5110.x_pos = 8;
	  Nokia5110.y_pos = 24;

	  Nokia5110_printString("Return");
}


static void LCD_GUI_Config_Screen_Vibration_Detection_settings_D()
{
	  Nokia5110_clear();

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 0;

	  Nokia5110_printString("[ VBR DETECT ]");

	  Nokia5110.x_pos = 8;
	  Nokia5110.y_pos = 8;

	  Nokia5110_printString("Z_velocity:");

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 16;

	  Nokia5110_printFloat(Motion_Processing.vibration_threshold[Z],1);

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 24;

	  Nokia5110_printString(">Return");
}



static void LCD_GUI_Config_Screen_LED_settings_A()
{
	  Nokia5110_clear();

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 0;

	  Nokia5110_printString("[ LED CONFIG ]");

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 8;

	  Nokia5110_printString(">X_LED mode:");

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 16;

	  switch(LED.LED_parameters.LED1_mode)
	  {
	    case(Angle_detection):{Nokia5110_printString("Tilt dct.");}break;
	    case(Vibration_detection):{Nokia5110_printString("Vbr dct.");}break;
	    case(Accelerometer_measurement):{Nokia5110_printString("Accel meas.");}break;
	    case(Gyroscope_measurement):{Nokia5110_printString("Gyro meas.");}break;
	  }

	  Nokia5110.x_pos = 8;
	  Nokia5110.y_pos = 24;

	  Nokia5110_printString("Y_LED mode:");

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 32;

	  switch(LED.LED_parameters.LED2_mode)
	  {
	    case(Angle_detection):{Nokia5110_printString("Tilt dct.");}break;
	    case(Vibration_detection):{Nokia5110_printString("Vbr dct.");}break;
	    case(Accelerometer_measurement):{Nokia5110_printString("Accel meas.");}break;
	    case(Gyroscope_measurement):{Nokia5110_printString("Gyro meas.");}break;
	  }
}


static void LCD_GUI_Config_Screen_LED_settings_B()
{
	  Nokia5110_clear();

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 0;

	  Nokia5110_printString("[ LED CONFIG ]");

	  Nokia5110.x_pos = 8;
	  Nokia5110.y_pos = 8;

	  Nokia5110_printString("X_LED mode:");

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 16;

	  switch(LED.LED_parameters.LED1_mode)
	  {
	    case(Angle_detection):{Nokia5110_printString("Tilt dct.");}break;
	    case(Vibration_detection):{Nokia5110_printString("Vbr dct.");}break;
	    case(Accelerometer_measurement):{Nokia5110_printString("Accel meas.");}break;
	    case(Gyroscope_measurement):{Nokia5110_printString("Gyro meas.");}break;
	  }

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 24;

	  Nokia5110_printString(">Y_LED mode:");

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 32;

	  switch(LED.LED_parameters.LED2_mode)
	  {
	    case(Angle_detection):{Nokia5110_printString("Tilt dct.");}break;
	    case(Vibration_detection):{Nokia5110_printString("Vbr dct.");}break;
	    case(Accelerometer_measurement):{Nokia5110_printString("Accel meas.");}break;
	    case(Gyroscope_measurement):{Nokia5110_printString("Gyro meas.");}break;
	  }
}


static void LCD_GUI_Config_Screen_LED_settings_C()
{
	  Nokia5110_clear();

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 0;

	  Nokia5110_printString("[LED CONFIG]");

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 8;

	  Nokia5110_printString(">Z_LED mode.:");

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 16;

	  switch(LED.LED_parameters.LED3_mode)
	  {
	    case(Angle_detection):{Nokia5110_printString("Tilt dct.");}break;
	    case(Vibration_detection):{Nokia5110_printString("Vbr dct.");}break;
	    case(Accelerometer_measurement):{Nokia5110_printString("Accel meas.");}break;
	    case(Gyroscope_measurement):{Nokia5110_printString("Gyro meas.");}break;
	  }

	  Nokia5110.x_pos = 8;
	  Nokia5110.y_pos = 24;

	  Nokia5110_printString("Return");
}


static void LCD_GUI_Config_Screen_LED_settings_D()
{
	  Nokia5110_clear();

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 0;

	  Nokia5110_printString("[LED CONFIG]");

	  Nokia5110.x_pos = 8;
	  Nokia5110.y_pos = 8;

	  Nokia5110_printString("Z_LED mode.:");

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 16;

	  switch(LED.LED_parameters.LED3_mode)
	  {
	    case(Angle_detection):{Nokia5110_printString("Tilt dct.");}break;
	    case(Vibration_detection):{Nokia5110_printString("Vbr dct.");}break;
	    case(Accelerometer_measurement):{Nokia5110_printString("Accel meas.");}break;
	    case(Gyroscope_measurement):{Nokia5110_printString("Gyro meas.");}break;
	  }

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 24;

	  Nokia5110_printString(">Return");
}


static void LCD_GUI_Config_Screen_C()
{
  Nokia5110_clear();

  Nokia5110.x_pos = 0;
  Nokia5110.y_pos = 0;

  Nokia5110_printString("[   CONFIG   ]");

  Nokia5110.x_pos = 8;
  Nokia5110.y_pos = 16;

  Nokia5110_printString("IMU");

  Nokia5110.x_pos = 8;
  Nokia5110.y_pos = 24;

  Nokia5110_printString("Comp. Filt.");

  Nokia5110.x_pos = 0;
  Nokia5110.y_pos = 32;

  Nokia5110_printString(">Tilt dctn.");
}


static void LCD_GUI_Config_Screen_D()
{
  Nokia5110_clear();

  Nokia5110.x_pos = 0;
  Nokia5110.y_pos = 0;

  Nokia5110_printString("[   CONFIG   ]");

  Nokia5110.x_pos = 0;
  Nokia5110.y_pos = 16;

  Nokia5110_printString(">Vbr. dctn.");

  Nokia5110.x_pos = 8;
  Nokia5110.y_pos = 24;

  Nokia5110_printString("LED config.");

  Nokia5110.x_pos = 8;
  Nokia5110.y_pos = 32;

  Nokia5110_printString("Save");
}


static void LCD_GUI_Config_Screen_E()
{
  Nokia5110_clear();

  Nokia5110.x_pos = 0;
  Nokia5110.y_pos = 0;

  Nokia5110_printString("[   CONFIG   ]");

  Nokia5110.x_pos = 8;
  Nokia5110.y_pos = 16;

  Nokia5110_printString("Vbr. dctn.");

  Nokia5110.x_pos = 0;
  Nokia5110.y_pos = 24;

  Nokia5110_printString(">LED config.");

  Nokia5110.x_pos = 8;
  Nokia5110.y_pos = 32;

  Nokia5110_printString("Save");
}

static void LCD_GUI_Config_Screen_F()
{
  Nokia5110_clear();

  Nokia5110.x_pos = 0;
  Nokia5110.y_pos = 0;

  Nokia5110_printString("[   CONFIG   ]");

  Nokia5110.x_pos = 8;
  Nokia5110.y_pos = 16;

  Nokia5110_printString("Vbr. dctn.");

  Nokia5110.x_pos = 8;
  Nokia5110.y_pos = 24;

  Nokia5110_printString("LED config.");

  Nokia5110.x_pos = 0;
  Nokia5110.y_pos = 32;

  Nokia5110_printString(">Save");
}


/* IMU accelerations (raw) */

static void LCD_GUI_Accel_Raw()
{
  Nokia5110_clear();

  Nokia5110.x_pos = 0;
  Nokia5110.y_pos = 0;

  Nokia5110_printString("[ ACCEL RAW  ]");

  Nokia5110.x_pos = 0;
  Nokia5110.y_pos = 16;

  Nokia5110_printString("A_x=");  Nokia5110_printU16(imu.accelerometer_raw[X]);

  Nokia5110.x_pos = 0;
  Nokia5110.y_pos = 24;

  Nokia5110_printString("A_y=");  Nokia5110_printU16(imu.accelerometer_raw[Y]);

  Nokia5110.x_pos = 0;
  Nokia5110.y_pos = 32;

  Nokia5110_printString("A_z=");  Nokia5110_printU16(imu.accelerometer_raw[Z]);
}

/* IMU accelerations (processed) */

static void LCD_GUI_Accel_Processed()
{
  Nokia5110_clear();

  Nokia5110.x_pos = 0;
  Nokia5110.y_pos = 0;

  Nokia5110_printString("[ACCEL PRCSSD]");

  Nokia5110.x_pos = 0;
  Nokia5110.y_pos = 16;

  Nokia5110_printString("A_x=");  Nokia5110_printI32(imu.accelerometer[X]);

  Nokia5110.x_pos = 0;
  Nokia5110.y_pos = 24;

  Nokia5110_printString("A_y=");  Nokia5110_printI32(imu.accelerometer[Y]);

  Nokia5110.x_pos = 0;
  Nokia5110.y_pos = 32;

  Nokia5110_printString("A_z=");  Nokia5110_printI32(imu.accelerometer[Z]);
}

/* IMU gyroscope readings (raw) */

static void LCD_GUI_Gyro_Raw()
{
	  Nokia5110_clear();

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 0;

	  Nokia5110_printString("[ GYRO   RAW ]");

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 16;

	  Nokia5110_printString("G_x=");  Nokia5110_printI32(imu.gyroscope_raw[X]);

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 24;

	  Nokia5110_printString("G_y=");  Nokia5110_printI32(imu.gyroscope_raw[Y]);

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 32;

	  Nokia5110_printString("G_z=");  Nokia5110_printI32(imu.gyroscope_raw[Z]);

}

/* IMU gyroscope readings (processed) */

static void LCD_GUI_Gyro_Processed()
{
	  Nokia5110_clear();

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 0;

	  Nokia5110_printString("[GYRO  PRCSSD]");

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 16;

	  Nokia5110_printString("G_x=");  Nokia5110_printI32(imu.gyroscope[X]);

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 24;

	  Nokia5110_printString("G_y=");  Nokia5110_printI32(imu.gyroscope[Y]);

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 32;

	  Nokia5110_printString("G_z=");  Nokia5110_printI32(imu.gyroscope[Z]);

}

/* RYP from accelerometers. */

static void LCD_GUI_RYP_Accel()
{
	  Nokia5110_clear();

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 0;

	  Nokia5110_printString("[ RYP  ACCEL ]");

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 16;

	  Nokia5110_printString("R=");  Nokia5110_printFloat(Motion_Processing.accel_roll,2);

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 24;

	  Nokia5110_printString("P=");  Nokia5110_printFloat(Motion_Processing.accel_pitch,2);

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 32;

	  Nokia5110_printString("Y=");  Nokia5110_printFloat(Motion_Processing.accel_yaw,2);
}

/* RYP from gyroscope. */

static void LCD_GUI_RYP_Gyro()
{
	  Nokia5110_clear();

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 0;

	  Nokia5110_printString("[ RYP   GYRO ]");

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 16;

	  Nokia5110_printString("R=");  Nokia5110_printFloat(Motion_Processing.gyro_roll,2);

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 24;

	  Nokia5110_printString("P=");  Nokia5110_printFloat(Motion_Processing.gyro_pitch,2);

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 32;

	  Nokia5110_printString("Y=");  Nokia5110_printFloat(Motion_Processing.gyro_yaw,2);
}

/* RYP from complementary filter. */

static void LCD_GUI_RYP_Filter()
{
	  Nokia5110_clear();

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 0;

	  Nokia5110_printString("[  RPY   CF  ]");

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 16;

	  Nokia5110_printString("R=");  Nokia5110_printFloat(Motion_Processing.filter_roll,2);

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 24;

	  Nokia5110_printString("P=");  Nokia5110_printFloat(Motion_Processing.filter_pitch,2);

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 32;

	  Nokia5110_printString("Y=");  Nokia5110_printFloat(Motion_Processing.filter_yaw,2);
}

/* Motion detection. */

static void LCD_GUI_Motion_Detection()
{
	  Nokia5110_clear();

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 0;

	  Nokia5110_printString("[MOTION  DDCT]");

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 16;

	  Nokia5110_printString("T_x:");
	  Nokia5110_printU8(Motion_Processing.motion_detection.motion_detection_segments.X_angle_detected);

	  Nokia5110.x_pos = 42;
	  Nokia5110.y_pos = 16;
	  Nokia5110_printString("V_x:");
	  Nokia5110_printU8(Motion_Processing.motion_detection.motion_detection_segments.X_vibration_detected);

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 24;

	  Nokia5110_printString("T_y:");
	  Nokia5110_printU8(Motion_Processing.motion_detection.motion_detection_segments.Y_angle_detected);

	  Nokia5110.x_pos = 42;
	  Nokia5110.y_pos = 24;
	  Nokia5110_printString("V_y:");
	  Nokia5110_printU8(Motion_Processing.motion_detection.motion_detection_segments.Y_vibration_detected);

	  Nokia5110.x_pos = 0;
	  Nokia5110.y_pos = 32;

	  Nokia5110_printString("T_z:");
	  Nokia5110_printU8(Motion_Processing.motion_detection.motion_detection_segments.Z_angle_detected);

	  Nokia5110.x_pos = 42;
	  Nokia5110.y_pos = 32;
	  Nokia5110_printString("V_z:");
	  Nokia5110_printU8(Motion_Processing.motion_detection.motion_detection_segments.Z_vibration_detected);
}



/*********		Main FSM   **********/

void LCD_GUI_Update()
{
  SDCP_Param parameter;

  switch(LCD_GUI_screen_number)
  {
    case(LCD_GUI_Main_Initialize):
	{
	  /* If the switch is pressed, proceed to config screen. */

	  if(Rotary_Encoder.parameters.switch_state)
	  {
	    LCD_GUI_screen_number = LCD_GUI_Main_Config_Mode;

		__disable_irq();
		MPU6050_calibrate(imu.calibration_axis,imu.N_calibration_points);
		__enable_irq();

		MP_Init();
	  }
    }
	break;


    case(LCD_GUI_Main_Config_Mode):
	{
	  LCD_GUI_Config_Screen();

	  if(Rotary_Encoder.parameters.clockwise)
	  {
	    LCD_GUI_screen_number = LCD_GUI_Main_IMU_accel_raw;
	  }
	  else if(Rotary_Encoder.parameters.anticlockwise)
	  {
	    LCD_GUI_screen_number = LCD_GUI_Main_Motion_Detection;
	  }

	  /* When in configuration screen, if switch is pressed, transition to
	   * configuration mode. */

	  if(Rotary_Encoder.parameters.switch_state)
	  {
        LCD_GUI_screen_number = LCD_GUI_Config_Menu_A;
	  }
	}
	break;


	case(LCD_GUI_Main_IMU_accel_raw):
	{
	  LCD_GUI_Accel_Raw();

	  if(Rotary_Encoder.parameters.clockwise)
	  {
	    LCD_GUI_screen_number = LCD_GUI_Main_IMU_accel_processed;
	  }
	  if(Rotary_Encoder.parameters.anticlockwise)
	  {
	    LCD_GUI_screen_number = LCD_GUI_Main_Config_Mode;
	  }

      if(Rotary_Encoder.parameters.switch_state)
	  {
	    __disable_irq();
		MPU6050_calibrate(imu.calibration_axis,imu.N_calibration_points);
		__enable_irq();
	    MP_Init();
	  }
    }
	break;

	case(LCD_GUI_Main_IMU_accel_processed):
	{
	  LCD_GUI_Accel_Processed();

	  if(Rotary_Encoder.parameters.clockwise)
	  {
	    LCD_GUI_screen_number = LCD_GUI_Main_IMU_gyro_raw;
	  }
	  if(Rotary_Encoder.parameters.anticlockwise)
	  {
	    LCD_GUI_screen_number = LCD_GUI_Main_IMU_accel_raw;
	  }
	  if(Rotary_Encoder.parameters.switch_state)
	  {
		__disable_irq();
		MPU6050_calibrate(imu.calibration_axis,imu.N_calibration_points);
		__enable_irq();
		MP_Init();
	  }
    }
	break;

	case(LCD_GUI_Main_IMU_gyro_raw):
	{
	  LCD_GUI_Gyro_Raw();

	  if(Rotary_Encoder.parameters.clockwise)
	  {
	    LCD_GUI_screen_number = LCD_GUI_Main_IMU_gyro_processed;
	  }
	  else if(Rotary_Encoder.parameters.anticlockwise)
	  {
        LCD_GUI_screen_number = LCD_GUI_Main_IMU_accel_processed;
	  }

      if(Rotary_Encoder.parameters.switch_state)
	  {
	    __disable_irq();
		MPU6050_calibrate(imu.calibration_axis,imu.N_calibration_points);
		__enable_irq();
		MP_Init();
	  }
	}
	break;

	case(LCD_GUI_Main_IMU_gyro_processed):
	{
	  LCD_GUI_Gyro_Processed();

	  if(Rotary_Encoder.parameters.clockwise)
	  {
	    LCD_GUI_screen_number = LCD_GUI_Main_RPY_accel;
	  }
	  else if(Rotary_Encoder.parameters.anticlockwise)
	  {
	    LCD_GUI_screen_number = LCD_GUI_Main_IMU_gyro_raw;
	  }

	  if(Rotary_Encoder.parameters.switch_state)
	  {
	    __disable_irq();
		MPU6050_calibrate(imu.calibration_axis,imu.N_calibration_points);
		__enable_irq();
		MP_Init();
      }
    }
	break;


    case(LCD_GUI_Main_RPY_accel):
	{
      LCD_GUI_RYP_Accel();

	  if(Rotary_Encoder.parameters.clockwise)
	  {
	    LCD_GUI_screen_number = LCD_GUI_Main_RPY_gyro;
	  }
	  if(Rotary_Encoder.parameters.anticlockwise)
	  {
	    LCD_GUI_screen_number = LCD_GUI_Main_IMU_gyro_processed;
	  }
	  if(Rotary_Encoder.parameters.switch_state)
	  {
		__disable_irq();
		MPU6050_calibrate(imu.calibration_axis,imu.N_calibration_points);
		__enable_irq();
		MP_Init();
	  }
    }
	break;


    case(LCD_GUI_Main_RPY_gyro):
	{
	  LCD_GUI_RYP_Gyro();

	  if(Rotary_Encoder.parameters.clockwise)
	  {
	    LCD_GUI_screen_number = LCD_GUI_Main_RPY_filter;
	  }
	  else if(Rotary_Encoder.parameters.anticlockwise)
	  {
		LCD_GUI_screen_number = LCD_GUI_Main_RPY_accel;
	  }

	  if(Rotary_Encoder.parameters.switch_state)
	  {
	    __disable_irq();
		MPU6050_calibrate(imu.calibration_axis,imu.N_calibration_points);
		__enable_irq();
		MP_Init();
	  }
    }
	break;


	case(LCD_GUI_Main_RPY_filter):
	{
	  LCD_GUI_RYP_Filter();

	  if(Rotary_Encoder.parameters.clockwise)
	  {
	    LCD_GUI_screen_number = LCD_GUI_Main_Motion_Detection;
	  }
	  else if(Rotary_Encoder.parameters.anticlockwise)
	  {
	    LCD_GUI_screen_number = LCD_GUI_Main_RPY_gyro;
	  }

      if(Rotary_Encoder.parameters.switch_state)
	  {
		__disable_irq();
		MPU6050_calibrate(imu.calibration_axis,imu.N_calibration_points);
		__enable_irq();
		MP_Init();
	  }
    }
	break;

	case(LCD_GUI_Main_Motion_Detection):
	{
	  LCD_GUI_Motion_Detection();

	  if(Rotary_Encoder.parameters.clockwise)
	  {
	    LCD_GUI_screen_number = LCD_GUI_Main_Config_Mode;
	  }
	  else if(Rotary_Encoder.parameters.anticlockwise)
	  {
	    LCD_GUI_screen_number = LCD_GUI_Main_RPY_filter;
	  }

	  if(Rotary_Encoder.parameters.switch_state)
	  {
	    __disable_irq();
		MPU6050_calibrate(imu.calibration_axis,imu.N_calibration_points);
		__enable_irq();
		MP_Init();
	  }
    }
	break;

  /**************	Configuration screen. ************/

  	case(LCD_GUI_Config_Menu_A):
     {
      LCD_GUI_Config_Screen_A();

      if( Rotary_Encoder.parameters.clockwise )
      {
      	LCD_GUI_screen_number = LCD_GUI_Config_Menu_B;
      }
      if( Rotary_Encoder.parameters.anticlockwise )
      {
      	LCD_GUI_screen_number =	LCD_GUI_Config_Menu_F;
      }
      if( Rotary_Encoder.parameters.switch_state )
      {

        LCD_GUI_screen_number = LCD_GUI_IMU_Config_Menu_A;
      }
     }
    break;

    case(LCD_GUI_Config_Menu_B):
     {
      LCD_GUI_Config_Screen_B();

      if( Rotary_Encoder.parameters.clockwise )
      {
      	LCD_GUI_screen_number = LCD_GUI_Config_Menu_C;
      }
      if( Rotary_Encoder.parameters.anticlockwise )
      {
      	LCD_GUI_screen_number = LCD_GUI_Config_Menu_A;
      }
      if( Rotary_Encoder.parameters.switch_state )
      {
        LCD_GUI_screen_number = LCD_GUI_CF_Menu_A;
      }
     }
    break;

    case(LCD_GUI_Config_Menu_C):
     {
      LCD_GUI_Config_Screen_C();

      if( Rotary_Encoder.parameters.clockwise )
      {
      	LCD_GUI_screen_number = LCD_GUI_Config_Menu_D;
      }
      if( Rotary_Encoder.parameters.anticlockwise )
      {
      	LCD_GUI_screen_number =	LCD_GUI_Config_Menu_B;
      }
      if( Rotary_Encoder.parameters.switch_state )
      {
        LCD_GUI_screen_number = LCD_GUI_Tilt_Detection_Menu_A;
      }
     }
    break;

    case(LCD_GUI_Config_Menu_D):
     {
      LCD_GUI_Config_Screen_D();

      if( Rotary_Encoder.parameters.clockwise )
      {
      	LCD_GUI_screen_number = LCD_GUI_Config_Menu_E;
      }
      if( Rotary_Encoder.parameters.anticlockwise )
      {
      	LCD_GUI_screen_number = LCD_GUI_Config_Menu_C;
      }
      if( Rotary_Encoder.parameters.switch_state )
      {
        LCD_GUI_screen_number = LCD_GUI_Vibration_Detection_Menu_A;
      }
     }
    break;

    case(LCD_GUI_Config_Menu_E):
     {
      LCD_GUI_Config_Screen_E();

      if( Rotary_Encoder.parameters.clockwise )
      {
      	LCD_GUI_screen_number = LCD_GUI_Config_Menu_F;
      }
      if( Rotary_Encoder.parameters.anticlockwise )
      {
      	LCD_GUI_screen_number = LCD_GUI_Config_Menu_D;
      }
      if( Rotary_Encoder.parameters.switch_state )
      {
        LCD_GUI_screen_number = LCD_GUI_LED_Config_Menu_A;
      }
     }
    break;

    case(LCD_GUI_Config_Menu_F):
     {
      LCD_GUI_Config_Screen_F();

      if( Rotary_Encoder.parameters.clockwise )
      {
      	LCD_GUI_screen_number = LCD_GUI_Config_Menu_A;
      }
      if( Rotary_Encoder.parameters.anticlockwise )
      {
      	LCD_GUI_screen_number = LCD_GUI_Config_Menu_E;
      }

      if( Rotary_Encoder.parameters.switch_state )
      {
    	  __disable_irq();
    	  System_SysTick_delay_ms(100);
    	  Config_write();
    	  __enable_irq();

    	  LCD_GUI_screen_number = LCD_GUI_Main_Config_Mode;
      }
     }
    break;

  /***************	Configuration screen: IMU settings  ************/

    case(LCD_GUI_IMU_Config_Menu_A):
	{
      LCD_GUI_Config_Screen_IMU_settings_A();

      if( Rotary_Encoder.parameters.switch_state )
      {
        if( LCD_GUI_selected )
        {
        	LCD_GUI_selected = 0;
        }
        else
        {
        	LCD_GUI_selected = 1;
        }
      }

      if( Rotary_Encoder.parameters.clockwise && !LCD_GUI_selected )
      {
    	 LCD_GUI_screen_number =  LCD_GUI_IMU_Config_Menu_B;
      }
      if( Rotary_Encoder.parameters.anticlockwise && !LCD_GUI_selected )
      {
        LCD_GUI_screen_number = LCD_GUI_IMU_Config_Menu_F;
      }
      if( Rotary_Encoder.parameters.clockwise && LCD_GUI_selected )
      {
    	 imu.sensor_configurations.sensor_configuration_segments.accel_fs_config++;

    	 if(imu.sensor_configurations.sensor_configuration_segments.accel_fs_config > ACCEL_FS_16_G)
    	 {
    	   imu.sensor_configurations.sensor_configuration_segments.accel_fs_config = ACCEL_FS_16_G;
    	 }

    	 IMU_settings_SMF.IMU_settings_SMF_segments.ACCEL_FS =
    	 imu.sensor_configurations.sensor_configuration_segments.accel_fs_config;
      }
      if( Rotary_Encoder.parameters.anticlockwise && LCD_GUI_selected )
      {
        if(imu.sensor_configurations.sensor_configuration_segments.accel_fs_config)
        {
        	imu.sensor_configurations.sensor_configuration_segments.accel_fs_config--;
        }

        IMU_settings_SMF.IMU_settings_SMF_segments.ACCEL_FS =
        imu.sensor_configurations.sensor_configuration_segments.accel_fs_config;
      }

	}
    break;

    case(LCD_GUI_IMU_Config_Menu_B):
	{
      LCD_GUI_Config_Screen_IMU_settings_B();

      if( Rotary_Encoder.parameters.switch_state )
      {
        if( LCD_GUI_selected )
        {
        	LCD_GUI_selected = 0;
        }
        else
        {
        	LCD_GUI_selected = 1;
        }
      }

      if( Rotary_Encoder.parameters.clockwise && !LCD_GUI_selected)
      {
    	  LCD_GUI_screen_number =  LCD_GUI_IMU_Config_Menu_C;
      }
      if( Rotary_Encoder.parameters.anticlockwise && !LCD_GUI_selected)
      {
    	  LCD_GUI_screen_number = LCD_GUI_IMU_Config_Menu_A;
      }
      if( Rotary_Encoder.parameters.clockwise && LCD_GUI_selected)
      {
    	  imu.sensor_configurations.sensor_configuration_segments.gyro_fs_config++;

    	  if(imu.sensor_configurations.sensor_configuration_segments.gyro_fs_config > GYRO_FS_2000_DPS)
    	  {
    		  imu.sensor_configurations.sensor_configuration_segments.gyro_fs_config = GYRO_FS_2000_DPS;
    	  }

    	  IMU_settings_SMF.IMU_settings_SMF_segments.GYRO_FS =
    	  imu.sensor_configurations.sensor_configuration_segments.gyro_fs_config;
      }
      if( Rotary_Encoder.parameters.anticlockwise && LCD_GUI_selected)
      {
    	  if(imu.sensor_configurations.sensor_configuration_segments.gyro_fs_config)
    	  {
    		  imu.sensor_configurations.sensor_configuration_segments.gyro_fs_config--;
    	  }

    	  IMU_settings_SMF.IMU_settings_SMF_segments.GYRO_FS =
    	  imu.sensor_configurations.sensor_configuration_segments.gyro_fs_config;

    	  LCD_GUI_screen_number = LCD_GUI_IMU_Config_Menu_A;
      }

	}
    break;

    case(LCD_GUI_IMU_Config_Menu_C):
	{
      LCD_GUI_Config_Screen_IMU_settings_C();

      if( Rotary_Encoder.parameters.switch_state )
      {
        if( LCD_GUI_selected )
        {
        	LCD_GUI_selected = 0;
        }
        else
        {
        	LCD_GUI_selected = 1;
        }
      }

      if( Rotary_Encoder.parameters.clockwise && !LCD_GUI_selected)
      {
    	  LCD_GUI_screen_number =  LCD_GUI_IMU_Config_Menu_D;
      }
      if( Rotary_Encoder.parameters.anticlockwise && !LCD_GUI_selected)
      {
    	  LCD_GUI_screen_number = LCD_GUI_IMU_Config_Menu_B;
      }
      if( Rotary_Encoder.parameters.clockwise && LCD_GUI_selected)
      {
    	  imu.N_calibration_points++;

    	  IMU_calibration_SMF.IMU_calibration_SMF_segments.points =
    	  imu.N_calibration_points;
      }
      if( Rotary_Encoder.parameters.anticlockwise && LCD_GUI_selected)
      {
    	  if( imu.N_calibration_points )
    	  {
    		  imu.N_calibration_points--;
    	  }

    	  IMU_calibration_SMF.IMU_calibration_SMF_segments.points =
    	  imu.N_calibration_points;
      }
	}
    break;

    case(LCD_GUI_IMU_Config_Menu_D):
	{
      LCD_GUI_Config_Screen_IMU_settings_D();

      if( Rotary_Encoder.parameters.switch_state )
      {
        if( LCD_GUI_selected )
        {
        	LCD_GUI_selected = 0;
        }
        else
        {
        	LCD_GUI_selected = 1;
        }
      }

      if( Rotary_Encoder.parameters.clockwise && !LCD_GUI_selected )
      {
    	  LCD_GUI_screen_number =  LCD_GUI_IMU_Config_Menu_E;
      }
      if( Rotary_Encoder.parameters.anticlockwise && !LCD_GUI_selected )
      {
    	  LCD_GUI_screen_number = LCD_GUI_IMU_Config_Menu_C;
      }
      if( Rotary_Encoder.parameters.clockwise && LCD_GUI_selected )
      {
    	  imu.calibration_interval_ms++;

    	  IMU_calibration_SMF.IMU_calibration_SMF_segments.interval =
    	  imu.calibration_interval_ms;
      }
      if( Rotary_Encoder.parameters.anticlockwise && LCD_GUI_selected )
      {
    	  if( imu.calibration_interval_ms )
    	  {
    		  imu.calibration_interval_ms--;
    	  }

    	  IMU_calibration_SMF.IMU_calibration_SMF_segments.interval =
    	  imu.calibration_interval_ms;
      }

	}
    break;


    case(LCD_GUI_IMU_Config_Menu_E):
	{
      LCD_GUI_Config_Screen_IMU_settings_E();

      if( Rotary_Encoder.parameters.switch_state )
      {
        if( LCD_GUI_selected )
        {
        	LCD_GUI_selected = 0;
        }
        else
        {
        	LCD_GUI_selected = 1;
        }
      }

      if( Rotary_Encoder.parameters.clockwise && !LCD_GUI_selected)
      {
    	  LCD_GUI_screen_number =  LCD_GUI_IMU_Config_Menu_F;
      }
      if( Rotary_Encoder.parameters.anticlockwise && !LCD_GUI_selected)
      {
    	  LCD_GUI_screen_number = LCD_GUI_IMU_Config_Menu_D;
      }
      if( Rotary_Encoder.parameters.clockwise && LCD_GUI_selected)
      {
    	  imu.calibration_axis++;

    	  if(imu.calibration_axis > Z_Negative )
    	  {
            imu.calibration_axis = Z_Negative;
    	  }

    	  IMU_calibration_SMF.IMU_calibration_SMF_segments.axis =
    	  imu.calibration_axis;
      }
      if( Rotary_Encoder.parameters.anticlockwise && LCD_GUI_selected)
      {
    	  if( imu.calibration_axis )
    	  {
    		  imu.calibration_axis--;
    	  }

    	  IMU_calibration_SMF.IMU_calibration_SMF_segments.axis =
    	  imu.calibration_axis;
      }


	}
    break;

    case(LCD_GUI_IMU_Config_Menu_F):
	{
      LCD_GUI_Config_Screen_IMU_settings_F();

      if( Rotary_Encoder.parameters.clockwise )
      {
    	  LCD_GUI_screen_number =  LCD_GUI_IMU_Config_Menu_F;
      }
      if( Rotary_Encoder.parameters.anticlockwise )
      {
    	  LCD_GUI_screen_number = LCD_GUI_IMU_Config_Menu_E;
      }
      if( Rotary_Encoder.parameters.switch_state )
      {
    	  LCD_GUI_screen_number = LCD_GUI_Config_Menu_A;
      }
	}
    break;


    /***************	Configuration screen: Complementary filter  ************/

      case(LCD_GUI_CF_Menu_A):
  	{
        LCD_GUI_Config_Screen_CF_settings_A();

        if( Rotary_Encoder.parameters.switch_state )
        {
          if( LCD_GUI_selected )
          {
          	LCD_GUI_selected = 0;
          }
          else
          {
          	LCD_GUI_selected = 1;
          }
        }

        if( Rotary_Encoder.parameters.clockwise && !LCD_GUI_selected)
        {
      	 LCD_GUI_screen_number =  LCD_GUI_CF_Menu_B;
        }
        if( Rotary_Encoder.parameters.anticlockwise && !LCD_GUI_selected)
        {
          LCD_GUI_screen_number = LCD_GUI_CF_Menu_A;
        }
        if( Rotary_Encoder.parameters.clockwise && LCD_GUI_selected)
        {
      	 Motion_Processing.roll_coeff += 0.001f;

      	 if(Motion_Processing.roll_coeff > 1.0f)
      	 {
      		 Motion_Processing.roll_coeff = 1.0f;
      	 }

          parameter.value = Motion_Processing.roll_coeff;
          parameter.startBit = Roll_coeff_SBIT;
          parameter.bitLength = Roll_coeff_BLEN;
          parameter.dataType = UNSIGNED;
          parameter.offset = Roll_coeff_OFFSET;
          parameter.pFrame = &CF_coeff_SMF;
          parameter.factor = Roll_coeff_FACTOR;

          SDCP_WPF(&parameter);
        }
        if( Rotary_Encoder.parameters.anticlockwise && LCD_GUI_selected)
        {
         	 Motion_Processing.roll_coeff -= 0.001f;

         	 if(Motion_Processing.roll_coeff < 0.0f)
         	 {
         		 Motion_Processing.roll_coeff = 0.0f;
         	 }

             parameter.value = Motion_Processing.roll_coeff;
             parameter.startBit = Roll_coeff_SBIT;
             parameter.bitLength = Roll_coeff_BLEN;
             parameter.dataType = UNSIGNED;
             parameter.offset = Roll_coeff_OFFSET;
             parameter.pFrame = &CF_coeff_SMF;
             parameter.factor = Roll_coeff_FACTOR;

             SDCP_WPF(&parameter);
        }


  	}
      break;

      case(LCD_GUI_CF_Menu_B):
  	{
        LCD_GUI_Config_Screen_CF_settings_B();

        if( Rotary_Encoder.parameters.switch_state )
        {
          if( LCD_GUI_selected )
          {
          	LCD_GUI_selected = 0;
          }
          else
          {
          	LCD_GUI_selected = 1;
          }
        }

        if( Rotary_Encoder.parameters.clockwise && !LCD_GUI_selected)
        {
      	  LCD_GUI_screen_number =  LCD_GUI_CF_Menu_C;
        }
        if( Rotary_Encoder.parameters.anticlockwise && !LCD_GUI_selected)
        {
      	  LCD_GUI_screen_number = LCD_GUI_CF_Menu_A;
        }
        if( Rotary_Encoder.parameters.clockwise && LCD_GUI_selected)
        {
      	 Motion_Processing.pitch_coeff += 0.001f;

      	 if(Motion_Processing.pitch_coeff > 1.0f)
      	 {
      		 Motion_Processing.pitch_coeff = 1.0f;
      	 }

         parameter.value = Motion_Processing.pitch_coeff;
         parameter.startBit = Pitch_coeff_SBIT;
         parameter.bitLength = Pitch_coeff_BLEN;
         parameter.dataType = UNSIGNED;
         parameter.offset = Pitch_coeff_OFFSET;
         parameter.pFrame = &CF_coeff_SMF;
         parameter.factor = Pitch_coeff_FACTOR;

         SDCP_WPF(&parameter);

        }
        if( Rotary_Encoder.parameters.anticlockwise && LCD_GUI_selected)
        {
         	 Motion_Processing.pitch_coeff -= 0.001f;

         	 if(Motion_Processing.pitch_coeff < 0.0f)
         	 {
         		 Motion_Processing.pitch_coeff = 0.0f;
         	 }

             parameter.value = Motion_Processing.pitch_coeff;
             parameter.startBit = Pitch_coeff_SBIT;
             parameter.bitLength = Pitch_coeff_BLEN;
             parameter.dataType = UNSIGNED;
             parameter.offset = Pitch_coeff_OFFSET;
             parameter.pFrame = &CF_coeff_SMF;
             parameter.factor = Pitch_coeff_FACTOR;

             SDCP_WPF(&parameter);
        }
  	}
      break;

      case(LCD_GUI_CF_Menu_C):
  	{
        LCD_GUI_Config_Screen_CF_settings_C();

        if( Rotary_Encoder.parameters.switch_state )
        {
          if( LCD_GUI_selected )
          {
          	LCD_GUI_selected = 0;
          }
          else
          {
          	LCD_GUI_selected = 1;
          }
        }

        if( Rotary_Encoder.parameters.clockwise && !LCD_GUI_selected)
        {
      	  LCD_GUI_screen_number =  LCD_GUI_CF_Menu_D;
        }
        if( Rotary_Encoder.parameters.anticlockwise && !LCD_GUI_selected)
        {
      	  LCD_GUI_screen_number = LCD_GUI_CF_Menu_B;
        }

        if( Rotary_Encoder.parameters.clockwise && LCD_GUI_selected)
        {
      	 Motion_Processing.yaw_coeff += 0.001f;

      	 if(Motion_Processing.yaw_coeff > 1.0f)
      	 {
      		 Motion_Processing.yaw_coeff = 1.0f;
      	 }

         parameter.value = Motion_Processing.yaw_coeff;
         parameter.startBit = Yaw_coeff_SBIT;
         parameter.bitLength = Yaw_coeff_BLEN;
         parameter.dataType = UNSIGNED;
         parameter.offset = Yaw_coeff_OFFSET;
         parameter.pFrame = &CF_coeff_SMF;
         parameter.factor = Yaw_coeff_FACTOR;

         SDCP_WPF(&parameter);
        }
        if( Rotary_Encoder.parameters.anticlockwise && LCD_GUI_selected)
        {
         	 Motion_Processing.yaw_coeff -= 0.001f;

         	 if(Motion_Processing.yaw_coeff < 0.0f)
         	 {
         		 Motion_Processing.yaw_coeff = 0.0f;
         	 }

             parameter.value = Motion_Processing.yaw_coeff;
             parameter.startBit = Yaw_coeff_SBIT;
             parameter.bitLength = Yaw_coeff_BLEN;
             parameter.dataType = UNSIGNED;
             parameter.offset = Yaw_coeff_OFFSET;
             parameter.pFrame = &CF_coeff_SMF;
             parameter.factor = Yaw_coeff_FACTOR;

             SDCP_WPF(&parameter);
        }
  	}
      break;

      case(LCD_GUI_CF_Menu_D):
  	{
        LCD_GUI_Config_Screen_CF_settings_D();

        if( Rotary_Encoder.parameters.clockwise )
        {
      	  LCD_GUI_screen_number =  LCD_GUI_CF_Menu_D;
        }
        if( Rotary_Encoder.parameters.anticlockwise )
        {
      	  LCD_GUI_screen_number = LCD_GUI_CF_Menu_C;
        }
        if( Rotary_Encoder.parameters.switch_state)
        {
          LCD_GUI_screen_number = LCD_GUI_Config_Menu_B;
        }
  	}
      break;


      /***************	Configuration screen: Tilt threshold  ************/

        case(LCD_GUI_Tilt_Detection_Menu_A):
    	{
          LCD_GUI_Config_Screen_Tilt_Detection_settings_A();

          if( Rotary_Encoder.parameters.switch_state )
          {
            if( LCD_GUI_selected )
            {
            	LCD_GUI_selected = 0;
            }
            else
            {
            	LCD_GUI_selected = 1;
            }
          }

          if( Rotary_Encoder.parameters.clockwise && !LCD_GUI_selected)
          {
        	 LCD_GUI_screen_number =  LCD_GUI_Tilt_Detection_Menu_B;
          }
          if( Rotary_Encoder.parameters.anticlockwise && !LCD_GUI_selected)
          {
            LCD_GUI_screen_number = LCD_GUI_Tilt_Detection_Menu_A;
          }
          if( Rotary_Encoder.parameters.clockwise && LCD_GUI_selected)
          {
        	 Motion_Processing.angle_threshold[0] += 0.1f;

        	 if(Motion_Processing.angle_threshold[0] > 90.0f)
        	 {
        		 Motion_Processing.angle_threshold[0] = 90.0f;
        	 }

             MDT_X_SMF.MDT_X_SMF_segments.angle_threshold =
             Motion_Processing.angle_threshold[0]/X_angle_threshold_FACTOR;
          }
          if( Rotary_Encoder.parameters.anticlockwise && LCD_GUI_selected)
          {
        	  Motion_Processing.angle_threshold[0] -= 0.1f;

        	  if(Motion_Processing.angle_threshold[0] < 0.0f )
        	  {
        		  Motion_Processing.angle_threshold[0] = 0.0f;
        	  }

              MDT_X_SMF.MDT_X_SMF_segments.angle_threshold =
              Motion_Processing.angle_threshold[0]/X_angle_threshold_FACTOR;
          }

    	}
        break;

        case(LCD_GUI_Tilt_Detection_Menu_B):
    	{
          LCD_GUI_Config_Screen_Tilt_Detection_settings_B();

          if( Rotary_Encoder.parameters.switch_state )
          {
            if( LCD_GUI_selected )
            {
            	LCD_GUI_selected = 0;
            }
            else
            {
            	LCD_GUI_selected = 1;
            }
          }

          if( Rotary_Encoder.parameters.clockwise && !LCD_GUI_selected)
          {
        	  LCD_GUI_screen_number =  LCD_GUI_Tilt_Detection_Menu_C;
          }
          if( Rotary_Encoder.parameters.anticlockwise && !LCD_GUI_selected)
          {
        	  LCD_GUI_screen_number = LCD_GUI_Tilt_Detection_Menu_A;
          }

          if( Rotary_Encoder.parameters.clockwise && LCD_GUI_selected)
          {
        	 Motion_Processing.angle_threshold[1] += 0.1f;

        	 if(Motion_Processing.angle_threshold[1] > 90.0f)
        	 {
        		 Motion_Processing.angle_threshold[1] = 90.0f;
        	 }

             MDT_Y_SMF.MDT_Y_SMF_segments.angle_threshold =
             Motion_Processing.angle_threshold[1]/Y_angle_threshold_FACTOR;
          }
          if( Rotary_Encoder.parameters.anticlockwise && LCD_GUI_selected)
          {
        	  Motion_Processing.angle_threshold[1] -= 0.1f;

        	  if(Motion_Processing.angle_threshold[1] < 0.0f )
        	  {
        		  Motion_Processing.angle_threshold[1] = 0.0f;
        	  }

              MDT_Y_SMF.MDT_Y_SMF_segments.angle_threshold =
              Motion_Processing.angle_threshold[1]/Y_angle_threshold_FACTOR;
          }

    	}
        break;

        case(LCD_GUI_Tilt_Detection_Menu_C):
    	{
          LCD_GUI_Config_Screen_Tilt_Detection_settings_C();

          if( Rotary_Encoder.parameters.switch_state )
          {
            if( LCD_GUI_selected )
            {
            	LCD_GUI_selected = 0;
            }
            else
            {
            	LCD_GUI_selected = 1;
            }
          }


          if( Rotary_Encoder.parameters.clockwise && !LCD_GUI_selected)
          {
        	  LCD_GUI_screen_number =  LCD_GUI_Tilt_Detection_Menu_D;
          }
          if( Rotary_Encoder.parameters.anticlockwise && !LCD_GUI_selected)
          {
        	  LCD_GUI_screen_number = LCD_GUI_Tilt_Detection_Menu_B;
          }

          if( Rotary_Encoder.parameters.clockwise && LCD_GUI_selected)
          {
        	 Motion_Processing.angle_threshold[2] += 0.1f;

        	 if(Motion_Processing.angle_threshold[2] > 90.0f)
        	 {
        		 Motion_Processing.angle_threshold[2] = 90.0f;
        	 }

             MDT_Z_SMF.MDT_Z_SMF_segments.angle_threshold =
             Motion_Processing.angle_threshold[2]/Z_angle_threshold_FACTOR;
          }
          if( Rotary_Encoder.parameters.anticlockwise && LCD_GUI_selected)
          {
        	  Motion_Processing.angle_threshold[2] -= 0.1f;

        	  if(Motion_Processing.angle_threshold[2] < 0.0f )
        	  {
        		  Motion_Processing.angle_threshold[2] = 0.0f;
        	  }

              MDT_Z_SMF.MDT_Z_SMF_segments.angle_threshold =
              Motion_Processing.angle_threshold[2]/Z_angle_threshold_FACTOR;
          }

    	}
        break;

        case(LCD_GUI_Tilt_Detection_Menu_D):
    	{
          LCD_GUI_Config_Screen_Tilt_Detection_settings_D();

          if( Rotary_Encoder.parameters.clockwise )
          {
        	  LCD_GUI_screen_number =  LCD_GUI_Tilt_Detection_Menu_D;
          }
          if( Rotary_Encoder.parameters.anticlockwise )
          {
        	  LCD_GUI_screen_number = LCD_GUI_Tilt_Detection_Menu_C;
          }
          if( Rotary_Encoder.parameters.switch_state)
          {
            LCD_GUI_screen_number = LCD_GUI_Config_Menu_C;
          }
    	}
        break;


        /***************	Configuration screen: Vibration threshold  ************/

          case(LCD_GUI_Vibration_Detection_Menu_A):
      	{
            LCD_GUI_Config_Screen_Vibration_Detection_settings_A();

            if( Rotary_Encoder.parameters.switch_state )
            {
              if( LCD_GUI_selected )
              {
              	LCD_GUI_selected = 0;
              }
              else
              {
              	LCD_GUI_selected = 1;
              }
            }


            if( Rotary_Encoder.parameters.clockwise && !LCD_GUI_selected)
            {
          	 LCD_GUI_screen_number =  LCD_GUI_Vibration_Detection_Menu_B;
            }
            if( Rotary_Encoder.parameters.anticlockwise && !LCD_GUI_selected)
            {
              LCD_GUI_screen_number = LCD_GUI_Vibration_Detection_Menu_A;
            }
            if( Rotary_Encoder.parameters.clockwise && LCD_GUI_selected)
            {
          	 Motion_Processing.vibration_threshold[0] += 0.1f;

          	 if(Motion_Processing.vibration_threshold[0] > 90.0f)
          	 {
          		 Motion_Processing.vibration_threshold[0] = 90.0f;
          	 }

             MDT_X_SMF.MDT_X_SMF_segments.vibration_threshold =
             (uint32_t)Motion_Processing.vibration_threshold[0];
            }
            if( Rotary_Encoder.parameters.anticlockwise && LCD_GUI_selected)
            {
          	  Motion_Processing.vibration_threshold[0] -= 0.1f;

          	  if(Motion_Processing.vibration_threshold[0] < 0.0f )
          	  {
          		  Motion_Processing.vibration_threshold[0] = 0.0f;
          	  }

              MDT_X_SMF.MDT_X_SMF_segments.vibration_threshold =
              (uint32_t)Motion_Processing.vibration_threshold[0];
            }


      	}
          break;

          case(LCD_GUI_Vibration_Detection_Menu_B):
      	{
            LCD_GUI_Config_Screen_Vibration_Detection_settings_B();

            if( Rotary_Encoder.parameters.switch_state )
            {
              if( LCD_GUI_selected )
              {
              	LCD_GUI_selected = 0;
              }
              else
              {
              	LCD_GUI_selected = 1;
              }
            }

            if( Rotary_Encoder.parameters.clockwise && !LCD_GUI_selected)
            {
          	  LCD_GUI_screen_number =  LCD_GUI_Vibration_Detection_Menu_C;
            }
            if( Rotary_Encoder.parameters.anticlockwise && !LCD_GUI_selected)
            {
          	  LCD_GUI_screen_number = LCD_GUI_Vibration_Detection_Menu_A;
            }

            if( Rotary_Encoder.parameters.clockwise && LCD_GUI_selected)
            {
          	 Motion_Processing.vibration_threshold[1] += 0.1f;

          	 if(Motion_Processing.vibration_threshold[1] > 90.0f)
          	 {
          		 Motion_Processing.vibration_threshold[1] = 90.0f;
          	 }

             MDT_Y_SMF.MDT_Y_SMF_segments.vibration_threshold =
             (uint32_t)Motion_Processing.vibration_threshold[1];

            }
            if( Rotary_Encoder.parameters.anticlockwise && LCD_GUI_selected)
            {
          	  Motion_Processing.vibration_threshold[1] -= 0.1f;

          	  if(Motion_Processing.vibration_threshold[1] < 0.0f )
          	  {
          		  Motion_Processing.vibration_threshold[1] = 0.0f;
          	  }

              MDT_Y_SMF.MDT_Y_SMF_segments.vibration_threshold =
              (uint32_t)Motion_Processing.vibration_threshold[1];
            }

      	}
          break;

          case(LCD_GUI_Vibration_Detection_Menu_C):
      	{
            LCD_GUI_Config_Screen_Vibration_Detection_settings_C();

            if( Rotary_Encoder.parameters.switch_state )
            {
              if( LCD_GUI_selected )
              {
              	LCD_GUI_selected = 0;
              }
              else
              {
              	LCD_GUI_selected = 1;
              }
            }


            if( Rotary_Encoder.parameters.clockwise && !LCD_GUI_selected)
            {
          	  LCD_GUI_screen_number =  LCD_GUI_Vibration_Detection_Menu_D;
            }
            if( Rotary_Encoder.parameters.anticlockwise && !LCD_GUI_selected)
            {
          	  LCD_GUI_screen_number = LCD_GUI_Vibration_Detection_Menu_B;
            }

            if( Rotary_Encoder.parameters.clockwise && LCD_GUI_selected)
            {
          	 Motion_Processing.vibration_threshold[2] += 0.1f;

          	 if(Motion_Processing.vibration_threshold[2] > 90.0f)
          	 {
          		 Motion_Processing.vibration_threshold[2] = 90.0f;
          	 }

             MDT_Z_SMF.MDT_Z_SMF_segments.vibration_threshold =
             (uint32_t)Motion_Processing.vibration_threshold[2];
            }
            if( Rotary_Encoder.parameters.anticlockwise && LCD_GUI_selected)
            {
          	  Motion_Processing.vibration_threshold[2] -= 0.1f;

          	  if(Motion_Processing.vibration_threshold[2] < 0.0f )
          	  {
          		  Motion_Processing.vibration_threshold[2] = 0.0f;
          	  }

              MDT_Z_SMF.MDT_Z_SMF_segments.vibration_threshold =
              (uint32_t)Motion_Processing.vibration_threshold[2];
            }

      	}
          break;

          case(LCD_GUI_Vibration_Detection_Menu_D):
      	{
            LCD_GUI_Config_Screen_Vibration_Detection_settings_D();

            if( Rotary_Encoder.parameters.clockwise )
            {
          	  LCD_GUI_screen_number =  LCD_GUI_Vibration_Detection_Menu_D;
            }
            if( Rotary_Encoder.parameters.anticlockwise )
            {
          	  LCD_GUI_screen_number = LCD_GUI_Vibration_Detection_Menu_C;
            }
            if( Rotary_Encoder.parameters.switch_state)
            {
              LCD_GUI_screen_number = LCD_GUI_Config_Menu_C;
            }
      	}
          break;


          /***************	Configuration screen: LED configurations.  ************/

            case(LCD_GUI_LED_Config_Menu_A):
        	{
              LCD_GUI_Config_Screen_LED_settings_A();

              if( Rotary_Encoder.parameters.switch_state )
              {
                if( LCD_GUI_selected )
                {
                	LCD_GUI_selected = 0;
                }
                else
                {
                	LCD_GUI_selected = 1;
                }
              }


              if( Rotary_Encoder.parameters.clockwise && !LCD_GUI_selected)
              {
            	 LCD_GUI_screen_number =  LCD_GUI_LED_Config_Menu_B;
              }
              if( Rotary_Encoder.parameters.anticlockwise && !LCD_GUI_selected)
              {
                LCD_GUI_screen_number = LCD_GUI_LED_Config_Menu_A;
              }
              if( Rotary_Encoder.parameters.clockwise && LCD_GUI_selected)
              {
            	 LED.LED_parameters.LED1_mode++;

            	 if(LED.LED_parameters.LED1_mode > PWM_test)
            	 {
            		 LED.LED_parameters.LED1_mode = PWM_test;
            	 }

            	 LED_CNTRL_SMF.LED_CNTRL_SMF_segments.XLED_mode =
            	 LED.LED_parameters.LED1_mode;
              }
              if( Rotary_Encoder.parameters.anticlockwise && LCD_GUI_selected)
              {
                if(LED.LED_parameters.LED1_mode)
                {
                	LED.LED_parameters.LED1_mode--;
                }

           	    LED_CNTRL_SMF.LED_CNTRL_SMF_segments.XLED_mode =
           	    LED.LED_parameters.LED1_mode;
              }

        	}
            break;

            case(LCD_GUI_LED_Config_Menu_B):
        	{
              LCD_GUI_Config_Screen_LED_settings_B();

              if( Rotary_Encoder.parameters.switch_state )
              {
                if( LCD_GUI_selected )
                {
                	LCD_GUI_selected = 0;
                }
                else
                {
                	LCD_GUI_selected = 1;
                }
              }


              if( Rotary_Encoder.parameters.clockwise && !LCD_GUI_selected)
              {
            	  LCD_GUI_screen_number =  LCD_GUI_LED_Config_Menu_C;
              }
              if( Rotary_Encoder.parameters.anticlockwise && !LCD_GUI_selected)
              {
            	  LCD_GUI_screen_number = LCD_GUI_LED_Config_Menu_A;
              }
              if( Rotary_Encoder.parameters.clockwise && LCD_GUI_selected)
              {
            	 LED.LED_parameters.LED2_mode++;

            	 if(LED.LED_parameters.LED2_mode > PWM_test)
            	 {
            		 LED.LED_parameters.LED2_mode = PWM_test;
            	 }

            	 LED_CNTRL_SMF.LED_CNTRL_SMF_segments.YLED_mode =
            	 LED.LED_parameters.LED2_mode;
              }
              if( Rotary_Encoder.parameters.anticlockwise && LCD_GUI_selected)
              {
                if(LED.LED_parameters.LED2_mode)
                {
                	LED.LED_parameters.LED2_mode--;
                }

           	    LED_CNTRL_SMF.LED_CNTRL_SMF_segments.YLED_mode =
           	    LED.LED_parameters.LED2_mode;
              }

        	}
            break;

            case(LCD_GUI_LED_Config_Menu_C):
        	{
              LCD_GUI_Config_Screen_LED_settings_C();

              if( Rotary_Encoder.parameters.switch_state )
              {
                if( LCD_GUI_selected )
                {
                	LCD_GUI_selected = 0;
                }
                else
                {
                	LCD_GUI_selected = 1;
                }
              }


              if( Rotary_Encoder.parameters.clockwise && !LCD_GUI_selected)
              {
            	  LCD_GUI_screen_number =  LCD_GUI_LED_Config_Menu_D;
              }
              if( Rotary_Encoder.parameters.anticlockwise && !LCD_GUI_selected)
              {
            	  LCD_GUI_screen_number = LCD_GUI_LED_Config_Menu_B;
              }

              if( Rotary_Encoder.parameters.clockwise && LCD_GUI_selected)
              {
            	 LED.LED_parameters.LED3_mode++;

            	 if(LED.LED_parameters.LED3_mode > PWM_test)
            	 {
            		 LED.LED_parameters.LED3_mode = PWM_test;
            	 }

            	 LED_CNTRL_SMF.LED_CNTRL_SMF_segments.ZLED_mode =
            	 LED.LED_parameters.LED3_mode;
              }
              if( Rotary_Encoder.parameters.anticlockwise && LCD_GUI_selected)
              {
                if(LED.LED_parameters.LED3_mode)
                {
                	LED.LED_parameters.LED3_mode--;
                }

           	   LED_CNTRL_SMF.LED_CNTRL_SMF_segments.ZLED_mode =
           	   LED.LED_parameters.LED3_mode;
              }

        	}
            break;

            case(LCD_GUI_LED_Config_Menu_D):
        	{
              LCD_GUI_Config_Screen_LED_settings_D();

              if( Rotary_Encoder.parameters.clockwise )
              {
            	  LCD_GUI_screen_number =  LCD_GUI_LED_Config_Menu_D;
              }
              if( Rotary_Encoder.parameters.anticlockwise )
              {
            	  LCD_GUI_screen_number = LCD_GUI_LED_Config_Menu_C;
              }
              if( Rotary_Encoder.parameters.switch_state)
              {
                LCD_GUI_screen_number = LCD_GUI_Config_Menu_D;
              }
        	}
            break;


  }

  /* Reset rotary encoder variables and update display. */


  Rotary_Encoder.parameters.anticlockwise = 0;
  Rotary_Encoder.parameters.clockwise = 0;
  Rotary_Encoder.parameters.switch_state = 0;

  if( Nokia5110.pending == 0 )
  {
    Nokia5110_updateDisplay();
  }
}


