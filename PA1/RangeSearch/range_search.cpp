#include <cstdio> // 使用 C 风格的输入输出

// 二分查找函数，返回在 sorted_nums 中小于等于 _elem 的最大索引
int BinSearch(const int *sorted_nums, int lo, int hi, int _elem) {
    while (lo < hi) {
        int mi = (lo + hi) / 2;
        if (sorted_nums[mi] > _elem)
            hi = mi;
        else
            lo = mi + 1;
    }
    return lo - 1;
}

// 统计在 [left, right] 范围内的元素数量
int range_points(const int *sorted_nums, int size, int left, int right) {
    int nl = BinSearch(sorted_nums, 0, size, left - 1);
    int nh = BinSearch(sorted_nums, 0, size, right);
    return nh - nl;
}

// 交换两个整数
void swap(int &A, int &B) {
    int temp = A;
    A = B;
    B = temp;
}

// 分区函数，将小于等于 pivot 的元素移到左边，返回 pivot 的新位置
int partition(int *nums, int lo, int hi) {
    int pivot = nums[hi - 1];
    int index = lo;
    for (int current = lo; current < hi - 1; ++current) {
        if (nums[current] <= pivot) {
            swap(nums[index++], nums[current]);
        }
    }
    swap(nums[index], nums[hi - 1]);
    return index;
}

// 快速排序函数
void quick_sort(int *nums, int start, int end) {
    if (start < end - 1) {
        int pivot = partition(nums, start, end);
        quick_sort(nums, start, pivot);
        quick_sort(nums, pivot + 1, end);
    }
}

int main() {
    int n, m;
    scanf("%d %d", &n, &m); // 输入元素个数和查询区间数

    // 动态分配数组
    int *nums = new int[n];
    int *section_left = new int[m];
    int *section_right = new int[m];

    // 输入元素数组
    for (int i = 0; i < n; i++)
        scanf("%d", &nums[i]);

    // 输入查询区间的左右边界
    for (int j = 0; j < m; j++)
        scanf("%d %d", &section_left[j], &section_right[j]);

    // 对 nums 数组进行排序
    quick_sort(nums, 0, n);

    // 对每个查询区间输出元素数量
    for (int k = 0; k < m; k++) {
        printf("%d\n", range_points(nums, n, section_left[k], section_right[k]));
    }

    // 释放动态分配的内存
    delete[] nums;
    delete[] section_left;
    delete[] section_right;

    return 0;
}
