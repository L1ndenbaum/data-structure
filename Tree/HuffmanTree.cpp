#include<queue>
#include<iostream>
#include<vector>
#include<string>
#include<unordered_map>
#include<functional>

struct TreeNode
{
    int weight;
    char data;
    TreeNode* leftChild;
    TreeNode* rightChild;
    struct Compare
    {
        bool operator()(TreeNode* nodeA, TreeNode* nodeB)
        {
            return nodeA->weight > nodeB->weight; // 小顶堆
        }
    };
    TreeNode(int weight, char data):weight(weight), data(data), leftChild(NULL), rightChild(NULL){}
    TreeNode(
        int weight, 
        TreeNode* leftChild, 
        TreeNode* rightChild):weight(weight), leftChild(leftChild), rightChild(rightChild){}
};

/**
 * WPL, 带权路径长度, 为所有叶节点的带权路径长度和
 */
int getTreeWeightPathLength(TreeNode* root, int currentDepth)
{
    if(!root) { return 0; }
    if(!root->leftChild && !root->rightChild) // 叶节点
    {
        return root->weight * currentDepth;
    }

    int leftSubtreeWPL = 0; int rightSubtreeWPL = 0;
    if(root->leftChild)
    {
        leftSubtreeWPL = getTreeWeightPathLength(root->leftChild, currentDepth+1);
    }
    if(root->rightChild)
    {
        rightSubtreeWPL = getTreeWeightPathLength(root->rightChild, currentDepth+1);
    }

    return leftSubtreeWPL + rightSubtreeWPL;
}

TreeNode* buildHuffmanTree(std::vector<TreeNode*>& leafNodes)
{
    std::priority_queue<TreeNode*, std::vector<TreeNode*>, TreeNode::Compare> priority_queue;
    for (TreeNode*& leafNode : leafNodes) { priority_queue.push(leafNode); }
    while (priority_queue.size() != 1)
    {
        TreeNode* minWeightTree = priority_queue.top();
        priority_queue.pop();
        TreeNode* lastButOneWeightTree = priority_queue.top();
        priority_queue.pop();
        TreeNode* newNode = new TreeNode(
            minWeightTree->weight + lastButOneWeightTree->weight,
            minWeightTree, 
            lastButOneWeightTree
        );
        priority_queue.push(newNode);
    }

    return priority_queue.top();
}

/**
 * 前缀编码的过程, 用二叉树进行
 * 往左子树走, 编码尾部添加0, 往右子树走添加1
 * 前缀编码: 没有任何一个编码是另一个编码的前缀
 * 将字符的频率作为节点权值创建哈夫曼树后, 进行前缀编码, 即可得到最优编码 
 */
std::unordered_map<char, std::string> prefixEncoding(TreeNode* root)
{
    std::unordered_map<char, std::string> encodingTable;
    std::function<void(TreeNode*, std::string)>
        encode = [&](TreeNode* root, std::string currentEncoding) -> void
    {
        if(!root->leftChild && !root->rightChild) // 叶节点, 确定编码
        {
            encodingTable[root->data] = currentEncoding;
        }
        if(root->leftChild) { encode(root->leftChild, currentEncoding+"0"); }
        if(root->rightChild) { encode(root->rightChild, currentEncoding+"1"); }
    };

    encode(root, "");
    return encodingTable;
}

void traverse(TreeNode* root)
{
    if(!root) { return; }
    traverse(root->leftChild);
    std::cout<< root-> weight << ' ';
    traverse(root->rightChild);
}

int main()
{
    TreeNode* root = new TreeNode(0, 'A');
    root->leftChild = new TreeNode(0, 'B');
    root->rightChild = new TreeNode(2, 'C');

    root->leftChild->leftChild = new TreeNode(4, 'D');
    root->leftChild->rightChild = new TreeNode(0, 'E');

    root->leftChild->rightChild->leftChild = new TreeNode(7, 'F');
    root->leftChild->rightChild->rightChild = new TreeNode(5, 'G');

    std::cout << getTreeWeightPathLength(root, 0) << std::endl;
    
    std::vector<TreeNode*> leafNodes = {new TreeNode(45, 'a'), new TreeNode(13, 'b'),
                                        new TreeNode(12, 'c'), new TreeNode(16, 'd'),
                                        new TreeNode(9, 'e'), new TreeNode(5, 'f')};
    root = buildHuffmanTree(leafNodes);
    traverse(root);
    std::cout << std::endl;
    std::unordered_map<char, std::string> encodingTable = prefixEncoding(root);
    for(auto pair : encodingTable)
    {
        std::cout << pair.first << "  <->  " << pair.second << std::endl; 
    }
}