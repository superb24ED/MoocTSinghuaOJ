#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STRING 10010

char s1[MAX_STRING];
char s11[2 * MAX_STRING];
char s2[MAX_STRING];
int len1, len2;

int main() {
    while(scanf("%s %s", s1, s2) != EOF) {  // 使用EOF来控制循环终止
        len1 = strlen(s1);
        len2 = strlen(s2);
        
        // 如果长度不同，直接输出 NO
        if (len1 != len2) {
            printf("NO\n");
            continue;
        }

        // 构造 s1 + s1
        strcpy(s11, s1);
        strcat(s11, s1);

        // 使用 strstr 查找 s2 是否是 s1+s1 的子串
        if (strstr(s11, s2)) {
            printf("YES\n");
        } else {
            printf("NO\n");
        }
    }
    return 0;
}
