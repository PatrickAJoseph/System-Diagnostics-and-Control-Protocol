
#include "HPF.h"
#include "math.h"

void HPF_Init(HPF* filter,float f_s,float f_c)
{
	float T = 1/f_s;
	float w_c = 2*(float)M_PI*f_c;

	filter->output = 0.0f;
	filter->previous_input = 0.0f;
	filter->previous_output = 0.0f;

	filter->a = 2.0f/(2.0f + T*w_c);
	filter->b = (2.0f - T*w_c)/(2.0f + T*w_c);
}

float HPF_Update(HPF* filter,float input)
{
    filter->output = (( filter->a * ( input - filter->previous_input ) ) + (filter->b * filter->previous_output));

    filter->previous_output = filter->output;
    filter->previous_input = input;

    return(filter->output);
}

