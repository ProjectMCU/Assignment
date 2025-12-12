

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

#include <stdint.h>

#define SCH_MAX_TASKS   10
#define NO_TASK_ID      255

typedef void (*TaskFunction_t)(void);

typedef struct {
    TaskFunction_t pTask;   // con trỏ hàm
    uint32_t       Delay;   // Delay (ticks)
    uint32_t       Period;  // Period (ticks)
    uint8_t        RunMe;   // Task ready count
} sTask;

/* task array & tick counter được khai báo trong scheduler.c */
extern sTask SCH_tasks[SCH_MAX_TASKS];
extern volatile uint32_t tick_ms;

/* API của scheduler */
void SCH_Init(void);
void SCH_Update(void);
void SCH_Dispatch_Tasks(void);
void Task_Display_Update(void);

uint8_t SCH_Add_Task(TaskFunction_t pFunction, uint32_t DELAY, uint32_t PERIOD);
uint8_t SCH_Delete_Task(uint8_t id);

#endif /* INC_SCHEDULER_H_ */
