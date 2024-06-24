
#ifndef INC_ROTARY_ENCODER_H_
#define INC_ROTARY_ENCODER_H_

#include "main.h"
#include "SMFs.h"

volatile union Rotary_Encoder{

  uint32_t all;

  struct parameters{

    uint32_t clockwise:1;
    uint32_t anticlockwise:1;
    uint32_t switch_state:1;
    uint32_t switch_counter:14;
    uint32_t rotation_counter:14;

  }parameters;

}Rotary_Encoder;

void RotaryEncoder_Init();


#endif /* INC_ROTARY_ENCODER_H_ */
