#include "AVLTree.h"


//internal functions//
AVLTNode* _AVLT_newNode(int val);
bool _AVLT_add(AVLTNode **root, int val);
void _AVLT_printInorder(AVLTNode *root);//todo
void _AVLT_printPreorder(AVLTNode *root);//todo
void _AVLT_printPostorder(AVLTNode *root);//todo
void _AVLT_printTree(AVLTNode *root, int *height);
bool _AVLT_remove(AVLTNode **root, int val);
int _AVLT_findSub(AVLTNode *root);
int _AVLT_min(AVLTNode *root);
int _AVLT_max(AVLTNode *root);
void _AVLT_clear(AVLTNode **root);
void _AVLT_rotate(AVLTNode **root);
AVLTNode* _AVLT_rotateRight(AVLTNode *root);
AVLTNode* _AVLT_rotateLeft(AVLTNode *root);
int _AVLT_getBalance(AVLTNode *root);
int _AVLT_getHeight(AVLTNode *root);

void AVLT_init(AVLTree *avl)
{
    avl->numberOfNodes = 0;
    avl->root = NULL;
}

void AVLT_clear(AVLTree *avl)
{
    _AVLT_clear(&(avl->root));
    avl->numberOfNodes = 0;
    avl->root = NULL;
}

bool AVLT_add(AVLTree *avl, int val)
{
    bool ret = _AVLT_add(&avl->root, val);
    if(!ret)
    {
        printf("Value %d already exist in tree.\n", val);
    }
    return ret;
}

void AVLT_printTree(AVLTree *avl)
{
    int height = 0;
    _AVLT_printTree(avl->root, &height);
}

void AVLT_remove(AVLTree *avl, int val)
{
    if(avl->root == NULL)
    {
        printf("Tree is empty.\n");
    }
    bool ret = _AVLT_remove(&(avl->root), val);
    if(!ret)
    {
        printf("This value do not exist in tree\n");
    }
}

void _AVLT_clear(AVLTNode **root)
{
    if((*root) == NULL)
    {
        return;
    }
    _AVLT_clear(&(*root)->right);
    _AVLT_clear(&(*root)->left);
    free(*root);
}

AVLTNode* _AVLT_newNode(int val)
{
    AVLTNode *node = (AVLTNode*)malloc(sizeof(AVLTNode));
    node->data = val;
    node->balance = 0;
    node->left = NULL;
    node->right = NULL;
}


bool _AVLT_add(AVLTNode **root, int val)
{
    if((*root) == NULL)
    {
        *root = _AVLT_newNode(val);
        return true;
    }
    if(val > (*root)->data)
    {
        bool ret = _AVLT_add(&(*root)->right, val);
        if(ret)
        {
            (*root)->balance = _AVLT_getBalance(*root);
            if((*root)->balance > 1)
            {
                _AVLT_rotate(root);
            }
        }
        return ret;
    }
    if(val < (*root)->data)
    {
        bool ret = _AVLT_add(&(*root)->left, val);
        if(ret)
        {
            (*root)->balance = _AVLT_getBalance(*root);
            if((*root)->balance < -1)
            {
                _AVLT_rotate(root);
            }
        }
        return ret;
    }
    return false;
}

bool _AVLT_remove(AVLTNode **root, int val)
{
    if(*root == NULL)
    {
        return false;
    }
    if((*root)->data == val)
    {
        if((*root)->left == NULL && (*root)->right == NULL)
        {
            free(*root);
            *root = NULL;
            return true;
        }
        else
        {
            int sub = _AVLT_findSub(*root);
            (void)_AVLT_remove(root, sub);
            (*root)->data = sub;
            (*root)->balance = _AVLT_getBalance(*root);
            return true;
        }
    }
    else
    {
        bool retRight, retLeft;
        retRight = _AVLT_remove(&(*root)->right, val);
        retLeft = _AVLT_remove(&(*root)->left, val);
        (*root)->balance = _AVLT_getBalance(*root);
        return retRight || retLeft ? true : false;
    }
}

int _AVLT_findSub(AVLTNode *root)
{
    int *minRight = NULL, *maxLeft = NULL, Rdiff = 0, Ldiff = 0;
    if(root->right != NULL)
    {
        minRight = (int *)malloc(sizeof(int));
        *minRight = _AVLT_min(root->right);
        Rdiff = *minRight - root->data;
    }
    if(root->left != NULL)
    {
        maxLeft = (int *)malloc(sizeof(int));
        *maxLeft = _AVLT_max(root->left);
        Ldiff = root->data - *maxLeft;
    }
    if(minRight != NULL && maxLeft != NULL)
    {
        int max = *maxLeft, min = *minRight;
        free(minRight);
        free(maxLeft);
        return Rdiff < Ldiff ? min : max;
    }
    else if(minRight == NULL)
    {
        int max = *maxLeft;
        free(maxLeft);
        return max;
    }
    else
    {
        int min = *minRight;
        free(minRight);
        return min;
    }
}

int _AVLT_min(AVLTNode *root)
{
    while (root->left != NULL)
    {
        root = root->left;
    }
    return root->data;
}

int _AVLT_max(AVLTNode *root)
{
    while (root->right != NULL)
    {
        root = root->right;
    }
    return root->data;
}

void _AVLT_printTree(AVLTNode *root, int *height)
{
    (*height)++;
    if(root == NULL)
    {
        return;
    }
    _AVLT_printTree(root->right, height);
    for (size_t i = 0; i < (*height); i++)
    {
        printf("    ");
    }
    printf("%d(%d)\n", root->data, root->balance);
    (*height)--;
    _AVLT_printTree(root->left, height);
    (*height)--;
}

void _AVLT_rotate(AVLTNode **root)
{
    if((*root)->balance > 0)
    {
        (*root) = _AVLT_rotateLeft(*root);
    }
    else
    {
        (*root) = _AVLT_rotateRight(*root);
    }
    (*root)->balance = _AVLT_getBalance(*root);
}

AVLTNode* _AVLT_rotateRight(AVLTNode *root)
{
    AVLTNode *x = root->left;
    AVLTNode *T2 = x->right;
 
    x->right = root;
    root->left = T2;
    root->balance = _AVLT_getBalance(root);
    return x;
}

AVLTNode* _AVLT_rotateLeft(AVLTNode *root)
{
    AVLTNode *y = root->right;
    AVLTNode *T2 = y->left;
 
    y->left = root;
    root->right = T2;
    root->balance = _AVLT_getBalance(root);

    return y;
}

int _AVLT_getBalance(AVLTNode *root)
{
    int right = -1, left = -1;
    if(root->right != NULL)
    {
        right = _AVLT_getHeight(root->right);
    }
    if(root->left != NULL)
    {
        left = _AVLT_getHeight(root->left);
    }
    return right - left;
}

int _AVLT_getHeight(AVLTNode* root) {
    if (root == NULL) {
        return -1;
    }

    int lefth = _AVLT_getHeight(root->left);
    int righth = _AVLT_getHeight(root->right);

    if (lefth > righth) {
        return lefth + 1;
    } else {
        return righth + 1;
    }
}