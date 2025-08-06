#include <iostream>
#define elemType int

struct Node
{
    Node* next;
    elemType data;
};

struct LinkedQueue
{
    Node* front;
    Node* rear;
};

Node* initNewNode(elemType value)
{
    Node* newNode = (Node*)malloc(sizeof(Node));
    if(!newNode)
    {
        return NULL;
    }

    *newNode = {.next=NULL, .data=value};
    return newNode;
}

LinkedQueue* initLinkedQueue()
{
    LinkedQueue* queuePtr = (LinkedQueue*)malloc(sizeof(LinkedQueue));
    if(!queuePtr)
    {
        return NULL;
    }

    queuePtr->front = NULL;
    queuePtr->rear = NULL;
    return queuePtr;
}

bool isQueueEmpty(LinkedQueue* queuePtr)
{
    return queuePtr->front==NULL;
}

bool pushToQueue(LinkedQueue* queuePtr, elemType value)
{
    if(!queuePtr)
    {
        return false;
    }

    Node* newNode = initNewNode(value);
    if(!newNode)
    {
        return false;
    }

    if(isQueueEmpty(queuePtr)) // 空队列做特殊处理
    {
        queuePtr->front = newNode;
        queuePtr->rear = newNode;
        return true;
    }
    //非空队列的一般处理
    queuePtr->rear->next = newNode;
    queuePtr->rear = newNode;

    return true;
}

bool popFromQueue(LinkedQueue* queuePtr, elemType* bufferForPoppedElem)
{
    if(!queuePtr || isQueueEmpty(queuePtr) || !bufferForPoppedElem)
    {
        return false;
    }

    *bufferForPoppedElem = queuePtr->front->data;
    Node* deletePtr = queuePtr->front;
    queuePtr->front = queuePtr->front->next;
    if(queuePtr->front == NULL)
    {
        queuePtr->rear = NULL;
    } // 出队后如果队空, 需要修改rear

    free(deletePtr);

    return true;
}

void printQueue(LinkedQueue* queuePtr)
{
    if(isQueueEmpty(queuePtr))
    {
        printf("Empty Queue!\n");
        return;
    }

    printf("\n----------------Current Queue------------------\n");
    for (Node* front = queuePtr->front; front!=NULL ; front=front->next)
    {
        printf("%d ", front->data);
    }
    printf("\n-----------------------------------------------\n");
    
    return;
}

int main()
{
    LinkedQueue* queuePtr = initLinkedQueue();
    pushToQueue(queuePtr, 16);
    pushToQueue(queuePtr, 32);
    pushToQueue(queuePtr, 64);
    printQueue(queuePtr);
    elemType poppedElem;
    popFromQueue(queuePtr, &poppedElem);
    printf("%d\n", poppedElem);
    printQueue(queuePtr);
    
    return 0;
}

