#include<iostream>
#include<vector>
#define vertexType int
struct GraphByAdjacentMatrix
{
    int numVertices;
    vertexType* vertices;
    int** adjMatrix;
    GraphByAdjacentMatrix(int numVertices, std::vector<vertexType> vertices)
    {
        this->vertices = new vertexType[numVertices];
        this->numVertices = numVertices;
        for(int i=0; i<vertices.size(); i++)
        {
            this->vertices[i] = vertices[i];
        }

        adjMatrix = new int*[numVertices];
        adjMatrix[0] = new int[numVertices*numVertices];
        for (int i = 1; i < numVertices; i++)
        {
            adjMatrix[i] = adjMatrix[0] + i*numVertices;
        }
    };

    void addEdge(int src, int dst, int weight)
    {
        adjMatrix[src][dst] = weight;
    }

    ~GraphByAdjacentMatrix()
    {
        delete[] adjMatrix[0];
        delete[] adjMatrix;
    };
};
