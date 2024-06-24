
#ifndef INC_LED_H_
#define INC_LED_H_

#include "main.h"
#include "Application.h"


typedef enum LED_mode
{
  Angle_detection = 0,
  Vibration_detection = 1,
  Accelerometer_measurement = 2,
  Gyroscope_measurement = 3,
  PWM_test = 4,
  GPIO_test = 5
}LED_mode;

union LED
{
  uint32_t all;

  struct LED_parameters
  {
    uint32_t LED1_duty : 7;
    uint32_t LED2_duty : 7;
    uint32_t LED3_duty : 7;
    uint32_t LED1_mode : 3;
    uint32_t LED2_mode : 3;
    uint32_t LED3_mode : 3;
    uint32_t LED1_state : 1;
    uint32_t LED2_state : 1;
    uint32_t LED3_state : 1;
    uint32_t Test_LED_state : 1;
  }LED_parameters;


}LED;

void LED_Init();
void LED_Update();
uint16_t LED_PWM_Map(uint8_t duty);

#endif /* INC_LED_H_ */
