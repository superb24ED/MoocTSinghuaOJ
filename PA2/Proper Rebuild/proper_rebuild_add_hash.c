#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_VAL 1000  // 假设树节点值的范围在0到MAX_VAL之间，根据实际情况可调整

// 二叉树节点结构
typedef struct BinTreeNode {
    int _data;
    struct BinTreeNode *_left_child;
    struct BinTreeNode *_right_child;
} BinTreeNode;

// 二叉树结构
typedef struct BinTree {
    struct BinTreeNode *_root;
    int _size;
    int *_pre_order;
    int *_post_order;
    int *_in_order;
} BinTree;

typedef BinTree RealBinTree;

// 中序遍历函数，使用递归遍历二叉树
bool InorderTraverse(const BinTreeNode *start, int out_order[], int *index) {
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

// 构建索引哈希表的函数，用于优化查找
int *buildIndexMap(int arr[], int size) {
    // 创建索引数组（用作哈希表），大小为 MAX_VAL + 1
    int *index_map = (int *)malloc((MAX_VAL + 1) * sizeof(int));
    if (!index_map) return NULL;

    // 初始化每个位置为 -1，表示没有该元素
    for (int i = 0; i <= MAX_VAL; i++) {
        index_map[i] = -1;
    }

    // 填充哈希表，存储数组中每个元素的索引
    for (int i = 0; i < size; i++) {
        index_map[arr[i]] = i;
    }
    return index_map;
}

// 使用哈希表查找目标值的索引
int getIndexWithMap(int *index_map, int target) {
    if (target < 0 || target > MAX_VAL) return -1;  // 确保在合法范围内
    return index_map[target];
}

// 释放二叉树节点内存的递归函数
void freeTreeNodes(BinTreeNode *root) {
    if (root == NULL)
        return;
    if (root->_left_child)
        freeTreeNodes(root->_left_child);
    if (root->_right_child)
        freeTreeNodes(root->_right_child);
    free(root);
}

// 使用前序和后序序列重建二叉树
bool rebuildTreePrePost(BinTree *tree, BinTreeNode **current, int pre_left, int pre_right, int post_left, int post_right, int *index_map) {
    if (pre_left > pre_right || post_left > post_right)
        return false;

    // 叶子节点情况
    if (pre_left == pre_right && post_left == post_right) {
        *current = NULL;
        return true;
    }

    *current = (BinTreeNode *)malloc(sizeof(BinTreeNode));
    if (!*current)
        return false;
    (*current)->_left_child = (*current)->_right_child = NULL;

    // 只有一个节点的情况
    if (pre_left == pre_right - 1 && post_left == post_right - 1 && tree->_pre_order[pre_left] == tree->_post_order[post_left]) {
        (*current)->_data = tree->_post_order[post_left];
        return true;
    }

    // 检查根节点是否匹配
    if (tree->_pre_order[pre_left] != tree->_post_order[post_right - 1])
        return false;

    (*current)->_data = tree->_pre_order[pre_left];
    int pre_left1 = pre_left + 1, pre_right2 = pre_right;
    int post_left1 = post_left, post_right2 = post_right - 1;

    // 使用哈希表查找索引
    int post_mi = getIndexWithMap(index_map, tree->_pre_order[pre_left + 1]) + 1;
    if (post_mi == 0) return false;

    int pre_mi = getIndexWithMap(index_map, tree->_post_order[post_right2 - 1]);
    if (pre_mi == -1) return false;

    return rebuildTreePrePost(tree, &(*current)->_left_child, pre_left1, pre_mi, post_left1, post_mi, index_map) &&
           rebuildTreePrePost(tree, &(*current)->_right_child, pre_mi, pre_right2, post_mi, post_right2, index_map);
}

int main() {
    RealBinTree *tree = (RealBinTree *)malloc(sizeof(RealBinTree));
    if (!tree)
        goto Cleanup;

    tree->_pre_order = tree->_post_order = tree->_in_order = NULL;
    if (!scanf("%d", &tree->_size))
        return -1;
    if (tree->_size < 1)
        goto Cleanup;

    tree->_pre_order = (int *)malloc(tree->_size * sizeof(int));
    tree->_post_order = (int *)malloc(tree->_size * sizeof(int));
    tree->_in_order = (int *)malloc(tree->_size * sizeof(int));
    if (!(tree->_pre_order && tree->_post_order && tree->_in_order))
        goto Cleanup;

    for (int i = 0; i < tree->_size; i++)
        scanf("%d", tree->_pre_order + i);
    for (int i = 0; i < tree->_size; i++)
        scanf("%d", tree->_post_order + i);

    // 使用后序序列构建哈希表
    int *index_map = buildIndexMap(tree->_post_order, tree->_size);
    if (!index_map)
        goto Cleanup;

    if (rebuildTreePrePost(tree, &tree->_root, 0, tree->_size, 0, tree->_size, index_map)) {
        int index = 0;
        if (InorderTraverse(tree->_root, tree->_in_order, &index)) {
            for (int i = 0; i < tree->_size; i++) {
                printf("%d", tree->_in_order[i]);
                if (i != tree->_size - 1)
                    printf(" ");
            }
            printf("\n");
        }
    }

Cleanup:
    if (tree->_in_order)
        free(tree->_in_order);
    if (tree->_post_order)
        free(tree->_post_order);
    if (tree->_pre_order)
        free(tree->_pre_order);
    if (tree) {
        freeTreeNodes(tree->_root);
        free(tree);
    }
    if (index_map)
        free(index_map);

    return 0;
}
