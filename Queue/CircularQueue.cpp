#include <iostream>
#define MAX_SIZE 50
#define elemType int

struct CircularQueue
{
    int maxSize;
    int currentSize;
    int front;
    elemType* data;
};

CircularQueue* initCircularQueue(int maxSize)
{
    CircularQueue* queuePtr = (CircularQueue*)malloc(sizeof(CircularQueue));
    if(!queuePtr)
    {
        return NULL;
    }

    queuePtr->maxSize = maxSize;
    queuePtr->currentSize = 0;
    queuePtr->front = 0;
    queuePtr->data = (elemType*)malloc(sizeof(elemType) * maxSize);
    return queuePtr;
}

bool isQueueEmpty(CircularQueue* queuePtr)
{
    return queuePtr->currentSize == 0;
}

bool isQueueFull(CircularQueue* queuePtr)
{
    return queuePtr->currentSize == queuePtr->maxSize;
}

bool getFrontElem(CircularQueue* queuePtr, elemType* bufferForElem)
{
    if(!queuePtr || isQueueEmpty(queuePtr) || !bufferForElem)
    {
        return false;
    }

    *bufferForElem = queuePtr->data[queuePtr->front];
    return true;
}

bool getRearElem(CircularQueue* queuePtr, elemType* bufferForElem)
{
    if(!queuePtr || isQueueEmpty(queuePtr) || !bufferForElem)
    {
        return false;
    }

    int rear = (queuePtr->front+queuePtr->currentSize - 1) % queuePtr->maxSize; // 这个rear指向的位置有数, 直接返回
    *bufferForElem = queuePtr->data[rear];

    return true;
}

bool pushToQueue(CircularQueue* queuePtr, elemType value)
{
    if(isQueueFull(queuePtr))
    {
        return false;
    }

    int rear = (queuePtr->front+queuePtr->currentSize) % queuePtr->maxSize; // 这个rear指向的位置没有数, 需要被赋值
    queuePtr->data[rear] = value;
    queuePtr->currentSize++;
    return true;
}

bool popFromQueue(CircularQueue* queuePtr, elemType* bufferForPoppedElem)
{
    if(!queuePtr || isQueueEmpty(queuePtr) || !bufferForPoppedElem)
    {
        return false;
    }

    *bufferForPoppedElem = queuePtr->data[queuePtr->front];
    queuePtr->front = (queuePtr->front+1) % queuePtr->maxSize;
    queuePtr->currentSize--;
    return true;
}

void printQueue(CircularQueue* queuePtr)
{
    int rear = (queuePtr->front + queuePtr->currentSize-1) % queuePtr->maxSize;
    printf("\n----------------Current Queue------------------\n");
    for (int i = queuePtr->front; i <= rear; i=(i+1)%queuePtr->maxSize)
    {
        printf("%d ", queuePtr->data[i]);
    }
    printf("\n-----------------------------------------------\n");
}

int main()
{
    CircularQueue* queuePtr = initCircularQueue(40);
    printf("Empty: %d\n", isQueueEmpty(queuePtr));
    bool opSuccess = pushToQueue(queuePtr, 10);
    printf("Success: %d\n", opSuccess);
    opSuccess = pushToQueue(queuePtr, 20);
    printf("Success: %d\n", opSuccess);
    opSuccess = pushToQueue(queuePtr, 40);
    printf("Success: %d\n", opSuccess);
    opSuccess = pushToQueue(queuePtr, 60);
    printf("Success: %d\n", opSuccess);
    elemType poppedElem;
    opSuccess = popFromQueue(queuePtr, &poppedElem);
    printQueue(queuePtr);

    return 0;
}

