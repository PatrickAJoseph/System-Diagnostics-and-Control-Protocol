
import SDCP_API
import timing

#
# @brief: Function to test LED blinking sequence for device.
#

def LED_GPIO_Test(sdcp):
  
  sdcp.write_parameter_to_frame("LED_OVRRD",1);
  sdcp.write_parameter_to_frame("XLED_mode",5);
  sdcp.write_parameter_to_frame("YLED_mode",5);
  sdcp.write_parameter_to_frame("ZLED_mode",5);
  sdcp.write_frame_to_device("LED_CNTRL");
  timing.delay_ms(250);
  
  XLED_state = [0,1,0,1,0,1,0,1];
  YLED_state = [0,0,1,1,0,0,1,1];
  ZLED_state = [0,0,0,0,1,1,1,1];
  
  for i in range(0,8):
  
    sdcp.write_parameter_to_frame("XLED_state",XLED_state[i]);
    sdcp.write_parameter_to_frame("YLED_state",YLED_state[i]);
    sdcp.write_parameter_to_frame("ZLED_state",ZLED_state[i]);
    sdcp.write_frame_to_device("LED_CNTRL");
    timing.delay_ms(250);

  sdcp.write_parameter_to_frame("LED_OVRRD",1);
  sdcp.write_parameter_to_frame("XLED_mode",0);
  sdcp.write_parameter_to_frame("YLED_mode",0);
  sdcp.write_parameter_to_frame("ZLED_mode",0);
  sdcp.write_frame_to_device("LED_CNTRL");

  timing.delay_ms(250);

  sdcp.write_parameter_to_frame("LED_OVRRD",0);
  sdcp.write_parameter_to_frame("XLED_mode",0);
  sdcp.write_parameter_to_frame("YLED_mode",0);
  sdcp.write_parameter_to_frame("ZLED_mode",0);
  sdcp.write_frame_to_device("LED_CNTRL");

  timing.delay_ms(250);

#
# @brief: Function to test the PWM mode functtionality of LEDs.
#

def LED_PWM_Test(sdcp):
  
  sdcp.write_parameter_to_frame("LED_OVRRD",1);
  sdcp.write_parameter_to_frame("XLED_mode",4);
  sdcp.write_parameter_to_frame("YLED_mode",4);
  sdcp.write_parameter_to_frame("ZLED_mode",4);
  sdcp.write_frame_to_device("LED_CNTRL");
  timing.delay_ms(250);
  
  XLED_PWM = [0,12.5,25,37.5,50,62.5,75,87.5];
  YLED_PWM = [12.5,25.0,37.5,50,62.5,75,87.5,0];
  ZLED_PWM = [25.0,37.5,50,62.5,75,87.5,0,12.5];
  
  for i in range(0,120):
  
    sdcp.write_parameter_to_frame("XLED_Brightness",XLED_PWM[i%8]);
    sdcp.write_parameter_to_frame("YLED_Brightness",YLED_PWM[i%8]);
    sdcp.write_parameter_to_frame("ZLED_Brightness",ZLED_PWM[i%8]);
    sdcp.write_frame_to_device("LED_CNTRL");
    timing.delay_ms(100);

  sdcp.write_parameter_to_frame("LED_OVRRD",1);
  sdcp.write_parameter_to_frame("XLED_mode",0);
  sdcp.write_parameter_to_frame("YLED_mode",0);
  sdcp.write_parameter_to_frame("ZLED_mode",0);
  sdcp.write_frame_to_device("LED_CNTRL");

  timing.delay_ms(250);

  sdcp.write_parameter_to_frame("LED_OVRRD",0);
  sdcp.write_parameter_to_frame("XLED_mode",0);
  sdcp.write_parameter_to_frame("YLED_mode",0);
  sdcp.write_parameter_to_frame("ZLED_mode",0);
  sdcp.write_frame_to_device("LED_CNTRL");

  timing.delay_ms(250);
  

if __name__ == "__main__":

  sdcp = SDCP_API.sdcp()
  sdcp.load_parameter_file("Motion_Processing.param");
  sdcp.open_session("COM7",115200);
 
  #LED_GPIO_Test(sdcp);
  #LED_PWM_Test(sdcp);

  requestFrames = ["IMU_settings","IMU_calibration","IMU_CALIB_XACCEL_Offset","IMU_CALIB_YACCEL_Offset","IMU_CALIB_ZACCEL_Offset"]; 

  #for frameName in requestFrames:
  #  sdcp.print_frame_and_parameter_details(frameName);
  
  sdcp.request_frames(requestFrames[:3]);
  
  for frameName in requestFrames[:3]:
    sdcp.print_frame_packet(frameName);
    #sdcp.print_frame_and_parameter_details(frameName);

  sdcp.close_session();