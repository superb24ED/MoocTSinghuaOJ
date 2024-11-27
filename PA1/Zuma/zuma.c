#pragma warning(disable: 4996)
#include <stdio.h>
#include <string.h>

#define N 10005
char zuma_buf[N * 2];
char opt_char[N];
int opt_index[N];
int zuma_bufsz;

// 更新函数：对局部进行更新
void update(int front) {
    int back = front;
    char pivot = zuma_buf[front];
    int update_flag = 1;
    if (front == -1 || zuma_bufsz < 3)
        return;
    
    while (update_flag)
    {
        update_flag = 0;
        while (--front >= 0 && zuma_buf[front] == pivot);
        while (++back < zuma_bufsz && zuma_buf[back] == pivot);
        int count = back - front - 1;
        if (count >= 3) {
        for (int i = 0; i + back < zuma_bufsz; i++) {
            zuma_buf[front + 1 + i] = zuma_buf[back + i];
        }
        zuma_bufsz -= count;
        zuma_buf[zuma_bufsz] = 0;
        back = front;
        update_flag = 1;
        pivot = zuma_buf[front];

        }
    }
}
// 插入函数：在指定位置插入字符
void insert(int index, char elem) {
    if (index >= 0 && index <= zuma_bufsz) {
        for (int i = zuma_bufsz; i > index; i--) {
            zuma_buf[i] = zuma_buf[i - 1];
        }
        zuma_buf[index] = elem;
        zuma_bufsz++;
        zuma_buf[zuma_bufsz] = '\0';
    }
}


// 主函数
int main() {
    int n = 0;
    //这里使用scanf，那么第二个测例无法通过
    //因为第二个测例的第一行是空的.
    //这是scanf读取字符串不安全导致的
    fgets(zuma_buf, N, stdin);
    zuma_buf[strcspn(zuma_buf, "\n")] = 0; // 去除换行符
    //在c++中，有更好的字符串读取函数，避免连续读取字符串变量的时候，后续变量读取错误的问题
    //std::cin.getline(zuma_buf,N);
    zuma_bufsz = strlen(zuma_buf);

    // 输入插入操作的次数
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%d %c", &opt_index[i], &opt_char[i]);
    }
    // 执行每个插入和更新操作
    for (int j = 0; j < n; j++) {
        insert(opt_index[j], opt_char[j]);
        update(opt_index[j]);
        if (zuma_bufsz)
            printf("%s\n", zuma_buf);
        else printf("-\n");
    }

    return 0;
}
