#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>
typedef struct BinTreeNode
{
    int _data;
    struct BinTreeNode *_left_child;
    struct BinTreeNode *_right_child;
} BinTreeNode;
typedef struct BinTree
{
    struct BinTreeNode *_root;
    int _size;
    int *_pre_order;
    int *_post_order;
    int *_in_order;
} BinTree;
typedef BinTree RealBinTree;
bool InorderTraverse(const BinTreeNode *start, int out_order[], int *index)
{
    if (start == NULL)
        return true;
    // Traverse left subtree
    if (!InorderTraverse(start->_left_child, out_order, index))
        return false;
    // Visit the node
    out_order[(*index)++] = start->_data;
    // Traverse right subtree
    if (!InorderTraverse(start->_right_child, out_order, index))
        return false;
    return true;
}
int getIndex(int seq[], int start, int end, int target)
{
    for (int i = start; i <= end; i++)
    {
        if (seq[i] == target)
            return i;
    }
    return -1;
}
void freeTreeNodes(BinTreeNode *root)
{
    if (root == NULL)
        return;
    if (root->_left_child)
        freeTreeNodes(root->_left_child);
    if (root->_right_child)
        freeTreeNodes(root->_right_child);
    free(root);
}
bool rebuildTreePrePost(BinTree *tree, BinTreeNode **current, int pre_left, int pre_right, int post_left, int post_right)
{
    if (pre_left > pre_right || post_left > post_right)
        return false;
    if (pre_left == pre_right && post_left == post_right)
    {
        *current = NULL;
        return true;
    }
    *current = (BinTreeNode *)malloc(sizeof(BinTreeNode));
    if (!*current)
        return false;
    (*current)->_left_child=(*current)->_right_child=NULL;
    if (pre_left == pre_right - 1 && post_left == post_right - 1 && tree->_pre_order[pre_left] == tree->_post_order[post_left])
    {
       (*current)->_data = tree->_post_order[post_left];
        return true;
    }
    if (tree->_pre_order[pre_left] != tree->_post_order[post_right - 1])
        return false;
    (*current)->_data = tree->_pre_order[pre_left];
    int pre_left1 = pre_left + 1, pre_right2 = pre_right;
    int post_left1 = post_left, post_right2 = post_right - 1;
    int post_mi  = getIndex(tree->_post_order, post_left, post_right, tree->_pre_order[pre_left + 1]) + 1;
    if (post_mi == 0) return false;
   int pre_mi  = getIndex(tree->_pre_order, pre_left, pre_right, tree->_post_order[post_right2 - 1]);
    if (pre_mi== -1) return false;
     return rebuildTreePrePost(tree, &(*current)->_left_child, pre_left1, pre_mi, post_left1, post_mi) &&
                            rebuildTreePrePost(tree, &(*current)->_right_child, pre_mi, pre_right2, post_mi, post_right2);
}
int main()
{
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
    if (rebuildTreePrePost(tree, &tree->_root, 0, tree->_size, 0, tree->_size))
    {
        int index = 0;
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
    if (tree->_in_order)
        free(tree->_in_order);
    if (tree->_post_order)
        free(tree->_post_order);
    if (tree->_pre_order)
        free(tree->_pre_order);
    if (tree)
    {
        freeTreeNodes(tree->_root);
        free(tree);
    }
    return 0;
}
