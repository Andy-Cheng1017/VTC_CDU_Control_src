#ifndef MAIN_H
#define MAIN_H

#include "semphr.h"

#define RS485_SEMAPHORE_TIMEOUT 250

// typedef enum {
//   ONLY_WARNING,
//   WARNING_AND_STOP,
// } AlarmAction_t;

extern SemaphoreHandle_t RS485RegionMutex;

#endif  // !MAIN_H