
#include "scheduler.h"
#define WHEEL_SIZE 1000


typedef struct Node {
    uint8_t id;         
    struct Node* next; 
} Node;

sTask SCH_tasks[SCH_MAX_TASKS];
volatile uint32_t tick_ms = 0;

static Node node_pool[SCH_MAX_TASKS];
static Node* wheel[WHEEL_SIZE];
static uint16_t current_slot = 0;


static void wheel_insert(uint16_t slot, Node* n) {

    n->next = wheel[slot];
    wheel[slot] = n;
}


void SCH_Init(void)
{
    for (int i = 0; i < SCH_MAX_TASKS; i++) {
        SCH_tasks[i].pTask  = 0;
        SCH_tasks[i].Delay  = 0;
        SCH_tasks[i].Period = 0;
        SCH_tasks[i].RunMe  = 0;

        node_pool[i].next = 0;
    }

    for (int i = 0; i < WHEEL_SIZE; i++) {
        wheel[i] = 0;
    }

    current_slot = 0;
    tick_ms = 0;
}

void SCH_Update(void)
{
    tick_ms += 10;

    Node* n = wheel[current_slot];
    wheel[current_slot] = 0; 

    while (n != 0) {

        Node* next_node = n->next;

        uint8_t id = n->id;
        sTask* t = &SCH_tasks[id];

        if (t->pTask != 0) {
            t->RunMe++; 

            if (t->Period > 0) {
                uint16_t next_slot = (current_slot + t->Period) % WHEEL_SIZE;
                wheel_insert(next_slot, n);
            } else {
                n->next = 0;
            }
        }

        n = next_node;  
    }

    current_slot++;
    if (current_slot >= WHEEL_SIZE) current_slot = 0;
}

void SCH_Dispatch_Tasks(void)
{
    for (int i = 0; i < SCH_MAX_TASKS; i++) {
        if (SCH_tasks[i].RunMe > 0 && SCH_tasks[i].pTask != 0) {

            (*SCH_tasks[i].pTask)();
            SCH_tasks[i].RunMe--;

            if (SCH_tasks[i].Period == 0) {
                SCH_Delete_Task(i);
            }
        }
    }
}


uint8_t SCH_Add_Task(TaskFunction_t pFunction,
                     uint32_t DELAY,
                     uint32_t PERIOD)
{
    for (int i = 0; i < SCH_MAX_TASKS; i++) {

        if (SCH_tasks[i].pTask == 0) {
            SCH_tasks[i].pTask  = pFunction;
            SCH_tasks[i].Delay  = DELAY;
            SCH_tasks[i].Period = PERIOD;
            SCH_tasks[i].RunMe  = 0;

            node_pool[i].id = i;
            node_pool[i].next = 0;

            uint16_t slot = (current_slot + DELAY) % WHEEL_SIZE;
            wheel_insert(slot, &node_pool[i]);

            return i;
        }
    }
    return NO_TASK_ID;
}

uint8_t SCH_Delete_Task(uint8_t id)
{
    if (id >= SCH_MAX_TASKS) return 1;

    SCH_tasks[id].pTask  = 0;
    SCH_tasks[id].Delay  = 0;
    SCH_tasks[id].Period = 0;
    SCH_tasks[id].RunMe  = 0;

    return 0;
}
