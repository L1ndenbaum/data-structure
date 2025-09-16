#include<vector>
#include<iostream>
using namespace std;

/**
 * 并查集, 解决多个元素属于哪些不相交的集合问题
 * 每个集合都是一个树
 */
struct DisjointSet
{
    int* parent; // parent[i]指示元素i的上级是哪个元素, 最后会指向到一个集合的根
    int* rank; // 元素i的根代表的树的高度
    DisjointSet(int size)
    {
        parent = new int[size];
        rank = new int[size];
        for (int i = 0; i < size; i++)
        {
            parent[i] = i;
            rank[i] = 0;
        }
        
    }

    int findRoot(int i) // 找到元素i的根, 根即为一个集合的代表
    {
        if(parent[i] == i)
        { 
            return i; 
        }
        else 
        { 
            parent[i] = findRoot(parent[i]); // 路径压缩, 在找到集合的根时就更新元素的parent(但这样会破坏树形结构)
            return parent[i];
        }
    }

    void uniteSets(int i, int j) // 合并包含元素i 和 元素j 的两个集合
    {
        int iRoot = findRoot(i);
        int jRoot = findRoot(j);
        if(iRoot == jRoot) { return; }
        // 合并时, 总把高度低的树往高度高的树上合并, 以保证树高不增加(因为合并是进行在根节点的)
        // 高度相等, 选一个合并 并更新树高
        if(rank[iRoot] < rank[jRoot]) { parent[iRoot] = jRoot; }
        else if(rank[iRoot] > rank[jRoot]) { parent[jRoot] = iRoot; }
        else { parent[iRoot] = jRoot; rank[jRoot]++; }

        return;
    }

    ~DisjointSet()
    {
        delete[] parent;
        delete[] rank;
    }
};