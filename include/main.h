#ifndef MAIN_H
#define MAIN_H

#include "semphr.h"

#define RS485_SEMAPHORE_TIMEOUT 250

extern SemaphoreHandle_t RS485RegionMutex; 

#endif // !MAIN_H