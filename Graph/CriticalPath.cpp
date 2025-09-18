#include<iostream>
#include<vector>
#include<stack>
#include<queue>
using namespace std;

/**
 * 求关键路径, 先拓扑排序.
 * 这里的图中的边是带权的， 代表做一件事需要多长时间
 * 一件事能开始, 必须要所有依赖它的事做完
 * 这里假设所有图都是有向无环图, 在拓扑排序中不需要做错误检查, 并且只有一个源点和汇点
 */

void topologicalSortUtil(
    stack<int>& sortStack, 
    vector<bool>& isVisited, 
    vector<vector<int>> adjList[],
    int currentVertexIdx
)
{
    isVisited[currentVertexIdx] = true;
    for (vector<int> neighbor : adjList[currentVertexIdx])
    {
        if(!isVisited[neighbor[0]])
        {
            topologicalSortUtil(
                sortStack,
                isVisited,
                adjList,
                neighbor[0]
            );
        }
    }
    
    sortStack.push(currentVertexIdx);
    return;
}

vector<int> sortGraphTopological(vector<vector<int>> adjList[], int numVertices)
{
    stack<int> sortStack;
    vector<int> sortResult(numVertices, 0);
    vector<bool> isVisited(numVertices, false);
    for (int vertexIdx = 0; vertexIdx < numVertices; vertexIdx++)
    {
        if(!isVisited[vertexIdx])
        {
            topologicalSortUtil(sortStack, isVisited, adjList, vertexIdx);
        }
    }
    
    for (int i = 0; i < numVertices; i++)
    {
        int vertexIdx = sortStack.top();
        sortStack.pop();
        sortResult[i] = vertexIdx;
    }
    
    return sortResult;
}

vector<int> computeEarliestStartTime(vector<int>& topologicalSortResult, vector<vector<int>> adjList[], int& numVertices)
{
    // 由于做一件事之前, 要做完它的所有依赖, 所以一件事的最早开始时间取决于最大的 完成依赖的时间+依赖的最早开始时间
    vector<int> earliestStartTime(numVertices, 0);

    for (int& vertexIdx : topologicalSortResult)
    {
        for (vector<int>& neighbor : adjList[vertexIdx])
        {
            int neighborIdx = neighbor[0], weightToNeighbor = neighbor[1];
            earliestStartTime[neighborIdx] = max(
                earliestStartTime[neighborIdx],
                earliestStartTime[vertexIdx]+weightToNeighbor
            );
        }
    }
    
    return earliestStartTime;
}

vector<int> computeLatestStartTime(
    vector<int>& topologicalSortResult, 
    vector<vector<int>>& adjMatrix,
    int& finalVertexEarliestStartTime,
    int& numVertices)
{
    // 在保证后面的时间在其最迟发生时间之前能够发生时, 一件事最迟必须发生的时间
    vector<int> latestStartTime(numVertices, __INT_MAX__);
    latestStartTime[topologicalSortResult[numVertices-1]] = finalVertexEarliestStartTime;
    for (int i = numVertices-1; i >= 0; i--)
    {
        int& currentVertex = topologicalSortResult[i];
        for (int src = 0; src < numVertices; src++)
        {
            if(adjMatrix[src][currentVertex] != __INT_MAX__)
            {
                latestStartTime[src] = min(
                    latestStartTime[currentVertex]-adjMatrix[src][currentVertex],
                    latestStartTime[src]
                );
            }
        }
    }
    
    return latestStartTime;
}

/**
 * 找到所有关键点
 * 中途需要拓扑排序, 会将edgeList转换为adjList, 直接传入引用
 */
vector<bool> findCriticalVertices(
    vector<vector<int>>& edgeList,
    vector<vector<int>> adjList[],
    int& srcVertex,
    int& finalVertex,
    int numVertices)
{
    vector<vector<int>> adjMatrix(numVertices, vector<int>(numVertices, __INT_MAX__));
    auto convertEdgeList2AdjListAndAdjMatrix = [&]()
    {
        for(vector<int>& edge : edgeList)
        {
            int src = edge[0], dst = edge[1], weight = edge[2];
            adjList[src].push_back({dst, weight});
            adjMatrix[src][dst] = weight;
        }
    };

    convertEdgeList2AdjListAndAdjMatrix();
    vector<int> topologicalSortResult = sortGraphTopological(adjList, numVertices);
    vector<int> earliestStartTime = computeEarliestStartTime(topologicalSortResult, adjList, numVertices);
    vector<int> latestStartTime = computeLatestStartTime(
        topologicalSortResult, 
        adjMatrix, 
        earliestStartTime[numVertices-1],
        numVertices
    );
    srcVertex = topologicalSortResult[0];
    finalVertex = topologicalSortResult[numVertices-1];

    vector<bool> isCriticalVertex(numVertices, false); // 所有关键节点, 值为True表示为关键节点
    for (int i = 0; i < numVertices; i++)
    {
        if(!(latestStartTime[i] - earliestStartTime[i]))
        {
            isCriticalVertex[i] = true;
        }
    }
    
    return isCriticalVertex;
}

void findCriticalPathDFS(
    vector<vector<int>> adjList[],
    vector<bool> isCriticalVertex,
    vector<vector<int>>& allCriticalPaths,
    vector<int>& currentPath,
    int& maxSumWeight,
    int currentSumWeight,
    int currentVertex,
    int& finalVertexIdx
)
{
    currentPath.push_back(currentVertex);
    if(currentVertex == finalVertexIdx)
    {
        if(currentSumWeight >= maxSumWeight)
        {
            maxSumWeight = currentSumWeight;
            allCriticalPaths.push_back(currentPath);
        }
        return;
    }
    for (vector<int>& neighbor : adjList[currentVertex])
    {
        if(isCriticalVertex[neighbor[0]])
        {
            findCriticalPathDFS(
                adjList,
                isCriticalVertex,
                allCriticalPaths,
                currentPath,
                maxSumWeight,
                currentSumWeight + neighbor[1],
                neighbor[0],
                finalVertexIdx
            );
            currentPath.pop_back();
        }
    }
}

/**
 * 找到所有关键路径
 * 关键路径指的是从源点到汇点的所有路径中, 具有最大路径长度的路径
 * 关键路径上的所有点都是关键活动
 */
vector<vector<int>> findCriticalPaths(
    vector<vector<int>> edgeList, 
    int numVertices
)
{
    vector<vector<int>> adjList[numVertices];
    vector<int> currentPath;
    vector<vector<int>> allCriticalPaths;
    int srcVertex = -1, finalVertex = -1;
    vector<bool> isCriticalVertex = findCriticalVertices(
        edgeList, adjList, srcVertex,
        finalVertex, numVertices
    );

    int maxSumWeight = -1;
    findCriticalPathDFS(
        adjList, isCriticalVertex, allCriticalPaths, 
        currentPath, maxSumWeight, 0, srcVertex, finalVertex
    );

    return allCriticalPaths;
}

int main()
{
    vector<vector<int>> edgeList = {
        {0, 1, 3}, {0, 2, 2}, {1, 3, 2}, {2, 3, 4},
        {1, 4, 3}, {3, 5, 2}, {2, 5, 3}, {4, 5, 1}
    };

    vector<vector<int>> allCriticalPaths = findCriticalPaths(edgeList, 6);
    for (vector<int>& criticalPath : allCriticalPaths)
    {
        for (int& criticalPoint : criticalPath)
        {
            cout << criticalPoint << ' ';
        }
        cout << endl;
    }
    
    return 0;
}