#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>

// 定义二叉树节点结构体
typedef struct BinTreeNode
{
    int _data;                        // 节点存储的数据
    struct BinTreeNode *_left_child;  // 指向左子节点的指针
    struct BinTreeNode *_right_child; // 指向右子节点的指针
} BinTreeNode;

// 定义二叉树结构体
typedef struct BinTree
{
    struct BinTreeNode *_root; // 二叉树的根节点
    int _size;                 // 节点数
    int *_pre_order;           // 前序遍历序列数组
    int *_post_order;          // 后序遍历序列数组
    int *_in_order;            // 中序遍历序列数组
} BinTree;

typedef BinTree RealBinTree; // 真二叉树

// 中序遍历函数，将遍历结果存储到 out_order 数组中
bool InorderTraverse(const BinTreeNode *start, int out_order[], int *index)
{
    if (start == NULL)
        return true;

    // 遍历左子树
    if (!InorderTraverse(start->_left_child, out_order, index))
        return false;

    // 访问当前节点
    out_order[(*index)++] = start->_data;

    // 遍历右子树
    if (!InorderTraverse(start->_right_child, out_order, index))
        return false;

    return true;
}

// 查找目标值在序列 seq 的区间 [start, end] 中的索引位置
int getIndex(int seq[], int start, int end, int target)
{
    for (int i = start; i <= end; i++)
    {
        if (seq[i] == target)
            return i;
    }
    return -1; // 若未找到目标值，则返回 -1
}

// 递归释放所有二叉树节点的内存
void freeTreeNodes(BinTreeNode *root)
{
    if (root == NULL)
        return;

    // 递归释放左右子节点
    if (root->_left_child)
        freeTreeNodes(root->_left_child);
    if (root->_right_child)
        freeTreeNodes(root->_right_child);

    // 释放当前节点
    free(root);
}

// 基于前序和后序遍历序列重建二叉树
bool rebuildTreePrePost(BinTree *tree, BinTreeNode **current, int pre_left, int pre_right, int post_left, int post_right)
{
    // 基本情况：若区间无效，返回 false
    if (pre_left > pre_right || post_left > post_right)
        return false;

    // 啥都木有，说明前一个节点是叶子, 无视，退出，返回true
    if (pre_left == pre_right && post_left == post_right)
    {
        *current = NULL;
        return true;
    }

    // 分配内存并初始化当前节点
    *current = (BinTreeNode *)malloc(sizeof(BinTreeNode));
    if (!*current)
        return false; // 内存分配失败
    (*current)->_left_child = (*current)->_right_child = NULL;

    // 若只剩一个节点，当前为叶子节点，无需继续递归下去了
    if (pre_left == pre_right - 1 && post_left == post_right - 1 && tree->_pre_order[pre_left] == tree->_post_order[post_left])
    {
        (*current)->_data = tree->_post_order[post_left];
        return true;
    }

    // 检查根节点是否一致
    if (tree->_pre_order[pre_left] != tree->_post_order[post_right - 1])
        return false;

    // 设置当前节点的数据为前序遍历序列中的根节点
    (*current)->_data = tree->_pre_order[pre_left];
    /*
//                                left1                     left2                  right1                        right2
//pre order:    root     root_left_____________                     root_right_______________
//pos order:                _____________root_left                    _______________root_right              root
//                               ------sub tree 1------                  ---- --------sub tree 2-------
*/
    // 计算左右子树的前序和后序遍历的区间
    int pre_left1 = pre_left + 1, pre_right2 = pre_right;
    int post_left1 = post_left, post_right2 = post_right - 1;

    // 找到左右子树的边界
    int post_mi = getIndex(tree->_post_order, post_left, post_right, tree->_pre_order[pre_left + 1]) + 1;
    if (post_mi == 0)
        return false;

    int pre_mi = getIndex(tree->_pre_order, pre_left, pre_right, tree->_post_order[post_right2 - 1]);
    if (pre_mi == -1)
        return false;

    // 递归构建左右子树
    return rebuildTreePrePost(tree, &(*current)->_left_child, pre_left1, pre_mi, post_left1, post_mi) &&
           rebuildTreePrePost(tree, &(*current)->_right_child, pre_mi, pre_right2, post_mi, post_right2);
}

int main()
{
    // 创建树并分配内存
    RealBinTree *tree = (RealBinTree *)malloc(sizeof(RealBinTree));
    if (!tree)
        goto Cleanup; // 内存分配失败，跳到清理部分

    tree->_pre_order = tree->_post_order = tree->_in_order = NULL;

    // 读取树的大小
    if (!scanf("%d", &tree->_size))
        return -1;

    if (tree->_size < 1)
        goto Cleanup;

    // 为前序、后序和中序遍历数组分配内存
    tree->_pre_order = (int *)malloc(tree->_size * sizeof(int));
    tree->_post_order = (int *)malloc(tree->_size * sizeof(int));
    tree->_in_order = (int *)malloc(tree->_size * sizeof(int));
    if (!(tree->_pre_order && tree->_post_order && tree->_in_order))
        goto Cleanup;

    // 读取前序和后序遍历序列
    for (int i = 0; i < tree->_size; i++)
        scanf("%d", tree->_pre_order + i);
    for (int i = 0; i < tree->_size; i++)
        scanf("%d", tree->_post_order + i);

    // 根据前序和后序遍历重建树
    if (rebuildTreePrePost(tree, &tree->_root, 0, tree->_size, 0, tree->_size))
    {
        int index = 0;

        // 中序遍历并输出结果
        if (InorderTraverse(tree->_root, tree->_in_order, &index))
        {
            for (int i = 0; i < tree->_size; i++)
            {
                printf("%d", tree->_in_order[i]);
                if (i != tree->_size - 1)
                    printf(" ");
            }
            printf("\n");
        }
    }

Cleanup:
    // 释放分配的内存
    if (tree)
    {
        if (tree->_in_order)
            free(tree->_in_order);
        if (tree->_post_order)
            free(tree->_post_order);
        if (tree->_pre_order)
            free(tree->_pre_order);
        freeTreeNodes(tree->_root);
        free(tree);
    }
    return 0;
}
