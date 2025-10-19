#include<vector>
#include<iostream>
#include<algorithm>
#include<queue>
#include<functional>
#include"../Tree/DisjointSet.cpp"
using namespace std;

/**
 * 最小生成树的重要性质：
 * 对于无向带权图G={V,E}, U为顶点集V的非空子集.
 * 如果(u,v)是一条具有最小权值的边, 其中 u in U && v in V-U,
 * 那么一定存在一颗包含边(u,v)的最小生成树.
 */

/**
 * 最小生成树的Prim算法
 * 这个算法依赖于顶点数而不依赖于边数, 适合在边稠密的图中使用
 * 时间复杂度O(V^2)
 */
vector<vector<int>> getPrimMST(vector<vector<int>>& adjMatrix)
{
    int numVertices = adjMatrix.size();
    vector<vector<int>> mstEdgeList;
    vector<int> parents(numVertices);
    vector<int> minExpandCost(numVertices, __INT_MAX__);
    vector<bool> isInMST(numVertices, false);
    
    /**
     * minExpandCost[i]表示从当前的MST中的某个点到任意一个不在MST的顶点i的最小权值
     * 也就是说, 扩展一个顶点到MST中的最小代价
     */
    minExpandCost[0] = 0;
    auto getMinminExpandCost = [&]() -> int // 寻找一个从U到V-U中最小的边(u,v), 返回顶点索引vertexV
    {
        int min = __INT_MAX__;
        int minIdx;
        for (int vertexV = 0; vertexV < isInMST.size(); vertexV++)
        {
            if(!isInMST[vertexV] && minExpandCost[vertexV]<min)
            {
                min = minExpandCost[vertexV];
                minIdx = vertexV;
            }
        }
        
        return minIdx;
    };
    for (int cnt = 0; cnt<numVertices-1; cnt++) // MST一共有numVertices-1条边
    {
        int vertexU = getMinminExpandCost();
        isInMST[vertexU] = true; // 寻找并扩展当前代价最小的顶点U
        for (int vertexV = 0; vertexV < numVertices; vertexV++)
        {
            if(adjMatrix[vertexU][vertexV] && 
               !isInMST[vertexV] && adjMatrix[vertexU][vertexV] < minExpandCost[vertexV])
            {
                parents[vertexV] = vertexU;
                minExpandCost[vertexV] = adjMatrix[vertexU][vertexV]; //根据当前已知信息, 从u到v代价更小, 那么更新minExpandCost
            }
        }
    }

    for (int i = 1; i < numVertices; i++)
    {
        mstEdgeList.push_back({parents[i], i, adjMatrix[i][parents[i]]});
    }

    return mstEdgeList;
}


/**
 * 优化版本的最小生成树的Prim算法
 * 这里使用边集数组 edgeList 来存储图, 
 * edgeList[i] = {vertexU, vertexV, weight}
 * 这里使用了优先队列, 时间复杂度是O((V+E)logV)
 */
vector<vector<int>> getPrimMST(int numVertices, vector<vector<int>>& edgeList)
{
    vector<vector<int>> mstEdgeList;
    vector<vector<int>> adjList[numVertices];
    for (int i = 0; i < edgeList.size(); i++)
    {
        int vertexU = edgeList[i][0];
        int vertexV = edgeList[i][1];
        int weight = edgeList[i][2];
        adjList[vertexU].push_back({vertexV, weight});
        adjList[vertexV].push_back({vertexU, weight});
    }

    priority_queue<
        pair<int, pair<int, int>>, 
        vector<pair<int, pair<int,int>>>, 
        greater<pair<int, pair<int,int>>>
    > pq;
    vector<bool> isInMST(numVertices, false);
    pq.push({0, {-1, 0}}); // {weight, {vertexU, vertexV}}, -1为起点

    while (!pq.empty())
    {
        pair<int, pair<int, int>> top = pq.top();
        pq.pop();
        
        int vertexV = top.second.second; // 这个顶点还没有被纳入生成树
        if(isInMST[vertexV]) { continue; }
        int vertexU = top.second.first; // 这个顶点已经在生成树的顶点集合里了
        int weight = top.first; // 按weight排的小顶堆优先队列
        
        isInMST[vertexV] = true;
        if(vertexU != -1)
        {
            mstEdgeList.push_back({vertexU, vertexV, weight});
        }

        for(vector<int>& neighbor : adjList[vertexV]) // 扩展vertexV的邻居, 提议下一个可能被纳入MST的顶点
        {
            if(!isInMST[neighbor[0]])
            {
                pq.push({neighbor[1], {vertexV, neighbor[0]}});
            }
        }
    }

    return mstEdgeList;
}

/**
 * Kruskal的最小生成树算法
 * 时间复杂度O(ElogE) 包括排序O(ElogE), 并查集合并O(\alpha(V))
 * 适用于边稀疏而顶点多的图
 */
vector<vector<int>> getKruskalMST(int numVertices, vector<vector<int>>& edgeList)
{
    auto edgeCompare = [](vector<int>& edge1, vector<int>& edge2)->bool
    {
        return edge1[2] < edge2[2]; // 权值小的边优先
    };

    sort(edgeList.begin(), edgeList.end(), edgeCompare);
    vector<vector<int>> mstEdgeList;
    DisjointSet* disjointSet = new DisjointSet(numVertices);
    int numEdgesInMST = 0;
    for (vector<int>& edge : edgeList)
    {
        int vertexU = edge[0], vertexV = edge[1], weight = edge[2];
        if(disjointSet->findRoot(vertexU) == disjointSet->findRoot(vertexV))
        {
            continue;
        }
        disjointSet->uniteSets(vertexU, vertexV);
        mstEdgeList.push_back({vertexU, vertexV, weight});
        numEdgesInMST++;
        if(numEdgesInMST+1 == numVertices) { break; }
    }
    
    return mstEdgeList;
}

void printMST(vector<vector<int>> mstEdgeList)
{
    cout<< "Edge\tWeight" <<endl;
    for (vector<int>& edge : mstEdgeList)
    {
        cout << edge[0] << "<->" << edge[1] << "\t" << edge[2] <<endl;
    }
}

int main()
{
    vector<vector<int>> edgeList;
    edgeList.push_back({0, 1, 6});
    edgeList.push_back({0, 3, 5});
    edgeList.push_back({1, 4, 3});
    edgeList.push_back({4, 5, 6});
    edgeList.push_back({3, 5, 2});
    edgeList.push_back({0, 2, 1});
    edgeList.push_back({3, 2, 5});
    edgeList.push_back({5, 2, 4});
    edgeList.push_back({4, 2, 6});
    edgeList.push_back({1, 2, 5});
    vector<vector<int>> mstEdgeList = getPrimMST(6, edgeList);
    printMST(mstEdgeList);

    vector<vector<int>> adjMatrix = {
        {0,6,1,5,0,0},
        {6,0,5,0,3,0},
        {1,5,0,5,6,4},
        {5,0,5,0,0,2},
        {0,3,6,0,0,6},
        {0,0,4,2,6,0},
    };
    mstEdgeList = getPrimMST(adjMatrix);
    printMST(mstEdgeList);

    mstEdgeList = getKruskalMST(6, edgeList);
    printMST(mstEdgeList);

    return 0;
}