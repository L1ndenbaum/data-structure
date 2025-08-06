#include <iostream>
#define elemType int

struct SequenceQueue
{
    int maxSize;
    int front;
    int rear;
    elemType* data;
};

SequenceQueue* createSequenceQueue(int maxSize)
{
    SequenceQueue* queue = (SequenceQueue*)malloc(sizeof(SequenceQueue));
    if(queue)
    {
        queue->front = 0;
        queue->rear = -1;
        queue->maxSize = maxSize;
        queue->data = (elemType*)malloc(sizeof(elemType) * maxSize);
        return queue;
    }
    else
    {
        return NULL;
    }
}

bool isQueueEmpty(SequenceQueue* queuePtr)
{
    return queuePtr->front > queuePtr->rear;
}

bool isQueueFull(SequenceQueue* queuePtr)
{
    return queuePtr->rear == queuePtr->maxSize;
} // 存在伪溢出问题

void pushToQueue(SequenceQueue* queuePtr, elemType value)
{
    if(isQueueFull(queuePtr))
    {
        return;
    }

    queuePtr->data[++queuePtr->rear] = value;
    return;
}

void popFromQueue(SequenceQueue* queuePtr)
{
    if(isQueueEmpty(queuePtr))
    {
        return;
    }

    queuePtr->front++;
    return;
}

void getFrontElem(SequenceQueue* queuePtr, elemType* bufferForElem)
{
    if(isQueueEmpty(queuePtr) || !bufferForElem)
    {
        return;
    }

    *bufferForElem = queuePtr->data[queuePtr->front];
    return;
}

void printQueue(SequenceQueue* queuePtr)
{
    printf("\n----------------Current Queue------------------\n");
    for (int i = queuePtr->front; i <= queuePtr->rear; i++)
    {
        printf("%d ", queuePtr->data[i]);
    }
    printf("\n-----------------------------------------------\n");
}

int main() {
    struct SequenceQueue* queue = createSequenceQueue(100);
    pushToQueue(queue, 1);
    pushToQueue(queue, 2);
    pushToQueue(queue, 3);
    
    elemType frontElem;
    getFrontElem(queue, &frontElem);
    printf("%d\n", frontElem);
    popFromQueue(queue);
    pushToQueue(queue, 4);
    printQueue(queue);
    
    return 0;
}
