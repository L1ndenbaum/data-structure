#include<iostream>
#include<queue>
#include<vector>
#include<string>
#include<stack>
#include<unordered_map>
#include<functional>
#define elemType int

struct TreeNode
{
    elemType data;
    TreeNode* leftChild;
    TreeNode* rightChild;
    TreeNode(int data):leftChild(NULL),rightChild(NULL),data(data){}
};

// 先序遍历, root->leftChild->rightChild
void traverseBinaryTreePreorder(TreeNode* root)
{
    if(!root) { return; }
    std::cout<< root->data << ' ';
    traverseBinaryTreePreorder(root->leftChild);
    traverseBinaryTreePreorder(root->rightChild);
}

// 先序遍历的非递归写法
void traverseBinaryTreePreorderNonRecursive(TreeNode* root)
{
    if(!root) { return; }
    std::stack<TreeNode*> stack;
    stack.push(root);
    while (!stack.empty())
    {
        root = stack.top();
        std::cout<< root->data << ' ';
        stack.pop();
        if(root->rightChild) { stack.push(root->rightChild); }
        if(root->leftChild) { stack.push(root->leftChild); }
    }
    
    return;
}

// 中序遍历, leftChild->root->rightChild
void traverseBinaryTreeInorder(TreeNode* root)
{
    if(!root) { return; }
    traverseBinaryTreeInorder(root->leftChild);
    std::cout<< root->data << ' ';
    traverseBinaryTreeInorder(root->rightChild);
}

void traverseBinaryTreeInorderNonRecursive(TreeNode* root)
{
    std::stack<TreeNode*> stack;
    while (root || !stack.empty())
    {
        while (root) // 左子树一直往下走并入栈
        {
            stack.push(root);
            root = root->leftChild;
        }

        root = stack.top(); // 子树的根
        stack.pop();
        std::cout << root->data << ' ';

        root = root->rightChild; // 进入右子树
    }
    
    return;
}

// 后序遍历, leftChild->rightChild->root
void traverseBinaryTreePostorder(TreeNode* root)
{
    if(!root) { return; }
    traverseBinaryTreePostorder(root->leftChild);
    traverseBinaryTreePostorder(root->rightChild);
    std::cout<< root->data << ' ';
}

void traverseBinaryTreePostorderNonRecursive(TreeNode* root)
{
    if (!root) return;

    std::stack<TreeNode*> st;
    TreeNode* lastVisited = NULL;  // 记录最近访问过的节点
    TreeNode* curr = root;

    while (curr || !st.empty()) 
    {
        // 一直往左走
        while (curr) 
        {
            st.push(curr);
            curr = curr->leftChild;
        }

        // 查看栈顶
        TreeNode* node = st.top();

        // 如果右子树存在且没访问过，转向右子树
        if (node->rightChild && lastVisited != node->rightChild) 
        {
            curr = node->rightChild;
        } 
        else 
        {
            // 否则就访问栈顶节点
            std::cout << node->data << ' ';
            lastVisited = node;
            st.pop();
        }
    }
}

// 使用队列进行层次遍历, 逐层输出树的内容
void traverseBinaryTreeLevelOrder(TreeNode* root)
{
    std::vector<std::vector<int>> res = {{}};
    std::queue<TreeNode*> queue;
    queue.push(root);
    int level = 0;
    while (!queue.empty())
    {
        int numLevelNodes = queue.size();
        for (int i = 0; i < numLevelNodes; i++)
        {
            TreeNode* front = queue.front();
            res[level].push_back(front->data);
            queue.pop();
            if(front->leftChild) { queue.push(front->leftChild); }
            if(front->rightChild) { queue.push(front->rightChild); }
        }
        level++;
        res.push_back({});
    }

    for (int currentLevel = 0; currentLevel < level; currentLevel++)
    {
        std::cout << "Level " << currentLevel << ':' << std::endl;
        for(int& data : res[currentLevel])
        {
            std::cout << data << ' ';
        }
        std::cout << std::endl;
    }
}

/**
 * 根据先序遍历和中序遍历确定一个二叉树
 * 因为先序遍历是Root->L->R, 先在先序遍历中找到Root
 * 之后去中序遍历中找Root在哪, 划分左右子树, 确定左右子树的大小
 * 确定了左右子树的大小后, 就能找到先序遍历中左右子树的对应子序列, 进而确定左子树和右子树的根
 * 再去创建左右子树
 * build()函数从先序遍历的preorderStartIdx位置开始找到根
 * 从 inorderSeq的 inorderStartIdx到inorderEndIdx部分确定子树
 */
TreeNode* constructBinaryTreeByPreorderInorderSequences(std::vector<int>& preorderSeq, std::vector<int>& inorderSeq)
{
    std::unordered_map<char, int> inorderIndexMap;// 存中序遍历值对应的位置索引
    for(int i=0; i<inorderSeq.size(); i++)
    {
        inorderIndexMap[inorderSeq[i]] = i;
    }

    std::function<TreeNode*(int,int,int)> 
    build = [&](int preorderStartIdx, int inorderStartIdx, int inorderEndIdx) -> TreeNode*
    {
        if(inorderStartIdx > inorderEndIdx) { return NULL; }
        elemType rootData = preorderSeq[preorderStartIdx];
        TreeNode* root = new TreeNode(rootData);
        int inorderRootIdx = inorderIndexMap[rootData];
        int leftSubtreeSize = inorderRootIdx - inorderStartIdx;
        root->leftChild = build(
            preorderStartIdx+1, 
            inorderStartIdx, 
            inorderRootIdx-1
        );
        root->rightChild = build(
            preorderStartIdx+1 + leftSubtreeSize,
            inorderRootIdx+1,
            inorderEndIdx
        );

        return root;
    };
    
    return build(0, 0, inorderSeq.size()-1);
}

/**
 * 根据后序和中序序列还原二叉树
 * 因为后序是 L->R->Root, 所以和先序还原差不多, 但是反着来
 */
TreeNode* constructBinaryTreeByPostorderInorderSequences(std::vector<int>& postorderSeq, std::vector<int>& inorderSeq)
{
    std::unordered_map<int, int> inorderIdxMap;
    for (int i = 0; i < inorderSeq.size(); i++)
    {
        inorderIdxMap[inorderSeq[i]] = i;
    }

    std::function<TreeNode*(int, int, int)> 
    build = [&](int postorderEnd, int inorderStart, int inorderEnd) -> TreeNode*
    {
        if(inorderStart > inorderEnd) { return NULL; }
        TreeNode* root = new TreeNode(postorderSeq[postorderEnd]);
        int rootInorderIdx = inorderIdxMap[postorderSeq[postorderEnd]];
        int rightSubTreeSize = inorderEnd - rootInorderIdx;
        root->rightChild = build(
            postorderEnd-1, 
            rootInorderIdx+1, 
            inorderEnd
        );
        root->leftChild = build(
            postorderEnd-rightSubTreeSize-1, 
            inorderStart,
            rootInorderIdx-1
        );

        return root;
    };
    
    return build(postorderSeq.size()-1, 0, inorderSeq.size()-1);
}

int main() 
{
    //      5
    //     / \
    //   12   13
    //   /  \    \
    //  7    14   2
    // /  \   /  \  / \
    //17 23 27  3  8  11

    TreeNode *root = new TreeNode(5);
    root->leftChild = new TreeNode(12);
    root->rightChild = new TreeNode(13);

    root->leftChild->leftChild = new TreeNode(7);
    root->leftChild->rightChild = new TreeNode(14);

    root->rightChild->rightChild = new TreeNode(2);

    root->leftChild->leftChild->leftChild = new TreeNode(17);
    root->leftChild->leftChild->rightChild = new TreeNode(23);

    root->leftChild->rightChild->leftChild = new TreeNode(27);
    root->leftChild->rightChild->rightChild = new TreeNode(3);

    root->rightChild->rightChild->leftChild = new TreeNode(8);
    root->rightChild->rightChild->rightChild = new TreeNode(11);
    traverseBinaryTreeInorder(root);
    std::cout<<std::endl;
    traverseBinaryTreeInorderNonRecursive(root);
}