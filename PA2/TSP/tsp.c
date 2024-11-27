#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<memory.h>
#include<stdbool.h>
#include<time.h>

// 宏定义，返回两个值中较大的一个
#define MAX(a, b)  ( (a) > (b) ? (a) : (b) )
/*
// 状态枚举类型，表示节点的状态
typedef enum STATUS{
    UNDISCOVERED = 0,  // 未发现
    DISCOVERED = 1,    // 已发现
    VISITED = 2        // 已访问
}STATUS;
*/

// 链表节点定义，存储邻接节点的数据
typedef struct EdgeList{
    int _data;               // 存储邻接节点的编号
    struct EdgeList *next;   // 指向下一个邻接节点
}EdgeList;

// 村庄结构体，包含指向所有出路的链表头指针
typedef struct Villege{
    //STATUS _status;
    //int _d_time;
    //int _f_time;
    //int _data;
    EdgeList *_out_roads;   // 指向出路链表的头指针
}Villege;

// 递归方式计算每个节点的最长路径
int maxTravNodesDFSRe(Villege* villeges, int *max_nodes_list, int node){
   // 如果该节点的最长路径已经计算过，则直接返回
   if(max_nodes_list[node] != -1){
        return max_nodes_list[node];
   }

    int max_nodes = 1;  // 默认当前节点的路径长度为 1

    EdgeList *p = villeges[node]._out_roads;  // 获取当前节点的所有出路
    while (p)  // 遍历所有出路
    {
        int next_node =  p->_data;  // 获取下一个节点
        // 递归调用计算下一个节点的最长路径
        max_nodes = MAX(max_nodes, 1 + maxTravNodesDFSRe(villeges, max_nodes_list, next_node));
        p = p->next;  // 移动到下一个邻接节点
    }

    max_nodes_list[node] = max_nodes;  // 记录当前节点的最长路径
    return max_nodes;
}

// 非递归方式计算每个节点的最长路径，使用栈模拟递归
int maxTravNodesDFSIt(Villege *villeges, int *max_nodes_list, int size){
    int max_nodes = 1;  // 默认最长路径为 1
    int *stack = (int *)malloc(size * sizeof(int));  // 模拟递归的栈
    bool *in_stack = (bool *)malloc(size * sizeof(bool));  // 跟踪节点是否在栈中，避免重复处理
    memset(in_stack, 0, size * sizeof(bool));  // 初始化栈标记

    for (int i = 0; i < size; i++) {
        if (max_nodes_list[i] != -1) {
            continue;  // 如果该节点的最长路径已知，跳过
        }

        int top = 0;
        stack[top++] = i;  // 将当前节点压入栈中
        in_stack[i] = true;

        while (top > 0) {
            int node = stack[top - 1];  // 栈顶节点
            EdgeList *p = villeges[node]._out_roads;
            bool all_children_visited = true;  // 标记所有子节点是否已访问
            int max_child_nodes = 0;  // 当前节点的最长子路径

            while (p) {
                int next_node = p->_data;
                if (max_nodes_list[next_node] == -1 && !in_stack[next_node]) {
                    // 如果子节点未访问，且不在栈中，将其压入栈
                    stack[top++] = next_node;
                    in_stack[next_node] = true;
                    all_children_visited = false;  // 子节点未完全访问
                    break;
                }
                max_child_nodes = MAX(max_child_nodes, max_nodes_list[next_node]);
                p = p->next;
            }

            if (all_children_visited) {
                // 如果所有子节点已访问，更新当前节点的最长路径
                max_nodes_list[node] = 1 + max_child_nodes;
                max_nodes = MAX(max_nodes, max_nodes_list[node]);  // 更新全局最长路径
                in_stack[node] = false;  // 从栈中弹出节点
                top--;  // 弹出栈顶节点
            }
        }
    }

    free(stack);  // 释放栈内存
    free(in_stack);  // 释放栈标记内存
    return max_nodes;  // 返回计算出的最长路径
}

// 使用 Kahn 算法计算图的最长路径
int maxTravNodesKahn(Villege *villeges, int *topo_order, int size){
    int *in_degree = (int *)malloc(size * sizeof(int));  // 存储每个节点的入度
    memset(in_degree, 0, size * sizeof(int));  // 初始化入度为 0

    // 计算每个节点的入度
    for (int i = 0; i < size; i++) {
        EdgeList *p = villeges[i]._out_roads;
        while (p) {
            in_degree[p->_data]++;  // 每个邻接节点的入度 +1
            p = p->next;
        }
    }

    int topo_index = 0;  // 拓扑排序的下标

    // 队列来存放入度为 0 的节点
    int *queue = (int *)malloc(size * sizeof(int));
    int queue_start = 0, queue_end = 0;

    // 将所有入度为 0 的节点加入队列
    for (int i = 0; i < size; i++) {
        if (in_degree[i] == 0) {
            queue[queue_end++] = i;
        }
    }

    // 拓扑排序过程
    while (queue_start < queue_end) {
        int node = queue[queue_start++];  // 取出队列中的节点
        topo_order[topo_index++] = node;  // 记录拓扑排序结果

        EdgeList *p = villeges[node]._out_roads;
        while (p) {
            int next_node = p->_data;
            in_degree[next_node]--;  // 减少邻接节点的入度
            if (in_degree[next_node] == 0) {
                queue[queue_end++] = next_node;  // 如果入度为 0，加入队列
            }
            p = p->next;
        }
    }

    // 如果拓扑排序后的节点数不等于图中的节点数，说明图中有环
    if (topo_index != size) {
        printf("Graph has a cycle\n");
        free(in_degree);
        free(queue);
        return -1;  // 返回 -1 表示有环
    }

    // 计算最长路径
    int *dist = (int *)malloc(size * sizeof(int));
    for (int i = 0; i < size; i++) {
        dist[i] = 1;  // 初始所有节点的最短路径为 1
    }

    // 按照拓扑顺序遍历节点
    for (int i = 0; i < size; i++) {
        int node = topo_order[i];
        EdgeList *p = villeges[node]._out_roads;
        while (p) {
            int next_node = p->_data;
            dist[next_node] = MAX(dist[next_node], dist[node] + 1);  // 更新最长路径
            p = p->next;
        }
    }

    // 找到并返回最长路径
    int max_path = 0;
    for (int i = 0; i < size; i++) {
        max_path = MAX(max_path, dist[i]);
    }

    // 释放内存
    free(in_degree);
    free(queue);
    free(dist);
    return max_path;
}

// 主函数，用于计算整个图的最长路径
int maxTravNodes(Villege* villeges, int size) {
    int *nodes_data = (int*)malloc(size * sizeof(int));
    memset(nodes_data, -1, size * sizeof(int));  // 初始化为 -1，表示未计算路径
    int max_nodes = 0;

    srand(time(NULL));  // 用当前时间作为随机种子
    switch ( rand() & 3) {
    case 1:
        max_nodes = maxTravNodesDFSIt(villeges, nodes_data, size);  // 使用非递归 DFS
        break;
    case 2:
        for (int i = 0; i < size; i++) {
            max_nodes = MAX(max_nodes, maxTravNodesDFSRe(villeges, nodes_data, i));  // 使用递归 DFS
        }
        break;
    default:
        max_nodes = maxTravNodesKahn(villeges, nodes_data, size);  // 使用 Kahn 算法
        break;
    }

    free(nodes_data);  // 释放内存
    return max_nodes;
}

// 主函数：读取输入并计算图的最长路径
int main() {
    int n, m;
    scanf("%d %d", &n, &m);  // 读取节点和边数
    Villege *villeges = (Villege *)malloc(n * sizeof(Villege));  // 分配节点数组
    memset(villeges,0,n*sizeof(villeges)); // 初始化

    // 读取每条边并构建图
    for (int i = 0; i < m; i++) {
        int x, y;
        scanf("%d %d", &x, &y);  // 读取边的两个端点
        x--;  // 转换为 0 基索引
        y--;
        EdgeList *node = (EdgeList *)malloc(sizeof(EdgeList));
        node->_data = y;
        node->next = villeges[x]._out_roads;  // 将节点加入出路链表
        villeges[x]._out_roads = node;
    }

    printf("%d\n", maxTravNodes(villeges, n));  // 输出最长路径

    // 释放内存
    for (int i = 0; i < n; i++) {
        EdgeList *p = villeges[i]._out_roads;
        while (p) {
            EdgeList *temp = p;
            p = p->next;
            free(temp);
        }
    }
    free(villeges);

    return 0;
}
