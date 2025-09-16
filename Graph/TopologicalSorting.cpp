#include<vector>
#include<iostream>
using namespace std;

/**
 * 拓扑排序, 是对一个有向无环图排序
 * 排出来的顺序可理解为先后做某事的顺序, 
 * 一个有向无环图表示节点间的某些依赖关系,
 * 具有依赖的节点不能在被依赖节点前被执行
 */

vector<int> sortTopological(vector<vector<int>> adjList)
{
    auto sortDFS = [&]()
    {
        
    }
}