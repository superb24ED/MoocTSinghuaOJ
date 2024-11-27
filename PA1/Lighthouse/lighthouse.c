#pragma warning(disable : 4996)
#include <stdio.h>
#include <malloc.h>
/*
lighthouse.c
get ordered point pairs
*/
typedef struct _Point
{
    long long int posX;
    long long int posY;
} Point;
#define MAX_N 4000010
int total_points = 0;
Point points[MAX_N];
Point tmp[MAX_N / 2];

void swap(long long int* A, long long int* B)
{
    long long int temp = *A;
    *A = *B;
    *B = temp;
}
// 快速排序函数
void quick_sort_byX(int start, int end)
{
    if (start < end - 1)
    {
        long long int pivot = points[end - 1].posX;
        int index = start;
        for (int current = start; current < end - 1; ++current)
        {
            if (points[current].posX <= pivot)
            {
                swap(&points[index].posX, &points[current].posX);
                swap(&points[index].posY, &points[current].posY);
                index++;
            }
        }
        swap(&points[index].posX, &points[end - 1].posX);
        swap(&points[index].posY, &points[end - 1].posY);
        quick_sort_byX(start, index);
        quick_sort_byX(index + 1, end);
    }
}

long long int merge(int start, int mi, int end)
{
    long long int count = 0;
    int left_size = mi - start;

    for (int i = 0; i < left_size; i++)
    {
        tmp[i].posX = points[start + i].posX;
        tmp[i].posY = points[start + i].posY;
    }
    int left = 0, right = mi;
    int index = start;

    while (left < left_size || right < end)
    {
        if (left >= left_size)
        {
            points[index].posX = points[right].posX;
            points[index].posY = points[right].posY;
            index++;
            right++;
        }
        else if (right >= end)
        {
            points[index].posX = tmp[left].posX;
            points[index].posY = tmp[left].posY;
            index++;
            left++;

        }
        else if (points[right].posY < tmp[left].posY)
        {
            points[index].posX = points[right].posX;
            points[index].posY = points[right].posY;
            index++;
            right++;
        }
        else
        {
            points[index].posX = tmp[left].posX;
            points[index].posY = tmp[left].posY;
            index++;
            left++;
            count += end - right;
        }
    }
    return count;
}

long long int merge_byY(int start, int end)
{
    int mi = (start + end) / 2;
    long long  int count = 0;
    if (end - start <= 1)
        return 0;
    else if (start - end == 2) {
        if (points[start].posY > points[end - 1].posY) {
            swap(&points[start].posY, &points[end - 1].posY);
            swap(&points[start].posX, &points[end - 1].posX);
        }
        else {
            count++;
        }
        return count;
    }
    count += merge_byY(start, mi);
    count += merge_byY(mi, end);
    count += merge(start, mi, end);
    return count;
}

int main()
{
    scanf("%d", &total_points);
    //Point* points = (Point*)malloc((total_points + 5) * sizeof(Point));
    for (int i = 0; i < total_points; i++)
        scanf("%lld %lld", &points[i].posX, &points[i].posY);
    quick_sort_byX(0, total_points);
    printf("%lld\n", merge_byY(0, total_points));

    return 0;
}
