#include<iostream>
using namespace std;

struct BSTNode
{
    int key;
    BSTNode* leftChild;
    BSTNode* rightChild;
    BSTNode(int key):key(key),leftChild(nullptr),rightChild(nullptr){}
};

/**
 * 在BST中插入一个值为key的节点
 */
BSTNode* insertNode(BSTNode* root, int key)
{
    if(!root) { return new BSTNode(key); } // 一个子树为空, 创建对应节点
    if (root->key == key) { return root; } // 已经有了值为key的节点, 直接返回
    if(root->key < key) // 进入右子树
    {
        root->rightChild = insertNode(root->rightChild, key);
    }
    else // 进入左子树
    {
        root->leftChild = insertNode(root->leftChild, key);
    }

    return root;
}

/**
 * 在子树中搜索值为key的节点
 */
BSTNode* searchNode(BSTNode* root, int key)
{
    if(!root) { return nullptr; }
    if(root->key == key) { return root; }
    if(root->key > key) // 进入左子树
    {
        return searchNode(root->leftChild, key);
    }
    else
    {
        return searchNode(root->rightChild, key);
    }
}

/**
 * 在一个子树中, 删除值为key的节点
 * 当节点没有左右孩子时, 最简单, 只需要将节点置为空即可
 * 当节点只有一个孩子子树时, 直接用孩子子树替换这个节点
 * 当节点有两个孩子子树时, 找到节点的中序后继(或者中序前驱), 用其替换节点, 再删除中序后继(或前驱)
 * 因为中序后继为右子树中最左下的节点, 很好删除, 并且它一定没有左孩子
 */
BSTNode* deleteNode(BSTNode* root, int key)
{
    if (!root) { return root; }
    if(root->key > key) // 要删除的节点在root的左子树
    {
        root->leftChild = deleteNode(root->leftChild, key);
        return root;
    }
    else if(root->key < key) // 要删除的节点在root的右子树
    {
        root->rightChild = deleteNode(root->rightChild, key);
        return root;
    }
    else // root就是要删除的节点
    {
        if(!root->leftChild) // 要删除的节点没有左子树, 直接用右子树替换root
        {
            BSTNode* tempPtr = root->rightChild;
            delete root;
            return tempPtr;
        }
        else if(!root->rightChild) // 要删除的节点没有右子树, 直接用左子树替换root
        {
            BSTNode* tempPtr = root->leftChild;
            delete root;
            return tempPtr;
            // 上面这两个if已经足够处理root两个孩子都没有的情况了
        }
        else // 左右孩子都有
        {
            BSTNode* successorParent = root;
            BSTNode* inorderSuccessor = root->rightChild;
            while (inorderSuccessor->leftChild)
            {
                successorParent = inorderSuccessor;
                inorderSuccessor = inorderSuccessor ->leftChild;
            }
            root->key = inorderSuccessor->key;
            if(successorParent->leftChild == inorderSuccessor)
            {
                // 中序后继在右子树的最左下
                successorParent->leftChild = inorderSuccessor->rightChild;
            }
            else
            {
                // 中序后继就是右子树的根
                successorParent->rightChild = inorderSuccessor->rightChild;
            }
            delete inorderSuccessor;
            return root;
        }
    }
}

/**
 * 中序遍历BST, 输出有序序列
 */
void traverseBSTInorder(BSTNode* root)
{
    if(!root) { return; }
    traverseBSTInorder(root->leftChild);
    cout << root->key << ' ';
    traverseBSTInorder(root->rightChild);

    return;
}

int main()
{
    BSTNode* root = nullptr;
    root = insertNode(root, 10);
    root = insertNode(root, 5);
    root = insertNode(root, 15);
    root = insertNode(root, 12);
    root = insertNode(root, 28);
    traverseBSTInorder(root);
    int x = 15;
    if(searchNode(root, 15))
    {
        cout << endl << "15 is in BST" << endl;
    }
    root = deleteNode(root, x);
    traverseBSTInorder(root);
    return 0;
}