#include <iostream>
using std::cout, std::endl;

class BTreeNode
{
public:
    int minDegree; // 每个非根节点最少有几个孩子(最多会有2*minDegree个孩子, 对应每个节点的最多key数量 2*minDegree-1)
    int numCurrentKeys; //当前节点中实际拥有的key数量
    int* keys; // 实际用到前numCurrentKeys个
    BTreeNode** children; // 实际用到前numCurrentKeys+1个
    bool isLeaf;
    BTreeNode(int _minDegree, bool _isLeaf)
    {
        minDegree = _minDegree;
        isLeaf = _isLeaf;
        keys = new int[2*_minDegree-1];
        children = new BTreeNode*[2*minDegree];
        numCurrentKeys = 0;
    }

    /**
     * 遍历以此节点为根的子树
     * 是中序遍历的推广
     */
    void traverse(); 

    /**
     * 当节点未满时, 向以此节点为根的子树中插入一个key
     */
    void insertNonFull(int key);

    /**
     * 为当前节点划分一个孩子splitedChild
     * 被分割的孩子splitedChild一定是满的
     */
    void splitChild(int splitedChildIdx, BTreeNode* splitedChild);

    /**
     * 在以此节点为根的子树中搜索key
     */
    BTreeNode* searchNode(int key);

    /**
     * 返回第一个 >=key 的key索引
     */
    int findKeyIdxGreaterOrEqual(int key);

    /**
     * Warpper Function, 删除以此节点为根的子树中 值等于key的键
     */
    void removeKeyFromNode(int key);

    /**
     * Ultility Function, 在以此节点为根的子树中 在叶子节点中删除key
     */
    void removeKeyFromLeafNode(int removedKeyIdx);

    /**
     * Ultility Function, 在以此节点为根的子树中, 在非叶子节点中删除key
     */
    void removeKeyFromNonLeafNode(int removedKeyIdx);

    /**
     * 找到在节点第idx个位置的key的前驱的key值
     */
    int getPredecessorKeyValue(int idx);

    /**
     * 找到在节点第idx个位置的key的后继的key值
     */
    int getSuccessorKeyValue(int idx);

    /**
     * 如果 children[childIdx] 的 key 数量少于 minDegree-1，
     * 通过借或合并让它至少拥有 minDegree 个 key
     */
    void fillChild(int childIdx);

    /**
     * 从children[childIdx]的前一个兄弟节点 借一个key过来
     * 并调整父节点和兄弟节点的结构
     */
    void borrowFromPrevChild(int childIdx);

    /**
     * 从children[childIdx]的后一个兄弟节点 借一个key过来
     *  并调整父节点和兄弟节点的结构
     */
    void borrowFromNextChild(int childIdx);
    
    /**
     * 将 children[childIdx]、父节点的 key[childIdx]、
     * 以及 children[childIdx+1] 合并为一个节点
     */
    void mergeChildren(int childIdx);
};

class BTree
{
public:
    BTreeNode* root;
    int minDegree;
    BTree(int _minDegree):root(nullptr),minDegree(_minDegree){}

    void traverse()
    {
        if(root) { root->traverse(); }
    }
    
    void insertNode(int key);

    void deleteNode(int key);
};

void BTreeNode::traverse()
{
    // 有n个key和n+1个children, 遍历n个key和最开始的n个孩子
    int currentKeyIdx;
    for (currentKeyIdx = 0; currentKeyIdx < this->numCurrentKeys; currentKeyIdx++)
    {
        // 此节点非叶, 在输出它前, 先遍历它的子树
        if(!this->isLeaf)
        {
            this->children[currentKeyIdx]->traverse();
        }
        cout << " " << keys[currentKeyIdx];
    }
    
    // 打印以最后一个孩子为根的子树 因为每个key夹在两个child中间
    if(!isLeaf)
    {
        this->children[currentKeyIdx]->traverse();
    }
}

BTreeNode* BTreeNode::searchNode(int key)
{
    int currentKeyIdx = 0; // 当前key
    while (currentKeyIdx<this->numCurrentKeys && key>this->keys[currentKeyIdx])
    {
        currentKeyIdx++;
    } // 定位到节点区间
    if(currentKeyIdx<this->numCurrentKeys && key==this->keys[currentKeyIdx])
    {
        return this;
    }
    if(this->isLeaf)
    {
        return nullptr;
    }
    
    return this->children[currentKeyIdx]->searchNode(key);
}

void BTreeNode::splitChild(int splitedChildIdx, BTreeNode* splitedChild)
{
    // 分割一个满了的孩子节点, 这个节点中有 2*minDegree-1个key
    // minDegree-1个键去左边, minDegree-1个键去右边, 剩下一个上浮
    // 创建一个能存储被分割的孩子的右半边minDegree-1个key的节点
    BTreeNode* newNode = new BTreeNode(
        splitedChild->minDegree, 
        splitedChild->isLeaf
    );
    newNode->numCurrentKeys = this->minDegree-1;
    
    // 把右半边minDegree-1个key从被分割的孩子复制到新节点
    // 这里, 满了的节点有2*minDegree-1个key, 所以minDegree就是右半边的第一个keyIdx
    for (int i = 0; i < this->minDegree-1; i++)
    {
        newNode->keys[i] = splitedChild->keys[i+minDegree];
    }
    
    // 不是叶子节点的话, 把最后的minDegree个孩子从被分割的孩子复制到新节点
    if(!splitedChild->isLeaf)
    {
        for (int i = 0; i < minDegree; i++)
        {
            newNode->children[i] = splitedChild->children[i+minDegree];
        }
    }

    // 减少被分裂的孩子中的key数量, 即更新左半边的key数
    splitedChild->numCurrentKeys = this->minDegree-1;
    // 在父节点的孩子指针数组中，为新生成的右孩子腾出一个插槽
    for (int i = this->numCurrentKeys; i >= splitedChildIdx+1; i--)
    {
        this->children[i+1] = this->children[i];
    }
    // 将新右孩子连接到这个节点
    this->children[splitedChildIdx+1] = newNode;

    // 在父节点的 key 数组中，从插入位置起将所有 key 右移一格，为上浮的中位数 key 腾出空位(也就是splitedChildIdx的位置)
    // 中位数key插入的位置是splitedChildIdx是因为, 这个key正好把原先的孩子分开为左右半边, 那么它上浮后, 它对应的左右就是splitedChild的左和新节点的右
    for (int i = this->numCurrentKeys-1; i >= splitedChildIdx; i--)
    {
        this->keys[i+1] = this->keys[i];
    }
    
    // 然后把被分裂孩子的中位数 key 插入到空出来的位置
    this->keys[splitedChildIdx] = splitedChild->keys[this->minDegree-1];
    // 这个节点的key数量多了一个
    this->numCurrentKeys++;
}

void BTreeNode::insertNonFull(int key)
{
    int currentKeyIdx = this->numCurrentKeys-1;
    if(this->isLeaf) // 是叶子直接找位置插
    {
        // 找到第一个<=key的位置, 从右往左移动比 key 大的元素，为新 key 腾出插入位置
        while (currentKeyIdx>=0 && keys[currentKeyIdx]>key)
        {
            keys[currentKeyIdx+1] = keys[currentKeyIdx];
            currentKeyIdx--;
        }
        keys[currentKeyIdx+1] = key;
        this->numCurrentKeys++;
    }
    else // 不是叶找孩子插
    {
        // 找到最后一个 <= key 的 key，下一步应递归到其右侧的子节点(currentKeyIdx+1)
        while (currentKeyIdx>=0 && keys[currentKeyIdx]>key)
        {
            currentKeyIdx--;
        }
        if(this->children[currentKeyIdx+1]->numCurrentKeys == 2*this->minDegree-1)
        {
            // 若目标孩子已满，先将其分裂
            // 分裂后中位数 key 上浮至父节点，并产生一个新的右孩子
            // 根据 key 的大小决定应递归到左孩子还是右孩子
            this->splitChild(currentKeyIdx+1, this->children[currentKeyIdx+1]);
            if(keys[currentKeyIdx+1] < key) { currentKeyIdx++; }
        }
        this->children[currentKeyIdx+1]->insertNonFull(key);
    }
}

int BTreeNode::findKeyIdxGreaterOrEqual(int key)
{
    int currentKeyIdx = 0;
    while (currentKeyIdx<this->numCurrentKeys && this->keys[currentKeyIdx] < key)
    {
        currentKeyIdx++;
    }
    
    return currentKeyIdx;
}

void BTreeNode::removeKeyFromLeafNode(int removedKeyIdx)
{
    // 把所有 >key的节点 往前挪一格
    for (int keyIdx = removedKeyIdx+1; keyIdx < this->numCurrentKeys; keyIdx++)
    {
        this->keys[keyIdx-1] = this->keys[keyIdx];
    }
    // 减少key的数量
    this->numCurrentKeys--;

    return;
}

int BTreeNode::getPredecessorKeyValue(int keyIdx)
{
    // keyIdx节点的前驱key, 就是它的左子树中的最大的key, 并且一定在一个叶子节点里
    BTreeNode* current = this->children[keyIdx];
    while (!current->isLeaf)
    {
        current = current->children[current->numCurrentKeys];
    }
    
    return current->keys[current->numCurrentKeys-1];
}

int BTreeNode::getSuccessorKeyValue(int keyIdx)
{
    // keyIdx节点的后继key, 就是它的右子树中的最小的key, 并且一定在一个叶子节点里
    BTreeNode* current = this->children[keyIdx+1];
    while (!current->isLeaf)
    {
        current = current->children[0];
    }

    return current->keys[0];
}

void BTreeNode::removeKeyFromNonLeafNode(int removedKeyIdx)
{
    // 不能直接删内部节点的key,否则会破坏顺序和平衡, 所以要用一个来自子树的合适key替代它, 再在下面删那个key
    int key = this->keys[removedKeyIdx];

    if(this->children[removedKeyIdx]->numCurrentKeys > this->minDegree) // 左孩子够胖, 从它身上借key
    {
        int predecessorKeyValue = this->getPredecessorKeyValue(removedKeyIdx);
        this->keys[removedKeyIdx] = predecessorKeyValue;
        this->children[removedKeyIdx]->removeKeyFromNode(predecessorKeyValue);
    }
    else if(this->children[removedKeyIdx+1]->numCurrentKeys > this->minDegree) // 右孩子够胖, 从它身上借key
    {
        int successorKeyValue = this->getSuccessorKeyValue(removedKeyIdx);
        this->keys[removedKeyIdx] = successorKeyValue;
        this->children[removedKeyIdx]->removeKeyFromNode(successorKeyValue);
    }
    else // 左右孩子都太瘦, 合并两个节点
    {
        this->mergeChildren(removedKeyIdx);
        this->children[removedKeyIdx]->removeKeyFromNode(key);
    }

    return;
}

void BTreeNode::removeKeyFromNode(int key)
{
    int removedKeyIdx = this->findKeyIdxGreaterOrEqual(key); // 找到被删除的
    // 要被删除的key在这个节点中
    if(removedKeyIdx<this->numCurrentKeys && this->keys[removedKeyIdx]==key)
    {
        if(this->isLeaf)
        {
            this->removeKeyFromLeafNode(removedKeyIdx);
        }
        else
        {
            this->removeKeyFromNonLeafNode(removedKeyIdx);
        }
    }
    else // 要被删除的key不在这个节点中 去子树中找
    {
        if(this->isLeaf) // 此节点是叶节点, 表明key不存在
        {
            cout<<"The key "<< key <<" is does not exist in the tree\n";
            return;
        }
        
        // 这个bool值标记key是否在最后一个孩子节点中
        // 因为在孩子中删除节点前会检查孩子的key数量，不够要向兄弟借或者合并兄弟
        // 如果目标key在最后一个孩子上, 那么它只能和左兄弟合并, 新形成的孩子下标会少1
        bool keyInLastChild = (removedKeyIdx==this->numCurrentKeys ? true : false);

        // 删除完一个key后, 一个节点的key数量最少为minDegree-1
        // 所以, 当从一个可能存在key的节点删除这个key之前, 要确保其至少有minDegree个key
        if(this->children[removedKeyIdx]->numCurrentKeys < this->minDegree)
        {
            this->fillChild(removedKeyIdx);
        }

        // 这里, 此版本的代码在keyInLastChild==true时，总会fillChild,所以直接用这个bool值做状态分支
        if(keyInLastChild)
        {
            this->children[removedKeyIdx-1]->removeKeyFromNode(key);
        }
        else
        {
            this->children[removedKeyIdx]->removeKeyFromNode(key);
        }
    }

    return;
}

void BTreeNode::fillChild(int childIdx)
{
    // 左兄弟足够胖, 可以从它身上拿出一个key来填充
    if(childIdx != 0 
       && this->children[childIdx-1]->numCurrentKeys >= this->minDegree)
    {
        this->borrowFromPrevChild(childIdx);
    }
    // 右兄弟足够胖, 可以从它身上拿出来一个key填充
    else if(childIdx != this->numCurrentKeys 
            && this->children[childIdx+1]->numCurrentKeys >= this->minDegree)
    {
        this->borrowFromNextChild(childIdx);
    }
    // 左右都太瘦没法借, 就merge两个孩子, 如果是最后一个孩子, 就和它的左兄弟合并, 否则和右兄弟合并
    else 
    {
        if(childIdx == this->numCurrentKeys)
        {
            this->mergeChildren(childIdx-1);
        }
        else
        {
            this->mergeChildren(childIdx);
        }
    }

    return;
}

void BTreeNode::borrowFromPrevChild(int childIdx)
{
    BTreeNode* child = this->children[childIdx];
    BTreeNode* leftSibling = this->children[childIdx-1];
    // 因为是从左兄弟借来的, 左兄弟的key都比自己的小, 所以借来的key放在第一个位置

    // 给借来的新key腾出地方
    for (int keyIdx = child->numCurrentKeys-1; keyIdx >= 0 ; keyIdx--)
    {
        child->keys[keyIdx+1] = child->keys[keyIdx];
    }
    // 如果children[childIdx]不是叶子, 由于key挪了一格, 孩子也要跟着动
    if(!child->isLeaf)
    {
        for (int keyIdx = child->numCurrentKeys; keyIdx >= 0 ; keyIdx--)
        {
            child->children[keyIdx+1] = child->children[keyIdx];
        }
    }
    
    // 从左兄弟借来的key要经过父节点中转
    // 原来父节点的keys[childIdx-1]正好在 leftSibling和child的中间
    // 现在child把它收为自己的最小key
    child->keys[0] = this->keys[childIdx-1];

    // child不是叶子, 还要把左兄弟的最后一个孩子给他挂上
    // 因为左兄弟的最后一个子树中, 所有keyd都比新的child->keys[0]小, 所以成为第一个孩子
    if(!child->isLeaf)
    {
        child->children[0] = leftSibling->children[leftSibling->numCurrentKeys];
    }

    // 父节点失去了原本的keys[childIdx-1], 要把左兄弟的最大key补给它
    this->keys[childIdx-1] = leftSibling->keys[leftSibling->numCurrentKeys-1];
    leftSibling->numCurrentKeys--;
    child->numCurrentKeys++;

    return;
}

void BTreeNode::borrowFromNextChild(int childIdx)
{
    BTreeNode* child = this->children[childIdx];
    BTreeNode* rightSibling = this->children[childIdx+1];

    // 追加借来的key到child为最大key
    child->keys[child->numCurrentKeys] = this->keys[childIdx];

    // child不是叶, 也把借来的key对应的孩子接上
    if(!child->isLeaf)
    {
        child->children[child->numCurrentKeys+1] = rightSibling->children[0];
    }
    // 父节点失去原本的key[childIdx], 把右兄弟的最小key补给他
    this->keys[childIdx] = rightSibling->keys[0];

    // 右兄弟失去了第一个key, 把所有key往左挪一个
    for (int keyIdx = 1; keyIdx < rightSibling->numCurrentKeys; keyIdx++)
    {
        rightSibling->keys[keyIdx-1] = rightSibling->keys[keyIdx];
    }
    
    // 右兄弟不是叶子, 也要把孩子往左挪一个
    if(!rightSibling->isLeaf)
    {
        for (int keyIdx = 1; keyIdx <= rightSibling->numCurrentKeys; keyIdx++)
        {
            rightSibling->children[keyIdx-1] = rightSibling->children[keyIdx]; 
        }
    }

    rightSibling->numCurrentKeys--;
    child->numCurrentKeys++;

    return;
}

void BTreeNode::mergeChildren(int childIdx)
{
    BTreeNode* child = this->children[childIdx];
    BTreeNode* rightSibling = this->children[childIdx+1];

    // 下放父节点的中间key到左孩子尾部, 追加右兄弟的key
    child->keys[child->numCurrentKeys] = this->keys[childIdx];
    for (int keyIdx = 0; keyIdx < rightSibling->numCurrentKeys; keyIdx++)
    {
        child->keys[keyIdx+1+child->numCurrentKeys] = rightSibling->keys[keyIdx];
    }
    // child不是叶, 要把右兄弟的孩子挪到这上面
    if(!child->isLeaf)
    {
        for (int keyIdx = 0; keyIdx < rightSibling->numCurrentKeys; keyIdx++)
        {
            child->children[keyIdx+child->numCurrentKeys] = rightSibling->children[keyIdx];
        }
    }
    
    // 父节点少了一个key, 所有key往左一格
    for (int keyIdx = childIdx+1; keyIdx < this->numCurrentKeys; keyIdx++)
    {
        this->keys[keyIdx-1] = this->keys[keyIdx];
    }
    // 父节点的孩子也要挪, 因为childIdx和childIdx+1两个孩子合并了, 所以从childIdx+2开始
    for (int keyIdx = childIdx+2; keyIdx < this->numCurrentKeys; keyIdx++)
    {
        this->children[keyIdx-1] = this->children[keyIdx];
    }
    
    child->keys += rightSibling->numCurrentKeys+1;
    this->numCurrentKeys--;

    // 右兄弟现在不用了, 释放
    delete rightSibling;
    return;
}

void BTree::insertNode(int key)
{
    if(!this->root) // 树是空的, 初始化根和其中维护的变量
    {
        root = new BTreeNode(this->minDegree, true);
        root->keys[0] = key;
        root->numCurrentKeys = 1;
    }
    else // 树不是空的
    {
        if(this->root->numCurrentKeys == 2*minDegree-1) // 根是满的, 树要长高一层
        {
            // 分配一个新根, 并且把旧根当作新根的孩子
            BTreeNode* newRoot = new BTreeNode(minDegree, false);
            newRoot->children[0] = this->root;
            // 旧根是满的, 在新根上触发孩子(旧根)的节点分裂, 分割旧的根为左右两个节点, 移动1个key到新根
            newRoot->splitChild(0, newRoot->children[0]);

            // 现在新根有两个孩子, 决定哪个孩子拥有新的key
            int currentKeyIdx = 0;
            if(newRoot->keys[0] < key) { currentKeyIdx++; }
            newRoot->children[currentKeyIdx]->insertNonFull(key);

            // 更新树根
            this->root = newRoot;
        }
        else // 根未满 直接插入
        {
            this->root->insertNonFull(key);
        }
    }
}

void BTree::deleteNode(int key)
{
    if(!this->root)
    {
        cout << "B Tree is empty" << endl;
        return;
    }

    
    this->root->removeKeyFromNode(key);
    if(root->numCurrentKeys == 0) // 删除了key后, root没有key了, 让第一个孩子做新的root
    {
        BTreeNode* temp = this->root;
        if(this->root->isLeaf)
        {
            this->root = nullptr;
        }
        else
        {
            this->root = this->root->children[0];
        }
        delete temp;
    }

    return;
}

int main()
{
    BTree t(3); // A B-Tree with minimum degree 3

    t.insertNode(10);
    t.insertNode(5);
    t.insertNode(15);
    t.insertNode(2);
    t.insertNode(7);
    t.insertNode(12);
    t.insertNode(20);

    cout << "Before Deletion:";
    t.traverse();
    cout << endl;

    t.deleteNode(5);
    cout << "After Deletion:";
    t.traverse();
    cout << endl;

    return 0;
}