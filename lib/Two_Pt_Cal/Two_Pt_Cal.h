#ifndef Cal_H
#define Cal_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

// 校正数据结构
typedef struct {
  float offset;  // 偏移量
  float slope;   // 斜率
  uint32_t *raw_l;
  uint32_t *raw_h;
  uint32_t *ideal_l;
  uint32_t *ideal_h;
} CalParam_t;

/**
 * @brief 初始化校正参数为默认值
 *
 * @param params 指向 CalParam_t 的指针
 */
void Cal_Init(CalParam_t *params);

/**
 * @brief 根据两点计算校正参数 (offset 和 slope)
 *
 * @param raw1 第一点的原始值
 * @param ideal1 第一点的理想值
 * @param raw2 第二点的原始值
 * @param ideal2 第二点的理想值
 * @param params 指向存储校正参数的结构体
 * @return int 返回 0 表示成功，-1 表示输入无效（如 raw1 == raw2 导致斜率为无穷大）
 */
uint8_t Cal_CalcParams(CalParam_t *params);

/**
 * @brief 使用校正参数计算校正后的值
 *
 * @param raw 原始输入值
 * @param params 指向校正参数的结构体
 * @return 校正后的值
 */
int32_t Cal_Apply(const CalParam_t *params, int32_t raw);

#ifdef __cplusplus
}
#endif

#endif  // Cal_H
