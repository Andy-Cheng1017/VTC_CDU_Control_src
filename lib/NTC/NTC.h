

#ifndef _NTC_H
#define _NTC_H
/*
  Author:     Nima Askari
  WebSite:    http://www.github.com/NimaLTD
  Instagram:  http://instagram.com/github.NimaLTD
  Youtube:    https://www.youtube.com/channel/UCUhY7qY1klJm1d2kulr9ckw
  
  Version:    1.0.0
  
  
  Reversion History:
  
  (1.0.0)
  First release.
*/

#include <stdint.h>

#define _NTC_R_SERIES         10000.0f
#define _NTC_R_NOMINAL        10000.0f
#define _NTC_TEMP_NOMINAL     25.0f
#define _NTC_ADC_MAX          4095 //  12bit
#define _NTC_BETA             3950

float ntc_convertToC(uint32_t adcValue);
float ntc_convertToF(uint32_t adcValue);
  
#endif
