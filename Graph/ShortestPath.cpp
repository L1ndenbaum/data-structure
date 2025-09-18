#include<vector>
#include<queue>
#include<iostream>
#define INF __INT_MAX__
using namespace std;


/**
 * Dijkstra的单源最短路径(从一个点到图上其他点的最短路径)算法
 * 使用了优先队列, 时间复杂度O(ElogV)
 */
vector<int> getDijkstraShortestPath(vector<vector<int>>& edgeList, int numVertices, int srcVertex)
{
    auto compareEdges = [](vector<int>& edge1, vector<int>& edge2)->bool
    {
        return edge1[0] > edge2[0]; // 小顶堆, 把权值小的边放上面
    };

    vector<vector<int>> adjList[numVertices]; // adjList[Vertex] = {{next, weight}}
    auto constructAdjList = [&]()
    {
        for (vector<int>& edge : edgeList)
        {
            int src = edge[0], dst = edge[1], weight = edge[2];
            adjList[src].push_back({dst, weight});
        }
    };
    constructAdjList();

    // 优先队列的元素为 {src到某节点的距离, 某节点}
    priority_queue<vector<int>, vector<vector<int>>, decltype(compareEdges)> pq(compareEdges);
    vector<int> minDistances(numVertices, INF); // distance[i]为src到顶点i的距离
    pq.push({0, srcVertex});
    minDistances[srcVertex] = 0;
    while (!pq.empty())
    {
        int currentVertex = pq.top()[1];
        pq.pop();

        for(vector<int>& neighbor : adjList[currentVertex])
        {
            int nextVertex = neighbor[0], weight = neighbor[1];
            if(minDistances[nextVertex] > minDistances[currentVertex]+weight)
            {
                minDistances[nextVertex] = minDistances[currentVertex]+weight;
                pq.push({minDistances[nextVertex], nextVertex});
            }
        }
    }
    
    return minDistances;
}

/**
 * Floyd的多源最短路径(找到所有点到其他所有点的最短路径)算法
 * 时间复杂度O(V^3)
 */
vector<vector<int>> getFloydShortestPath(vector<vector<int>>& adjMatrix)
{
    vector<vector<int>> minDistances = adjMatrix;
    int numVertices = adjMatrix.size();

    // 外层选择一个搭桥的中间节点, 看所有节点借助这个节点到其他所有点的距离是不是更小
    // 外层循环执行一次, 就获得了更多的拓展方式信息
    for (int passbyVertexIdx = 0; passbyVertexIdx < numVertices; passbyVertexIdx++)
    {
        for (int srcVertexIdx = 0; srcVertexIdx < numVertices; srcVertexIdx++)
        {
            for (int dstVertexIdx = 0; dstVertexIdx < numVertices; dstVertexIdx++)
            {
                if(minDistances[srcVertexIdx][passbyVertexIdx]!=INF && minDistances[passbyVertexIdx][dstVertexIdx]!=INF)
                {
                    minDistances[srcVertexIdx][dstVertexIdx] = min(
                        minDistances[srcVertexIdx][dstVertexIdx], 
                        minDistances[srcVertexIdx][passbyVertexIdx]+minDistances[passbyVertexIdx][dstVertexIdx]
                    );
                }
            }
        }
    }
    
    return minDistances;
}

int main()
{
    vector<vector<int>> edgeList = {
        {0,1,10}, {1,2,1}, {0,4,5}, {4,3,2}, {3,2,6},
        {3,0,7}, {1,4,2}, {4,1,3}, {4,2,9}, {2,3,4}
    };
    vector<int> minDistancesOfVertex0 = getDijkstraShortestPath(edgeList, 5, 0);
    for(int& minDistance : minDistancesOfVertex0)
    {
        cout<< minDistance << ' ';
    }
    cout<<endl;

    vector<vector<int>> dist = {
        {0, 4, INF, 5, INF},
        {INF, 0, 1, INF, 6},
        {2, INF, 0, 3, INF},
        {INF, INF, 1, 0, 2},
        {1, INF, INF, 4, 0}
    };

    vector<vector<int>> minDistances = getFloydShortestPath(dist);
    for(int i = 0; i<minDistances.size(); i++) {
        for(int j = 0; j<minDistances.size(); j++) {
            cout<<minDistances[i][j]<<" ";
        }
        cout<<endl;
    }
    return 0;
}