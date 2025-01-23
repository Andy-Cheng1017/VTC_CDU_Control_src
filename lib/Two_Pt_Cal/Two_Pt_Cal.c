#include "Two_Pt_Cal.h"
#include <stddef.h>
#include <math.h>

void Cal_Init(CalParam_t *params) {
  if (params == NULL) {
    return;
  }
  params->offset = 0.0f;
  params->slope = 1.0f;
}

uint8_t Cal_CalcParams(CalParam_t *params) {
  if (params->raw_l == NULL || params->raw_h == NULL || params->ideal_l == NULL || params->ideal_h == NULL) {
    return -1;
  }
  if (params == NULL || *(params->raw_l) == *(params->raw_h)) {
    // 检查输入是否有效，避免斜率计算中除以零
    return -1;
  }

  // 计算斜率 (slope)
  params->slope = (float)(*(params->ideal_h) - *(params->ideal_l)) / (*(params->raw_h) - *(params->raw_l));
  // 计算偏移量 (offset)
  params->offset = (float)(*(params->ideal_l)) - params->slope * (*(params->raw_l));
  return 0;  // 返回成功
}

int32_t Cal_Apply(const CalParam_t *params, int32_t raw) {
  if (params == NULL) {
    return raw;  // 如果参数无效，返回原始值
  }

  // 应用校正公式: corrected = slope * raw + offset
  return roundf(params->slope * raw + params->offset);
}
