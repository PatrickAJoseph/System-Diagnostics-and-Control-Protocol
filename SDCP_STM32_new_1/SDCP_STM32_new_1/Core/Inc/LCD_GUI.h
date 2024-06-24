
#ifndef INC_LCD_GUI_H_
#define INC_LCD_GUI_H_

#include "Application.h"

/* Main states/screens of GUI. */

#define LCD_GUI_Main_Initialize					0
#define LCD_GUI_Main_Config_Mode				1
#define LCD_GUI_Main_IMU_accel_raw				2
#define LCD_GUI_Main_IMU_accel_processed		3
#define LCD_GUI_Main_IMU_gyro_raw				4
#define LCD_GUI_Main_IMU_gyro_processed			5
#define LCD_GUI_Main_RPY_accel					6
#define LCD_GUI_Main_RPY_gyro					7
#define LCD_GUI_Main_RPY_filter					8
#define LCD_GUI_Main_Motion_Detection		    9

/* Configuration menu items. */

#define LCD_GUI_Config_Menu_A					10
#define LCD_GUI_Config_Menu_B					11
#define LCD_GUI_Config_Menu_C					12
#define LCD_GUI_Config_Menu_D					13
#define LCD_GUI_Config_Menu_E					14
#define LCD_GUI_Config_Menu_F					15

/* IMU configuration menu items. */

#define LCD_GUI_IMU_Config_Menu_A				16
#define LCD_GUI_IMU_Config_Menu_B				17
#define LCD_GUI_IMU_Config_Menu_C				18
#define LCD_GUI_IMU_Config_Menu_D				19
#define LCD_GUI_IMU_Config_Menu_E				20
#define LCD_GUI_IMU_Config_Menu_F				21

/* Complementary Filter menu items. */

#define LCD_GUI_CF_Menu_A						22
#define LCD_GUI_CF_Menu_B						23
#define LCD_GUI_CF_Menu_C						24
#define LCD_GUI_CF_Menu_D						25

/* Tilt Detection menu items. */

#define LCD_GUI_Tilt_Detection_Menu_A			26
#define LCD_GUI_Tilt_Detection_Menu_B			27
#define LCD_GUI_Tilt_Detection_Menu_C			28
#define LCD_GUI_Tilt_Detection_Menu_D			29

/* Vibration Detection menu items. */

#define LCD_GUI_Vibration_Detection_Menu_A		30
#define LCD_GUI_Vibration_Detection_Menu_B		31
#define LCD_GUI_Vibration_Detection_Menu_C		32
#define LCD_GUI_Vibration_Detection_Menu_D		33

/* LED configuration menu items. */

#define LCD_GUI_LED_Config_Menu_A				34
#define LCD_GUI_LED_Config_Menu_B				35
#define LCD_GUI_LED_Config_Menu_C				36
#define LCD_GUI_LED_Config_Menu_D				37

uint8_t LCD_GUI_screen_number;
uint8_t LCD_GUI_selected;

void LCD_GUI_Init();
void LCD_GUI_Update();

#endif /* INC_LCD_GUI_H_ */
