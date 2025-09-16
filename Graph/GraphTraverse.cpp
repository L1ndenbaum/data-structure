#include"Storage.cpp"
#include<iostream>
#include<vector>
#include<queue>
#include<functional>

void traverseGraphBFS(GraphByAdjacentMatrix* graph, int startNodeIdx)
{
    std::function<void(int)> visit =
    [&] (int idx) -> void
    {
        std::cout<< graph->vertices[idx]<< ' ';
    };

    bool* visited = new bool[graph->numVertices];
    visit(startNodeIdx);
    visited[startNodeIdx] = true;
    std::queue<int> queue;
    queue.push(startNodeIdx);
    while (!queue.empty())
    {
        int currentSrc = queue.front();
        queue.pop();
        for(int neighbor=0; neighbor<graph->numVertices; neighbor++)
        {
            if(!visited[neighbor] && graph->adjMatrix[currentSrc][neighbor])
            {
                visit(neighbor);
                visited[neighbor] = true;
                queue.push(neighbor);
            }
        }
    }
    
    return;
}

void traverseGraphDFS(GraphByAdjacentMatrix* graph, int startNodeIdx, bool* visited)
{
    std::cout<< graph->vertices[startNodeIdx]<< ' ';
    visited[startNodeIdx] = true;
    for (int neighbor = 0; neighbor < graph->numVertices; neighbor++)
    {
        if(!visited[neighbor] && graph->adjMatrix[startNodeIdx][neighbor])
        {
            traverseGraphDFS(graph, neighbor, visited);
        }
    }
    
    return;
}

int main()
{
    GraphByAdjacentMatrix* graph = new GraphByAdjacentMatrix(6, std::vector<int>{5, 1, 2, 6, 7, 3});
    graph->addEdge(0, 1, 1); graph->addEdge(1, 0, 1);
    graph->addEdge(1, 2, 1); graph->addEdge(2, 1, 1);
    graph->addEdge(3, 2, 1); graph->addEdge(2, 3, 1);
    graph->addEdge(4, 3, 1); graph->addEdge(3, 4, 1);
    graph->addEdge(3, 5, 1); graph->addEdge(5, 3, 1);
    graph->addEdge(4, 5, 1); graph->addEdge(5, 4, 1);

    traverseGraphBFS(graph, 0);

    std::cout<<std::endl;

    bool* visited = new bool[graph->numVertices];
    traverseGraphDFS(graph, 0, visited);
}