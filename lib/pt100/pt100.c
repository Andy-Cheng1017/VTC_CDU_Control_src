#include "pt100.h"

int32_t PT100_CalcTemp(uint8_t adc_gain, unsigned long adc_val) {
  unsigned long Vdiff_uv = ((adc_val * 125) >> 3) >> adc_gain;

  float pt_ideal_resist = (100.0 + 2920 * Vdiff_uv * 0.000001) / (1.0 - 0.000001 * Vdiff_uv * 10);
  // float pt_real_resist = 0.985451 * pt_ideal_resist - 0.00020694;

  // 使用公式 T = (-A + sqrt(A^2 - 4 * B * (1 - R/R0))) / (2 * B)
  float ratio = pt_ideal_resist / R0;
  float discriminant = A * A - 4.0f * B * (1.0f - ratio);

  if (discriminant < 0) {
    return 0xffff;  // 返回一个无效温度值
  }

  float temperature = (-A + sqrtf(discriminant)) / (2.0f * B);

  return (int32_t)roundf(temperature * 1000);
}