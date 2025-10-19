#include <iostream>
using namespace std;

struct Node
{
    int key;
    int height;
    Node* leftChild;
    Node* rightChild;

    Node(int key):
    key(key),
    leftChild(nullptr),
    rightChild(nullptr),
    height(1){}

};

void traverseInorder(Node* root)
{
    if(!root) { return; }
    traverseInorder(root->leftChild);
    cout << root->key << ' ';
    traverseInorder(root->rightChild);
}

/**
 * 获取以root为根的子树的高度
 * 这个高度是从根向叶子节点数的
 */
int getTreeHeight(Node* root)
{
    if(!root) { return 0; }
    return root->height;
}

/**
 * 获取以root为根的子树的平衡因子
 */
int getSubtreeBalance(Node* root)
{
    if(!root) { return 0;}
    return getTreeHeight(root->leftChild) - getTreeHeight(root->rightChild);
}

/**
 * 将一个以parent为根的子树右旋
 * 这个过程是:
 * 让现在的根作为左孩子的右孩子，
 * 同时，如果左孩子有右孩子，
 * 那么要把它接到现在的根上，
 * 并更新树高
 * 返回调整后的子树根
 */
Node* rightRotate(Node* parent)
{
    Node* leftChild = parent->leftChild;
    Node* leftSubtreeRightChild = leftChild->rightChild;
    leftChild->rightChild = parent;
    parent->leftChild = leftSubtreeRightChild;

    parent->height = 1 + max(
        getTreeHeight(parent->leftChild),
        getTreeHeight(parent->rightChild)
    );
    leftChild->height = 1 + max(
        getTreeHeight(leftChild->leftChild),
        getTreeHeight(leftChild->rightChild)
    );

    // 返回新的根
    return leftChild;
}

/**
 * 左旋以parent为根的子树
 * 这个过程是:
 * 将现在的根作为右孩子的左孩子，
 * 同时，如果右孩子有左孩子，
 * 那么把它接到现在的根上，
 * 并更新树高
 */
Node* leftRotate(Node* parent)
{
    Node* rightChild = parent->rightChild;
    Node* rightSubtreeLeftChild = rightChild->leftChild;
    rightChild->leftChild = parent;
    parent->rightChild = rightSubtreeLeftChild;

    parent->height = 1 + max(
        getTreeHeight(parent->leftChild),
        getTreeHeight(parent->rightChild)
    );
    rightChild->height = 1 + max(
        getTreeHeight(rightChild->leftChild),
        getTreeHeight(rightChild->rightChild)
    );

    return rightChild;
}

/**
 * 向AVL子树插入值为key的节点
 * 返回调整后的子树根
 */
Node* insertNode(Node* root, int key)
{
    // 普通的BST插入
    if(!root) { return new Node(key);}
    if(root->key < key)
    {
        root->rightChild = insertNode(root->rightChild, key);
    }
    else if(root->key > key)
    {
        root->leftChild = insertNode(root->leftChild, key);
    }
    else
    {
        return root;
    }

    // 插入后, 更新树高并计算平衡因子
    root->height = 1 + max(
        getTreeHeight(root->leftChild),
        getTreeHeight(root->rightChild)
    );
    int balanceFactor = getSubtreeBalance(root);

    // 如果以root为根的节点是不平衡, 共四种情况
    if(balanceFactor>1 && key<root->leftChild->key) // LL
    {
        /**
         * 这里的LL可以理解为,
         * 对于被破坏了平衡因子规则的root出发,
         * 破坏了平衡因子的节点在 左子树->左子树 上
         * 即: 描述 失衡节点->过高的子树的方向->新节点插入方向 的轨迹
         * 这时候需要把root子树右旋
         */
        return rightRotate(root);
    }
    else if(balanceFactor<-1 && key>root->rightChild->key) // RR
    {
        return leftRotate(root);
    }
    else if(balanceFactor>1 && key>root->leftChild->key) // LR
    {
        // 先把左子树左旋变成LL型
        root->leftChild = leftRotate(root->leftChild);
        return rightRotate(root);
    }
    else if(balanceFactor<-1 && key<root->rightChild->key) // RL
    {
        // xxx先把右子树右旋变成RR型
        root->rightChild = rightRotate(root->rightChild);
        return leftRotate(root);
    }

    // 一个节点没有被修改, 直接返回
    return root;
}

/**
 * 在以root为根的子树上, 删除值为key的节点
 * 返回修改后的子树根
 */
Node* deleteNode(Node* root, int key)
{
    // 先进行普通的BST删除操作
    if(!root) { return root; }
    if(key < root->key)
    {
        root->leftChild = deleteNode(root->leftChild, key);
    }
    else if(key > root->key)
    {
        root->rightChild = deleteNode(root->rightChild, key);
    }
    else
    {
        if(!root->leftChild || !root->rightChild)
        {
            Node* temp = root->leftChild ?
                         root->leftChild : root->rightChild;
            if(!temp) // 两孩子都没有
            {
                temp = root;
                root = nullptr;
            }
            else // 只有一个孩子
            {
                *root = *temp; // 把孩子复制到根上
            }
            delete temp;
        }
        else
        {
            // 找中序后继
            Node* successorParent = root;
            Node* inorderSuccessor = root->rightChild;
            while(inorderSuccessor->leftChild)
            {
                successorParent = inorderSuccessor;
                inorderSuccessor = inorderSuccessor->leftChild;
            }
            // 交换root和中序后继, 并删除中序后继
            root->key = inorderSuccessor->key;
            if(successorParent->leftChild == inorderSuccessor)
            {
                successorParent->leftChild = inorderSuccessor->rightChild;
            }
            else
            {
                successorParent->rightChild = inorderSuccessor->rightChild;
            }
            delete inorderSuccessor;
        }
    }

    if(!root) { return root; } // 删除的是一个叶节点

    // 最后更新当前子树的树高并平衡子树
    root->height = 1 + max(
        getTreeHeight(root->leftChild),
        getTreeHeight(root->rightChild)
    );
    int balanceFactor = getSubtreeBalance(root);
    // 对不平衡子树, 有四种可能
    if(balanceFactor>1 && getSubtreeBalance(root->leftChild)>=0) // LL
    {
        /**
         * 这里的LL,第一步也是确定哪个子树更高(L子树更高)
         * 之后, 看子树的子树哪个更高,确定子树的倾斜方向
         */
        return rightRotate(root);
    }
    else if(balanceFactor<-1 && getSubtreeBalance(root->rightChild)<=0) // RR
    {
        return leftRotate(root);
    }
    else if(balanceFactor>1 && getSubtreeBalance(root->leftChild)<0) // LR
    {
        root->leftChild = leftRotate(root->leftChild);
        return rightRotate(root);
    }
    else if(balanceFactor<-1 && getSubtreeBalance(root->rightChild)) // RL
    {
        root->rightChild = rightRotate(root->rightChild);
        return leftRotate(root);
    }

    return root;
}

/**
 * 从根为root的子树中搜索值为key的节点
 */
Node* searchNode(Node* root, int key)
{
    if(!root) { return nullptr; }
    if(root->key == key)
    {
        return root;
    }
    if(key < root->key)
    {
        return searchNode(root->leftChild, key);
    }
    else
    {
        return searchNode(root->rightChild, key);
    }
}

int main() { 
    Node *root = nullptr; 
    
    // Constructing tree given in the above figure 
    root = insertNode(root, 10); 
    root = insertNode(root, 20); 
    root = insertNode(root, 30); 
    root = insertNode(root, 40); 
    root = 
    insertNode(root, 50); 
    root = insertNode(root, 25); 
    
    /*
              30 
            /   \ 
          20     40 
         /  \      \ 
       10   25     50 
    */
    
    traverseInorder(root); 
    cout<<endl;

    root = deleteNode(root, 30);
    traverseInorder(root);
    cout<<endl;

    cout << searchNode(root, 10)->key;

    return 0; 
}