#include<iostream>
#include<vector>
#include<stack>
#include<queue>
using namespace std;

/**
 * 在一个有向无环图中, 用顶点表示时间, 用边表示活动, 边上的权值表示一个活动需要多长时间
 * 求出这个图中的关键路径, 关键路径指的是从源点到汇点的所有路径中最长的那一个
 * 关键路径的长度就是完成整个工程的最短时间
 * 关键路径上的边(活动)为关键活动, 关键活动如果延期, 就会导致工程的延期
 * 关键活动如果提前完成, 不一定能加快工期
 * 这里假设所有输入的图都是有向无环图, 在拓扑排序中不需要做错误检查, 并且只有一个源点和汇点
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

vector<int> computeEventEarliestStartTime(vector<int>& topologicalSortResult, vector<vector<int>> adjList[], int& numVertices)
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

vector<int> computeEventLatestStartTime(
    vector<int>& topologicalSortResult, 
    vector<vector<int>>& adjMatrix,
    int& finalVertexEarliestStartTime,
    int& numVertices
)
{
    // 在保证后面的时间在其最迟发生时间之前能够发生时, 一件事最迟必须发生的时间
    vector<int> eventLatestStartTime(numVertices, __INT_MAX__);
    eventLatestStartTime[topologicalSortResult[numVertices-1]] = finalVertexEarliestStartTime;
    for (int i = numVertices-1; i >= 0; i--)
    {
        int& currentVertex = topologicalSortResult[i];
        for (int src = 0; src < numVertices; src++)
        {
            if(adjMatrix[src][currentVertex] != __INT_MAX__)
            {
                eventLatestStartTime[src] = min(
                    eventLatestStartTime[currentVertex]-adjMatrix[src][currentVertex],
                    eventLatestStartTime[src]
                );
            }
        }
    }
    
    return eventLatestStartTime;
}

vector<vector<int>> computeActivityEarliestStartTime(
    vector<int>& eventEarliestStartTime, 
    vector<vector<int>> adjList[],
    int& numVertices
)
{
    // 一个活动的最早发生时间就是这个活动的起点所代表的事件的最早发生时间
    vector<vector<int>> activityEarliestStartTime; // {{srcEvent, dstEvent, earliestStartTime}}
    for (int i = 0; i < numVertices; i++)
    {
        for (vector<int>& activity : adjList[i])
        {
            int srcEvent = i, dstEvent = activity[0];
            activityEarliestStartTime.push_back(
                {
                    srcEvent, dstEvent, 
                    eventEarliestStartTime[srcEvent]
                }
            );
        }   
    }
    
    return activityEarliestStartTime;
}

vector<vector<int>> computeActivityLatestStartTime(
    vector<int>& eventLatestStartTime, 
    vector<vector<int>> adjList[],
    int& numVertices
)
{
    // 一个活动的最晚发生时间指一个活动的终点所代表的事件的最晚发生时间与该活动的时间差
    vector<vector<int>> activityLatestStartTime; // {{srcEvent, dstEvent, earliestStartTime}}
    for (int i = 0; i < numVertices; i++)
    {
        for (vector<int>& activity : adjList[i])
        {
            int srcEvent = i, dstEvent = activity[0], activityTime = activity[1];
            activityLatestStartTime.push_back(
                {
                    srcEvent, dstEvent, 
                    eventLatestStartTime[dstEvent]-activityTime
                }
            );
        }   
    }
    
    return activityLatestStartTime;
}

/**
 * 找到所有关键活动
 * 中途需要拓扑排序, 会将edgeList转换为adjList, 直接传入引用
 */
vector<pair<int, int>> findCriticalActivities(
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
    vector<int> eventEarliestStartTime = computeEventEarliestStartTime(
        topologicalSortResult, 
        adjList, 
        numVertices
    );
    vector<int> eventLatestStartTime = computeEventLatestStartTime(
        topologicalSortResult, 
        adjMatrix, 
        eventEarliestStartTime[numVertices-1],
        numVertices
    );
    vector<vector<int>> activityEarliestStartTime = computeActivityEarliestStartTime(
        eventEarliestStartTime,
        adjList,
        numVertices
    );
    vector<vector<int>> activityLatestStartTime = computeActivityLatestStartTime(
        eventLatestStartTime,
        adjList,
        numVertices
    );
    srcVertex = topologicalSortResult[0];
    finalVertex = topologicalSortResult[numVertices-1];

    vector<pair<int, int>> criticalActivities; // 所有关键活动的起始事件和结束事件
    for (int i=0; i<activityEarliestStartTime.size(); i++)
    {
        vector<int> activityTuple1 = activityEarliestStartTime[i];
        vector<int> activityTuple2 = activityLatestStartTime[i];
        int srcEvent = activityTuple1[0],
            dstEvent = activityTuple1[1],
            _activityEarliestStartTime = activityTuple1[2],
            _activityLatestStartTime = activityTuple2[2];
        if(!(_activityLatestStartTime - _activityEarliestStartTime))
        {
            criticalActivities.push_back({srcEvent, dstEvent});
        }
    }
    
    return criticalActivities;
}

void findCriticalPathDFS(
    vector<vector<int>> adjList[],
    vector<vector<int>> criticalActivityTable, // criticalActivityTable[i][j]=True表示从i到j的活动为关键活动
    vector<vector<int>>& allCriticalPaths,
    vector<int>& currentPath,
    int currentVertex,
    int& finalVertexIdx
)
{
    if(currentVertex == finalVertexIdx)
    {
        currentPath.push_back(finalVertexIdx);
        allCriticalPaths.push_back(currentPath);
        currentPath.pop_back();
        return;
    }
    for (vector<int>& neighbor : adjList[currentVertex])
    {
        if(criticalActivityTable[currentVertex][neighbor[0]])
        {
            currentPath.push_back(currentVertex);
            findCriticalPathDFS(
                adjList,
                criticalActivityTable,
                allCriticalPaths,
                currentPath,
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
    vector<pair<int, int>> criticalActivities = findCriticalActivities(
        edgeList,
        adjList,
        srcVertex, finalVertex, numVertices
    );
    vector<vector<int>> criticalActivityTable(numVertices, vector<int>(numVertices, 0));
    for (pair<int, int>& criticalActivity : criticalActivities)
    {
        int srcEvent = criticalActivity.first, dstEvent = criticalActivity.second;
        criticalActivityTable[srcEvent][dstEvent] = true;
    }
    
    findCriticalPathDFS(
        adjList,
        criticalActivityTable,
        allCriticalPaths,
        currentPath,
        srcVertex,
        finalVertex
    );

    return allCriticalPaths;
}

void printAllCriticalPaths(vector<vector<int>>& allCriticalPaths)
{
    cout << "----- All Critical Path -----" << endl;
    for (vector<int>& criticalPath : allCriticalPaths)
    {
        for (int& criticalPoint : criticalPath)
        {
            cout << criticalPoint << ' ';
        }
        cout << endl;
    }
}

int main()
{
    vector<vector<int>> edgeList = {
        {0, 1, 3}, {0, 2, 2}, {1, 3, 2}, {2, 3, 4},
        {1, 4, 3}, {3, 5, 2}, {2, 5, 3}, {4, 5, 1}
    };

    vector<vector<int>> allCriticalPaths = findCriticalPaths(edgeList, 6);
    printAllCriticalPaths(allCriticalPaths);

    edgeList = {
        {0, 1, 3}, {0, 2, 8}, {2, 1, 4},
        {1, 3, 9}, {1, 4, 6}, {2, 4, 10},
        {3, 5, 6}, {4, 5, 9}
    };
    allCriticalPaths = findCriticalPaths(edgeList, 6);
    printAllCriticalPaths(allCriticalPaths);
    
    return 0;
}