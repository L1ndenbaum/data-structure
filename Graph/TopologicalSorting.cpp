#include<vector>
#include<stack>
#include<queue>
#include<functional>
#include<iostream>
using namespace std;

/**
 * 拓扑排序, 是对一个有向无环图(DAG)排序
 * 排出来的顺序可理解为先后做某事的顺序, 
 * 一个有向无环图表示节点间的某些依赖关系,
 * 具有依赖的节点不能在被依赖节点前被执行
 * 从入度为0的节点执行
 * 返回 {-1} 说明有环
 */

vector<int> sortTopologicalDFS(vector<vector<int>>& adjList, int numVertices)
{
    stack<int> sortStack; // 最后全出栈即得到结果
    vector<int> visitState(numVertices, 0); // 节点访问状态: 0未访问, 1已访问但在递归栈中, 2已经处理完
    bool hasCycle;
    function<void(int)> sortDFS = [&](int currentVertexIdx)->void
    {
        if(hasCycle) { return; }
        visitState[currentVertexIdx] = 1;
        for (int adjVertexIdx : adjList[currentVertexIdx])
        {
            if(!visitState[adjVertexIdx])
            {
                sortDFS(adjVertexIdx);
            }
            else if(visitState[adjVertexIdx] == 1)
            {
                hasCycle = true;
                return;
            }
        };
        
        visitState[currentVertexIdx] = 2; // 在邻居都处理完了, 即依赖这个节点的节点处理完 这个节点就可以处理了
        sortStack.push(currentVertexIdx);
    };
    
    for (int vertexIdx = 0; vertexIdx < numVertices; vertexIdx++)
    {
        if(!visitState[vertexIdx])
        {
            sortDFS(vertexIdx);
        }
    }
    if(hasCycle) { return {-1}; }
    
    vector<int> sortResult;
    while (!sortStack.empty())
    {
        sortResult.push_back(sortStack.top());
        sortStack.pop();
    }
    
    return sortResult;
}

vector<int> sortTopologicalBFS(vector<vector<int>>& adjList, int numVertices) // Kahn's 
{
    vector<int> inDegree(numVertices, 0); // 每个节点的入度
    queue<int> vertexQueue;
    for (int i = 0; i < numVertices; i++)
    {
        for (int& neighborVertexIdx : adjList[i])
        {
            inDegree[neighborVertexIdx]++;
        }
    }

    for (int i = 0; i < numVertices; i++)
    {
        if(!inDegree[i]) { vertexQueue.push(i); }
    }
    
    vector<int> sortResult;
    bool hasCycle = false;
    while (!vertexQueue.empty())
    {
        int currentVertex = vertexQueue.front();
        sortResult.push_back(currentVertex);
        vertexQueue.pop();
        for (int neighborVertexIdx : adjList[currentVertex])
        {
            inDegree[neighborVertexIdx]--;
            if(!inDegree[neighborVertexIdx])
            {
                vertexQueue.push(neighborVertexIdx);
            }
        }
    }
    
    if(sortResult.size() != numVertices) // 有环 在一个环中, 所有节点的入度永远无法变成1, 大家相互锁死
    {
        return {-1};
    }
    else
    {
        return sortResult;
    }
}

int main()
{
    int numVertices = 6;
    vector<vector<int>> edges = {{2, 3}, {3, 1}, {4, 0}, {4, 1}, {5, 0}, {5, 2}};
    vector<vector<int>> adjList(numVertices);
    function<void()> constructAdjList = [&]()->void
    {
        for (vector<int>& edge : edges)
        {
            int src = edge[0], dst = edge[1];
            adjList[src].push_back(dst); 
        }
        return;
    } ;
    constructAdjList();

    vector<int> sortResult = sortTopologicalDFS(adjList, numVertices);

    for (auto node : sortResult)
    {
        cout << node << " ";
    }
    cout << endl;

    edges = {{0, 1}, {1, 2}, {2, 3},
           {3,1}};

    for (vector<int>& vertexAdj : adjList)
    {
        vertexAdj.clear();
    }
    
    constructAdjList();
    vector<int> result = sortTopologicalBFS(adjList, numVertices);

    for (auto i : result) {
        cout << i << " ";
    } 

    return 0;
}