
#ifndef INC_HPF_H_
#define INC_HPF_H_

/* High pass filter. */

/* Structure for 4th order high-pass filter. */

typedef struct HPF
{
  float output;
  float previous_input;
  float previous_output;
  float a;
  float b;
}HPF;


/* Function to initialize the high-pass filter. */

void HPF_Init(HPF* filter,float f_s,float f_c);

/* Function to update the current high-pass filter based on current input. */

float HPF_Update(HPF* filter, float input);

#endif /* INC_HPF_H_ */
