#ifndef MCP342X_WRAP_H
#define MCP342X_WRAP_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  MCP342x_CHANNEL_1 = 0,
  MCP342x_CHANNEL_2,
  MCP342x_CHANNEL_3,
  MCP342x_CHANNEL_4
} MCP342x_channel_t;

typedef enum {
  errorNone,
  errorConvertFailed,
  errorReadFailed,
  errorReadTimeout,
  errorConversionNotReady,
  errorConfigureFailed,
} MCP342x_error_t;

uint8_t MCP342x_generalCallReset();

MCP342x_error_t MCP342x_convertAndRead(MCP342x_channel_t channel, long *result);

#ifdef __cplusplus
}
#endif

#endif  // MCP342X_H