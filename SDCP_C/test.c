
#include <stdio.h>
#include <stdint.h>

#include "SDCP.h"


union FR1
{
  uint32_t all;
  
  struct seg_FR1
  {
	uint8_t u8 : 8;
    int8_t  i8 : 8;
    uint16_t u16 : 16;
	
  }seg_FR1;
	
}FR1;

union FR2
{
  uint32_t all;

  struct seg_FR2
  {
    uint32_t all;
	
  }seg_FR2;	  

}FR2;


int main()
{
  SDCP_Param parameter = {0};
  
  parameter.pFrame     = &FR2;  
  parameter.startBit   = 24;
  parameter.bitLength  = 8;
  parameter.factor     = 0.1f;
  parameter.offset     = -10.0f;
  parameter.dataType   = SIGNED;
  parameter.value      = -12.0f;
  
  SDCP_WPF(&parameter);
 
  printf("\nEncoded frame : %x  %x  %x  %x", (FR2.all >> 24) & ( 0xFF ),  (FR2.all >> 16) & ( 0xFF ) 
                                           , (FR2.all >> 8)  & ( 0xFF ),  (FR2.all >> 0) & ( 0xFF ) );
										   
  SDCP_RPF(&parameter);

  printf("\nParameter value: %f", parameter.value);  

  return(0);	
}