#include<iostream>
#define elemType int
/**
 * 线索二叉树利用了每个空指针, 在节点的leftChild或rightChild为空的时候, 将它们指向遍历顺序中的前驱或后继节点
 * 
 */
struct ThreadNode
{
    ThreadNode* leftChild;
    ThreadNode* rightChild;
    bool leftTag; // True 表示leftChild指向此节点前驱
    bool rightTag; // True表示rightChild指向此节点后继
    elemType data;
    ThreadNode(elemType data)
    {
        this->data = data;
        this->leftChild = NULL;
        this->rightChild = NULL;
        this->leftTag = false;
        this->rightTag = false;
    }
};

/**
 * 利用中序遍历将一个二叉树线索化
 * prevNode为之前访问的节点, 是对所有递归函数共享的一个变量
 * 注意, 左线索是给自己找前驱; 右线索是给前驱找后继,
 * 并且, 只能等到后继节点出现时, 再去补充其前驱的后继信息
 */
void makeInorderThreaded(ThreadNode* root, ThreadNode*& prevNode)
{
    if(!root) { return; }
    makeInorderThreaded(root->leftChild, prevNode); // 线索化左子树

    // 访问root, 访问后prevNode应该改变
    if(!root->leftChild)
    {
        root->leftChild = prevNode;
        root->leftTag = true;
    }
    if(prevNode && !prevNode->rightChild)
    {
        prevNode->rightChild = root;
        prevNode->rightTag = true;
    }
    prevNode = root;

    makeInorderThreaded(root->rightChild, prevNode); // 线索化右子树
}

void createInorderThreadedTree(ThreadNode* root)
{
    if(!root) { return; }
    ThreadNode* prevNode = NULL;
    makeInorderThreaded(root, prevNode);
    prevNode->rightChild = NULL; // 线索化之后, prevNode为遍历的最后一个节点, 它一定没有rightChild, 也处理它
    prevNode->rightTag = true;
}

/**
 * 知道root, 获取中序遍历序列中的第一个节点
 * 这个节点其实就是root左子树的最左下节点(但不一定是叶子)
 */
ThreadNode* getInorderTraverseFirstNode(ThreadNode* root)
{
    if(!root) { return NULL; }
    while (!root->leftTag)
    {
        root = root->leftChild;
    }
    
    return root;
}

/**
 * 获取中序遍历序列中的最后一个节点
 * 这个节点其实就是最右下的节点(不一定是叶子)
 */
ThreadNode* getInorderTraverseLastNode(ThreadNode* root)
{
    if(!root) { return NULL; }
    while (!root->rightTag)
    {
        root = root->rightChild;
    }
    
    return root;
}

/**
 * 获取中序遍历序列中当前节点的下一个节点
 */
ThreadNode* getInorderTraverseNextNode(ThreadNode* currentNode)
{
    if(currentNode->rightTag) { return currentNode->rightChild; } // 右线索
    else { return getInorderTraverseFirstNode(currentNode->rightChild); } // 右子树中最左下节点
}

/**
 * 获取中序遍历序列中当前节点的前一个节点
 */
ThreadNode* getInorderTraversePrevNode(ThreadNode* currentNode)
{
    if(currentNode->leftTag) { return currentNode->leftChild; }
    else { return getInorderTraverseLastNode(currentNode->leftChild); } // 左子树中最右下节点
}

/**
 * 线索二叉树的中序遍历
 */
void ThreadedBinaryTreeInorderTraverse(ThreadNode* root)
{
    if (!root) { return; }
    for (ThreadNode* currentNode=getInorderTraverseFirstNode(root); 
         currentNode != NULL; 
         currentNode = getInorderTraverseNextNode(currentNode))
    {
        std::cout << currentNode->data << ' ';
    }
}

int main() {
    //      5
    //     / \
    //   12   13
    //   /  \    \
    //  7    14   2
    // /  \   /  \  / \
    //17 23 27  3  8  11

    ThreadNode *root = new ThreadNode(5);
    root->leftChild = new ThreadNode(12);
    root->rightChild = new ThreadNode(13);

    root->leftChild->leftChild = new ThreadNode(7);
    root->leftChild->rightChild = new ThreadNode(14);

    root->rightChild->rightChild = new ThreadNode(2);

    root->leftChild->leftChild->leftChild = new ThreadNode(17);
    root->leftChild->leftChild->rightChild = new ThreadNode(23);

    root->leftChild->rightChild->leftChild = new ThreadNode(27);
    root->leftChild->rightChild->rightChild = new ThreadNode(3);

    root->rightChild->rightChild->leftChild = new ThreadNode(8);
    root->rightChild->rightChild->rightChild = new ThreadNode(11);
    createInorderThreadedTree(root);
    ThreadedBinaryTreeInorderTraverse(root);
}