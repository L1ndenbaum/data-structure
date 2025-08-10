#include <iostream>
#define elemType int

// 双链表实现的Deque
struct Node
{
    Node* prev;
    Node* next;
    elemType data;
};

struct LinkedDeque
{
    Node* front;
    Node* rear;
    int size;
};

Node* initNewNode(Node* prev, Node* next, elemType value)
{
    Node* newNode = (Node*)malloc(sizeof(Node));
    if(!newNode)
    {
        return NULL;
    }

    *newNode = {.prev=prev, .next=next, .data=value};
    return newNode;
}

LinkedDeque* initDeque()
{
    LinkedDeque* deque = (LinkedDeque*)malloc(sizeof(LinkedDeque));
    if(!deque)
    {
        return NULL;
    }

    deque->front = NULL;
    deque->rear = NULL;
    deque->size = 0;
    return deque;
}

bool isDequeEmpty(LinkedDeque* deque)
{
    return deque->front==NULL && deque->rear==NULL;
}

int getDequeSize(LinkedDeque* deque)
{
    return deque->size;
}

bool pushToFront(LinkedDeque* deque, elemType value)
{
    if(!deque)
    {
        return false;
    }

    Node* newNode = initNewNode(NULL, deque->front, value);
    if(isDequeEmpty(deque))
    {
        deque->front = newNode;
        deque->rear = newNode;
    }
    else
    {
        deque->front->prev = newNode;
        deque->front = newNode;
    }
    deque->size++;
    return true;
}

bool pushToRear(LinkedDeque* deque, elemType value)
{
    if(!deque)
    {
        return false;
    }

    Node* newNode = initNewNode(deque->rear, NULL, value);
    if(isDequeEmpty(deque))
    {
        deque->front = newNode;
        deque->rear = newNode;
    }
    else
    {
        deque->rear->next = newNode;
        deque->rear = newNode;
    }
    deque->size++;
    return true;
}

bool popFromFront(LinkedDeque* deque)
{
    if(!deque || isDequeEmpty(deque))
    {
        return false;
    }
    
    Node* deletePtr = deque->front;
    deque->front = deque->front->next;
    if(deque->front)
    {
        deque->front->prev = NULL;
    }
    else
    {
        deque->rear = NULL;
    }
    free(deletePtr);
    deque->size--;
    return true;
}

bool popFromRear(LinkedDeque* deque)
{
    if(!deque || isDequeEmpty(deque))
    {
        return false;
    }

    Node* deletePtr = deque->rear;
    deque->rear = deque->rear->prev;
    if(deque->rear)
    {
        deque->rear->next = NULL;
    }
    else
    {
        deque->front = NULL;
    }
    free(deletePtr);
    deque->size--;
    return false;
}

bool getFrontElem(LinkedDeque* deque, elemType* bufferForElem)
{
    if(!deque || isDequeEmpty(deque))
    {
        return false;
    }

    *bufferForElem = deque->front->data;
    return true;
}

bool getRearElem(LinkedDeque* deque, elemType* bufferForElem)
{
    if(!deque || isDequeEmpty(deque))
    {
        return false;
    }

    *bufferForElem = deque->rear->data;
    return true;
}

void clearDeque(LinkedDeque* deque)
{
    if(!deque)
    {
        return;
    }

    while (!isDequeEmpty(deque))
    {
        popFromFront(deque);
    }
    
    return;
}

void printDeque(LinkedDeque* deque)
{
    if(isDequeEmpty(deque))
    {
        printf("Empty Queue!\n");
        return;
    }

    printf("\n----------------Current Queue------------------\n");
    for (Node* front = deque->front; front!=NULL ; front=front->next)
    {
        printf("%d ", front->data);
    }
    printf("\n-----------------------------------------------\n");
    
    return;
}

int main()
{
    LinkedDeque* deque = initDeque();
    pushToRear(deque, 32);
    pushToRear(deque, 64);
    pushToRear(deque, 128);
    pushToRear(deque, 512);
    pushToRear(deque, 1024);
    printDeque(deque);
    popFromFront(deque);
    popFromRear(deque);
    popFromRear(deque);
    printf("Deque Size:%d\n", getDequeSize(deque));
    elemType frontElem;
    elemType rearElem;
    getFrontElem(deque, &frontElem);
    getRearElem(deque, &rearElem);
    printf("Front Elem:%d\n", frontElem);
    printf("Rear Elem:%d\n", rearElem);
    printDeque(deque);
    clearDeque(deque);
    printDeque(deque);
    
    return 0;
}

