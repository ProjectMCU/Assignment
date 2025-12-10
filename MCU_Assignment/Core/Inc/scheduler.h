/*
 * scheduler.h
 *
 *  Created on: Dec 8, 2025
 *      Author: ADMIN
 */

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

#include <stdint.h>
#include "stm32f1xx_hal.h"
#include <stddef.h>
#include <string.h>

#define SCH_MAX_TASKS	10
#define DISABLE_INTERRUPTS()  __disable_irq()
#define ENABLE_INTERRUPTS()   __enable_irq()

typedef struct {
    void (*pTask)(void);
    uint32_t expire_time;
    uint32_t period;
    uint32_t taskID;
    uint8_t RunMe;
} HeapTask;



void SCH_Init(void);
uint32_t SCH_Add_Task(void(*pFunction)(void),
						uint32_t DELAY,
						uint32_t PERIOD);
uint8_t SCH_Delete_Task(uint32_t taskID);
void SCH_Update(void);
void SCH_Dispatch_Tasks(void);

#endif /* INC_SCHEDULER_H_ */

