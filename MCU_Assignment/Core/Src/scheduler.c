#include "scheduler.h"

static HeapTask heap[SCH_MAX_TASKS];
static int heap_size = 0;
static uint32_t current_tick = 0;
static uint32_t next_task_id = 1;

static void swap(int i, int j) {
    HeapTask tmp = heap[i];
    heap[i] = heap[j];
    heap[j] = tmp;
}

static void heapify_up(int idx) {
    while (idx > 0) {
        int parent = (idx - 1) / 2;
        if (heap[idx].expire_time >= heap[parent].expire_time) break;
        swap(idx, parent);
        idx = parent;
    }
}

static void heapify_down(int idx) {
    while (1) {
        int left = 2 * idx + 1;
        int right = 2 * idx + 2;
        int smallest = idx;
        if (left < heap_size && heap[left].expire_time < heap[smallest].expire_time)
            smallest = left;
        if (right < heap_size && heap[right].expire_time < heap[smallest].expire_time)
            smallest = right;
        if (smallest == idx) break;
        swap(idx, smallest);
        idx = smallest;
    }
}

void SCH_Init(void) {
    DISABLE_INTERRUPTS();
    memset(heap, 0, sizeof(heap));
    heap_size = 0;
    current_tick = 0;
    next_task_id = 1;
    ENABLE_INTERRUPTS();
}

uint32_t SCH_Add_Task(void (*pFunction)(void), uint32_t DELAY, uint32_t PERIOD) {
    if (heap_size >= SCH_MAX_TASKS) return 0xFFFFFFFF;

    DISABLE_INTERRUPTS();
    uint32_t task_id = next_task_id++;

    heap[heap_size].pTask = pFunction;
    heap[heap_size].expire_time = current_tick + DELAY;
    heap[heap_size].period = PERIOD;
    heap[heap_size].taskID = task_id;

    heapify_up(heap_size);
    heap_size++;

    ENABLE_INTERRUPTS();
    return task_id;
}

void SCH_Update(void) {
    current_tick++;
}

void SCH_Dispatch_Tasks(void) {
    while (heap_size > 0 && heap[0].expire_time <= current_tick) {
        HeapTask task = heap[0];

        heap[0] = heap[heap_size - 1];
        heap_size--;
        heapify_down(0);

        if (task.pTask) {
            task.pTask();
        }

        if (task.period > 0) {
            heap[heap_size].pTask = task.pTask;
            heap[heap_size].expire_time = current_tick + task.period;
            heap[heap_size].period = task.period;
            heap[heap_size].taskID = task.taskID;
            heapify_up(heap_size);
            heap_size++;
        }
    }
}

uint8_t SCH_Delete_Task(uint32_t taskID) {
    if (taskID == 0) return 0;

    DISABLE_INTERRUPTS();

    int idx = -1;
    for (int i = 0; i < heap_size; i++) {
        if (heap[i].taskID == taskID) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        ENABLE_INTERRUPTS();
        return 0;
    }

    heap[idx] = heap[heap_size - 1];
    heap_size--;


    if (idx < heap_size) {
        uint32_t parent_time = (idx > 0) ? heap[(idx-1)/2].expire_time : 0;
        if (idx > 0 && heap[idx].expire_time < parent_time) {
            heapify_up(idx);
        } else {
            heapify_down(idx);
        }
    }

    ENABLE_INTERRUPTS();
    return 1;
}
