#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_TASK_STRING 10
#define MAX_PRIORITY (0xFFFFFFFFul)
#define MAX_N 4000010

typedef struct _TYPE_TASK {
    __uint64_t priority;
    char name[MAX_TASK_STRING];
} Task;

int n, m;
Task task_pool[MAX_N];
Task *heap[MAX_N];
int heap_size;

typedef __int64_t (*CompareFunc)(const Task *, const Task *);

CompareFunc heapCompare;

__int64_t compareBigger(const Task *a, const Task *b) {
    if (a->priority != b->priority)
        return a->priority - b->priority;
    else
        return strcmp(a->name, b->name);
}

__int64_t compareSmaller(const Task *a, const Task *b) {
    return compareBigger(b, a);
}

#define MAX(A, B, COMPFUNC) (COMPFUNC(heap[A], heap[B]) >= 0 ? A : B)
#define MIN(A, B, COMPFUNC) (COMPFUNC(heap[B], heap[A]) >= 0 ? A : B)

void percolateUp(int idx) {
    while (idx > 0) {
        int idp = (idx - 1) / 2;
        if (heapCompare(heap[idx], heap[idp]) > 0) {
            Task *tmp = heap[idx];
            heap[idx] = heap[idp];
            heap[idp] = tmp;
        }
        else break;
        idx = idp;
    }
}

void percolateDown(size_t idx) {
    int idc = 2 * idx + 1;//oj改错：这里索引有问题
    while (idc < heap_size) {
        if (idc < heap_size - 1)
            idc = MAX(idc, idc + 1, heapCompare);
        if (heapCompare(heap[idx], heap[idc]) >= 0) break;
        Task *tmp = heap[idc];
        heap[idc] = heap[idx];
        heap[idx] = tmp;
        idx = idc;
        idc = 2 * idx + 1;
    }
}

void buildHeap(CompareFunc compare) {
    heapCompare = compare ? compare : compareBigger;
    for (int i = 0; i < n; i++) 
        heap[i] = &task_pool[i];
    heap_size = n;

    for (int i = (heap_size - 2) / 2; i >= 0; i--)
        percolateDown(i);
}

Task* delHeap() {
    if (heap_size <= 0) {
        return NULL;
    }
    Task *topTask = heap[0];
    heap[0] = heap[heap_size - 1];
    heap_size--;
    percolateDown(0);
    return topTask;
}

void addToHeap(Task *task) {
    heap[heap_size++] = task;
    percolateUp(heap_size - 1);
}

int main() {
    scanf("%d %d", &n, &m);
    for (int i = 0; i < n; i++) {
        scanf("%lu %s", &task_pool[i].priority, task_pool[i].name);
    }
    if(n>1)
        buildHeap(compareSmaller);
    for (int i = 0; i < m; i++) {
        Task *task = delHeap();
        if(task){
            printf("%s\n", task->name);
            task->priority <<= 1;//oj改错： 先增加优先级再进堆
            if (task->priority <= MAX_PRIORITY) 
                addToHeap(task);
        }
        else break;
    }
    return 0;
}