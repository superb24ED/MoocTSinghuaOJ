#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

//kd oj 75/100

//#define _OJ_DEBUG_

#ifdef _OJ_DEBUG_
#include "temperature_lib.c"
#else
#include "temperature.h"
int n;
#endif
#define MAX_N 50010
#define DIM 2


typedef struct kdTreeNode
{                   
    int x, y, temp;                 
    struct kdTreeNode *left_child; 
    struct kdTreeNode *right_child;  
} kdTreeNode;

kdTreeNode nodePool[MAX_N]; 
kdTreeNode * _root = NULL;           
void swap(kdTreeNode *a, kdTreeNode *b)
{
    if (a == b) return;  // 如果是同一元素，则不交换
    kdTreeNode tmp = *a;
    *a = *b;
    *b = tmp;
}

int compare(const kdTreeNode *a, const kdTreeNode *b,int depth)
{
    if (depth % DIM == 0)  
        return a->x - b->x;
    else          
        return a->y - b->y;
}

int quickSelect(int start, int end, int target, int depth)
{
    if (start >= end || target < start || target >= end) return -1;

    while (start < end)
    {
        if (end - start < 3){
            if(compare(&nodePool[start], &nodePool[end - 1], depth) > 0){
                swap(&nodePool[start], &nodePool[end - 1]);
            }
            return start;
        }

        int p = rand() % (end - start - 2) + start + 1;

        //选取3个数中的 中间作为pivot: start,p,end-1
        // 最后一个元素end-1是pivot，保证它的大小在三者中间
        if (compare(&nodePool[start], &nodePool[end - 1], depth) > 0)
            swap(&nodePool[start], &nodePool[end - 1]);
        if (compare(&nodePool[start], &nodePool[p], depth) > 0)
            swap(&nodePool[start], &nodePool[p]);
        if (compare(&nodePool[p], &nodePool[end - 1], depth) < 0)
            swap(&nodePool[end - 1], &nodePool[p]);

        int pivot = end - 1;
        int lp = start - 1, rp = start - 1;
        while (++rp < end - 1)
            if (compare(&nodePool[rp], &nodePool[pivot], depth) <= 0)
                swap(&nodePool[++lp], &nodePool[rp]);
        swap(&nodePool[++lp], &nodePool[end - 1]);

        if (target == lp)
            return target;
        else
        {
            if (target > lp)
                start = lp + 1;
            else
                end = lp;
        }
    }
    return -1;
}

typedef struct StackFrame{
    int lo,hi,depth;
    kdTreeNode **proot;
}StackFrame;

void buildKdTree()
{
    int lo = 0, hi = n;
    int depth = 0;

    if (hi - lo < 1) return; 

    StackFrame *stack = (StackFrame *)malloc(2 * n * sizeof(StackFrame));  // 分配栈空间
    
    int top = -1;
    
    int mi = quickSelect(lo, hi, (lo + hi) / 2, 0);
    if (mi == -1) return;

    _root = nodePool + mi;

    stack[++top] = (StackFrame){mi + 1, hi, depth + 1, &_root->right_child};
    stack[++top] = (StackFrame){lo, mi, depth + 1, &_root->left_child};

    kdTreeNode **parent;
    while (top >= 0)
    {
        StackFrame *frame = &stack[top];
        lo = frame->lo;
        hi = frame->hi;
        depth = frame->depth;
        parent = frame->proot;

        mi = quickSelect(lo, hi, (lo + hi) / 2, depth % 2);

         top--;  
        if (mi == -1)
        {
            continue;
        }
        *parent = nodePool +  mi;
        //(*parent)->depth = depth;


        // 将左右子树的构建任务压栈
        if (mi + 1 < hi)
            stack[++top] = (StackFrame){mi + 1, hi, depth + 1, &(*parent)->right_child};
        if (lo < mi)
            stack[++top] = (StackFrame){lo, mi, depth + 1, &(*parent)->left_child};
    }

    free(stack);  // 释放栈空间
}


// 查询区域内温度平均值的递归函数
int64_t getAreaAverageRecursive(kdTreeNode *node, int x1, int y1, int x2, int y2, int depth, int *count)
{
    if (!node)
        return 0;

    int cx = node->x, cy = node->y;

    int64_t sum = 0;

    // 检查当前站点是否在区域内
    if (x1 <= cx && cx <= x2 && y1 <= cy && cy <= y2)
    {
        sum += node->temp;
        (*count)++;
    }

    // 剪枝优化：如果当前子树不可能与查询区域重叠，则无需遍历子树
    if ((depth % 2 == 0 && cx >= x1) || (depth % 2 == 1 && cy >= y1))
    {
        sum += getAreaAverageRecursive(node->left_child, x1, y1, x2, y2, depth + 1, count);
    }
    if ((depth % 2 == 0 && cx <= x2) || (depth % 2 == 1 && cy <= y2))
    {
        sum += getAreaAverageRecursive(node->right_child, x1, y1, x2, y2, depth + 1, count);
    }

    return sum;
}

// 查询区域内的温度平均值
int64_t getAreaAverage(int x1, int y1, int x2, int y2)
{
    int count = 0;
    int64_t sum = getAreaAverageRecursive(_root, x1, y1, x2, y2, 0, &count);
    return count > 0 ? sum / count : 0;
}

// 释放 kd 树的内存
void freeKdTree(kdTreeNode *node)
{
    if (!node)
        return;
    freeKdTree(node->left_child);
    freeKdTree(node->right_child);
    free(node);
}

int main()
{
    srand(time(0));
    int x1, x2, y1, y2, temp;
    n = GetNumOfStation();

    for (int no = 0; no < n; no++)
    {
        GetStationInfo(no, &x1, &y1, &temp);
        nodePool[no].x = x1;
        nodePool[no].y = y1;
        nodePool[no].temp = temp;
    }

    buildKdTree();

    while (GetQuery(&x1, &y1, &x2, &y2))
    {
        if(0==n) Response(0);
        else Response(getAreaAverage(x1, y1, x2, y2));
    }

    return 0;
}
