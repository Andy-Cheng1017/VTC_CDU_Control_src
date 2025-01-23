#ifndef PT100_H
#define PT100_H

#include <stdint.h>
#include <math.h>
#include <stdbool.h>

#define R0 100.0f
#define A  3.9083e-3f
#define B -5.775e-7f

void PT100_Init(int32_t pt100_110_real_val, int32_t pt100_200_real_val);

int32_t PT100_CalcTemp(uint8_t adc_gain, unsigned long adc_val);

#endif // PT100_H
