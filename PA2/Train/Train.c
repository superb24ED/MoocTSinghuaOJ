#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
/* train_dispatch.c */
typedef enum _Operaion{
    PUSH = 0,
    POP = 1
} Operation;
// 定义操作类型，用于记录栈操作的顺序
typedef struct BinTreeNode
{
    int _data;
    struct BinTreeNode *_left_child;
    struct BinTreeNode *_right_child;
} BinTreeNode;

/**
 * @brief 释放分配的内存
 *
 * @param stack 栈内存指针
 * @param operations 操作序列内存指针
 */
void freeMemory(int *stack, Operation *operations)
{
    if (stack) free(stack);
    if (operations) free(operations);
}

/**
 * @brief 尝试调度列车
 *
 * @param targetSeq 目标列车顺序数组
 * @param n 目标序列长度
 * @param m 栈的最大容量
 * @param operations 操作序列输出指针（需要手动 free）
 * @param operationCount 输出操作序列的长度
 * @return true 如果可以生成目标序列
 * @return false 如果无法生成目标序列
 */
bool tryDispatch(const int targetSeq[], int n, int m, Operation **operations, int *operationCount)
{
    // 动态分配栈和操作序列的内存
    int *stack = (int *)malloc(m * sizeof(int));
    if (!stack) return false;

    *operations = (Operation *)malloc(2 * n * sizeof(Operation)); // 每个元素最多有 PUSH 和 POP 两种操作
    if (!*operations)
    {
        free(stack);
        return false;
    }

    int top = -1;  // -1 表示栈为空
    *operationCount = 0;  // 初始化输出操作序列长度
    int in_train = 1;

    // 遍历目标序列
    for (int i = 0; i < n; i++)
    {
        // 将 in_train 入栈直到等于目标元素 targetSeq[i]
        while (in_train <= targetSeq[i])
        {
            if (top < m - 1)
            { // 栈未满，继续入栈
                stack[++top] = in_train++;
                (*operations)[(*operationCount)++] = PUSH;
            }
            else
            { // 栈满，无法入栈
                freeMemory(stack, *operations);
                *operations = NULL;
                return false;
            }
        }

        // 栈顶元素是否等于当前目标序列元素
        if (top > -1 && stack[top] == targetSeq[i])
        {
            top--; // 出栈
            (*operations)[(*operationCount)++] = POP;
        }
        else
        {
            freeMemory(stack, *operations);
            *operations = NULL;
            return false;
        }
    }

    free(stack); // 释放栈内存
    return true;
}

int main()
{
    int n, m;
    // 输入目标序列长度 n 和栈容量 m
    scanf("%d %d", &n, &m);

    // 动态分配目标序列数组
    int *a = (int *)malloc(n * sizeof(int));
    if (!a)
    {
        printf("Memory allocation failed\n");
        return 1;
    }

    // 读取目标序列
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &a[i]);
    }

    Operation *operations = NULL;
    int operationCount = 0;

    // 尝试调度列车
    if (tryDispatch(a, n, m, &operations, &operationCount))
    {
        // 输出操作序列
        for (int i = 0; i < operationCount; i++)
        {
            printf(operations[i] == PUSH ? "push\n" : "pop\n");
        }
        free(operations); // 操作成功，手动释放操作序列内存
    }
    else
    {
        printf("No\n");
    }

    free(a); // 释放目标序列内存
    return 0;
}
