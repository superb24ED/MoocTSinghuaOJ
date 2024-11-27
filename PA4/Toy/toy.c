#pragma warning(disable : 4996)
#include<stdio.h>
#include<stdint.h>
#include<stdbool.h>
#include<memory.h>

#define CUBIC_CAPACITY 10
#define MAX_CONBINETION 40320

char cubic[CUBIC_CAPACITY];
int steps[MAX_CONBINETION];
uint32_t queue[40320];
int rear = 0, front = 0, size = 1;
uint32_t target_state = 0x12345678;
const int w[8] = { 5040,720,120,24,6,2,1,0 };

//0x87654321 ->40319
int getIndex(uint32_t state) {
    unsigned char b[8] = { state >> 28 & 0xF,state >> 24 & 0xF,state >> 20 & 0xF,state >> 16 & 0xF,state >> 12 & 0xF,state >> 8 & 0xF,state >> 4 & 0xF,state & 0xF };
    int index = 0;
    int i = 0;
    for (int i = 0; i < 8; i++) {
        index += (b[i] - 1) * w[i];
        for (int j = i + 1; j < 8; j++)
            if (b[j] > b[i]) b[j]--;
    }
    return index;
}
uint32_t stringToState(const char state_string[8]) {
    for (int j = 0; j < 8; j++) {
        if (state_string[j] < '0' || state_string[j]>'9')
            return 0xFFFFFFFF;
    }
    int i = 0, state = 0;
    while (i < 8) {
        state <<= 4;
        state |= (state_string[i] - 48);
        i++;
    }
    return state;
}
/*
1 2 3 4           4 1 2 3
8 7 6 5   ->   5 8 7 6
0x12345678-> 0x41235876
*/
uint32_t rightShift(uint32_t state) {
    uint16_t  higher = state >> 16;
    uint16_t lower = state & 0xFFFF;
    higher = higher >> 4 | higher << 12;
    lower = lower << 4 | lower >> 12;
    state = higher << 16 | lower;
    return state;
}

uint32_t LeftShift(uint32_t state) {
    uint16_t  higher = state >> 16;
    uint16_t lower = state & 0xFFFF;
    higher = higher << 4 | higher >> 12;
    lower = lower >> 4 | lower << 12;
    state = higher << 16 | lower;
    return state;
}

/*
1 2 3 4          1 7 2 4
8 7 6 5   ->   8 6 3 5
0x12345678 -> 0x17245368
*/
uint32_t centralRotate90(uint32_t state) {
    uint8_t h1 = state >> 24, h4 = state & 0xFF;
    uint8_t h2 = (state >> 16) & 0xFF, h3 = (state & 0xFFFF) >> 8;
    uint8_t tmp = h1;
    h1 = (h1 & 0xF0) | (h4 >> 4);
    h4 = (h4 & 0xF) | h3 << 4;
    h3 = (h3 & 0xF0) | (h2 >> 4);
    h2 = (h2 & 0xF) | tmp << 4;
    state = h1 << 24 | h2 << 16 | h3 << 8 | h4;
    return state;
}

uint32_t centralInvRotate90(uint32_t state) {
    uint8_t h1 = state >> 24, h4 = state & 0xFF;
    uint8_t h2 = (state >> 16) & 0xFF, h3 = (state & 0xFFFF) >> 8;
    uint8_t tmp = h1;
    h1 = (h1 & 0xF0) | (h2 >> 4);
    h2 = (h2 & 0xF) | (h3 << 4);
    h3 = (h3 & 0xF0) | (h4 >> 4);
    h4 = (h4 & 0xF) | tmp << 4;
    state = h1 << 24 | h2 << 16 | h3 << 8 | h4;
    return state;
}

/*
1 2 3 4          8 7 6 5
8 7 6 5   ->   1 2 3 4
0x12345678 -> 0x87654321
*/
uint32_t exchange(uint32_t state) {
    uint8_t h1 = state >> 24, h4 = state & 0xFF;
    uint8_t h2 = (state >> 16) & 0xFF, h3 = (state & 0xFFFF) >> 8;
    h4 = h4 >> 4 | h4 << 4; h3 = h3 >> 4 | h3 << 4;
    h2 = h2 >> 4 | h2 << 4; h1 = h1 >> 4 | h1 << 4;
    state = h4 << 24 | h3 << 16 | h2 << 8 | h1;
    return state;
}

void bfs() {
    rear = 0, front = 0, size = 1;
    memset(steps, 0xFF, sizeof(steps));//steps : array uint32_t[40320]
    queue[0] = target_state;
    steps[getIndex(target_state)] = 0;
    while (size > 0) {
        uint32_t state = queue[rear];
        int current_step = steps[getIndex(state)];
        rear = (rear + 1) % MAX_CONBINETION; size--;//dequeue


        uint32_t new_state = LeftShift(state);
        int new_index = getIndex(new_state);
        if (-1 == steps[new_index]) {
            steps[new_index] = current_step + 1;
            front = (front + 1) % MAX_CONBINETION; size++;
            queue[front] = new_state;
        }

        new_state = exchange(state);
        new_index = getIndex(new_state);
        if (-1 == steps[new_index]) {
            steps[new_index] = current_step + 1;
            front = (front + 1) % MAX_CONBINETION; size++;
            queue[front] = new_state;
        }

        new_state = centralInvRotate90(state);
        new_index = getIndex(new_state);
        if (-1 == steps[new_index]) {
            steps[new_index] = current_step + 1;
            front = (front + 1) % MAX_CONBINETION; size++;
            queue[front] = new_state;
        }
    }
}

int main() {
    bfs();//计算所有可达位置
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf(" %c %c %c %c %c %c %c %c", cubic, cubic + 1, cubic + 2, cubic + 3, cubic + 4, cubic + 5, cubic + 6, cubic + 7);
        uint32_t state = stringToState(cubic);
        int index = getIndex(state);
        if (0xFFFFFFFF == state || index < 0 || index >= MAX_CONBINETION)
            printf("-1\n");
        else printf("%d\n", steps[index]);
    }
}