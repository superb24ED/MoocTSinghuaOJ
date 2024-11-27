#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>


// 定义发射器类型
typedef enum TYPE_TRANSMITTER_FM
{
    FM104_2 = 1,  // FM104.2 频率
    FM98_6 = 2,   // FM98.6 频率
    UNSET = 0     // 发射器未设置
} FM;

// 邻接表的节点结构，表示一个城镇与另一个城镇的连接关系
typedef struct EdgeList
{
    int _index;          // 邻接城镇的索引
    struct EdgeList *_next; // 指向下一个邻接的城镇
} EdgeList;

// 城镇的结构体，包含邻接城镇和发射器类型
typedef struct Town
{
    //如果他俩换位置，oj会报Runtime Error
    //在换位置的情况下，towns的长度申请为2*n，又不会报这个错误
    //应该是内存对齐问题
    EdgeList *adjTown;  // 邻接城镇列表
    FM _fm;             // 发射器类型
} Town;

// 另一个相似的结构体
// 设置结构体对齐方式为1字节
//#pragma pack(push, 1)
typedef struct Town2
{
    //加上#pragma pack(push, 1)后这个也不会报错了，确定是内存对齐的问题
    FM _fm;             // 发射器类型
    EdgeList *adjTown;  // 邻接城镇列表
} Town2;

// 恢复默认的对齐方式
//#pragma pack(pop)

// 广度优先搜索（BFS）函数，用于尝试部署发射器
bool tryDeployTransmitterBFS(Town *towns, int n)
{
    // 创建队列用于 BFS
    int queue_size = n;
    int *queue = (int *)malloc(queue_size * sizeof(int)); // 队列动态分配内存

    int rear = -1, front = -1; // 队列的尾部和头部指针
    int size = 0; // 当前队列中元素的数量

    // 遍历每个城镇，尝试为未设置发射器的城镇分配发射器
    for (int i = 0; i < n; i++)
    {
        if (towns[i]._fm != UNSET)  // 如果该城镇已有发射器，跳过
            continue;

        towns[i]._fm = FM104_2;  // 给该城镇设置发射器 FM104.2
        front = (front + 1) % queue_size;  // 环形队列处理
        queue[front] = i;  // 将该城镇加入队列
        size++;

        // BFS 处理队列中的每个城镇
        while (size > 0)
        {
            rear = (rear + 1) % queue_size; // 出队
            int index = queue[rear];
            size--;

            // 确定当前城镇相邻城镇应设置的发射器类型
            FM nextFM = (towns[index]._fm == FM104_2) ? FM98_6 : FM104_2;

            // 遍历当前城镇的邻接城镇
            EdgeList *p = towns[index].adjTown;
            while (p)
            {
                // 如果相邻城镇未设置发射器
                if (towns[p->_index]._fm == UNSET)
                {
                    towns[p->_index]._fm = nextFM;  // 给相邻城镇分配不同的发射器
                    front = (front + 1) % queue_size;  // 环形队列处理
                    queue[front] = p->_index;  // 将相邻城镇加入队列
                    size++;
                }
                else if (towns[p->_index]._fm == towns[index]._fm)
                {
                    // 如果相邻城镇的发射器与当前城镇相同，说明发生冲突
                    free(queue);  // 释放队列内存
                    return false; // 返回 false，表示无法二分图
                }
                p = p->_next; // 继续处理下一个邻接城镇
            }
        }
    }

    free(queue);  // 释放队列内存
    return true;   // 如果没有冲突，返回 true
}

//深度优先搜索（DFS）也可以解决同样的问题，这里是DFS的一个递归实现
bool tryDeployTransmitterDFS(Town *towns, int index, FM currentFM) {
    if (towns[index]._fm != UNSET) {
        return towns[index]._fm == currentFM;  // 已经设置发射器类型，检查是否一致
    }

    // 为当前城镇设置发射器类型
    towns[index]._fm = currentFM;

    // 遍历相邻的城镇，递归进行发射器部署
    EdgeList *p = towns[index].adjTown;
    while (p) {
        if (!tryDeployTransmitterDFS(towns, p->_index, (currentFM == FM104_2) ? FM98_6 : FM104_2)) {
            return false;  // 如果某个相邻城镇部署失败，返回 false
        }
        p = p->_next;
    }
    return true;
}

// 释放邻接表内存
void freeEdgeList(EdgeList *list)
{
    EdgeList *tmp = list;
    while (list)
    {
        tmp = list->_next;
        free(list);  // 释放当前节点内存
        list = tmp;  // 处理下一个节点
    }
}

int main()
{

    int n, m;
    scanf("%d %d", &n, &m);  // 输入城镇数 n 和道路数 m

    // 动态分配城镇数组内存
    Town *towns = (Town *)malloc(n * sizeof(Town));
    for (int i = 0; i < n; i++)
    {
        towns[i]._fm = UNSET;  // 初始化城镇的发射器类型为 UNSET
        towns[i].adjTown = NULL; // 初始化城镇的邻接表为空
    }

    int x, y;
    // 输入每条道路的连接关系，并更新邻接表
    for (int i = 0; i < m; i++)
    {
        scanf("%d %d", &x, &y);

        // 将 y 添加到 x 的邻接表
        EdgeList *p = (EdgeList *)malloc(sizeof(EdgeList));
        p->_index = y;
        p->_next = towns[x].adjTown;
        towns[x].adjTown = p;

        // 将 x 添加到 y 的邻接表
        p = (EdgeList *)malloc(sizeof(EdgeList));
        p->_index = x;
        p->_next = towns[y].adjTown;
        towns[y].adjTown = p;
    }
    // 检查是否能成功部署发射器
    printf(tryDeployTransmitterBFS(towns, n) ? "1\n" : "-1\n");
    /*
    // 尝试从每个未设置发射器的城镇开始部署
    bool canDeploy = true;
    for (int i = 0; i < n; i++) {
        if (towns[i]._fm == UNSET) {
            if (!tryDeployTransmitterDFS(towns, i, FM104_2)) {
                canDeploy = false;
                break;
            }
        }
    }
     printf(canDeploy ? "1\n" : "-1\n");
    */
    // 释放动态分配的邻接表内存
    for (int i = 0; i < n; i++)
        freeEdgeList(towns[i].adjTown);
    // 释放城镇数组内存
    free(towns);
    return 0;
}
