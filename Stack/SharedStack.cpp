#include<iostream>
#define STACK_SIZE 50
#define elemType int

enum StackIndex
{
    LEFT_STACK,
    RIGHT_STACK
};

/**
 * 共享栈为两个顺序栈共享一个数组空间
 */
struct SharedStack
{
    int maxSize = STACK_SIZE;
    elemType* data;
    int leftStackTop;
    int rightStackTop;
};

SharedStack* initSharedStack(int maxSize)
{
    SharedStack* stackPtr = (SharedStack*)malloc(sizeof(SharedStack));
    if(!stackPtr)
    {
        return NULL;
    }
    
    stackPtr->maxSize = maxSize;
    stackPtr->data = (elemType*)malloc(sizeof(elemType) * maxSize);
    stackPtr->leftStackTop = -1;
    stackPtr->rightStackTop = stackPtr->maxSize;

    return stackPtr;
}

bool isSharedStackEmpty(SharedStack* stackPtr)
{
    return stackPtr->leftStackTop==-1 || stackPtr->rightStackTop==stackPtr->maxSize;
}

bool isSharedStackFull(SharedStack* stackPtr)
{
    return stackPtr->rightStackTop - stackPtr->leftStackTop == 1;
}

void getTopElem(SharedStack* stackPtr, StackIndex stackIndex, elemType* bufferForElem)
{
    if(!stackPtr || !bufferForElem)
    {
        return;
    }

    switch (stackIndex)
    {
        case LEFT_STACK:
        {
            *bufferForElem = stackPtr->data[stackPtr->leftStackTop];
            return;
        }
            
        case RIGHT_STACK:
        {
            *bufferForElem = stackPtr->data[stackPtr->rightStackTop];
            return;
        }
        
        default:
        {
            return;
        }
    }
}

void pushElem(SharedStack* stackPtr, StackIndex stackIndex, elemType value)
{
    if(!stackPtr || isSharedStackFull(stackPtr))
    {
        return;
    }

    switch (stackIndex)
    {
        case LEFT_STACK:
        {
            stackPtr->data[++stackPtr->leftStackTop] = value;
            return;
        }
            
        case RIGHT_STACK:
        {
            stackPtr->data[--stackPtr->rightStackTop] = value;
            return;
        }
        
        default:
        {
            return;
        }
    }
}

void popElem(SharedStack* stackPtr, StackIndex stackIndex, elemType* bufferForPoppedElem)
{
    if(!stackPtr || !bufferForPoppedElem)
    {
        return;
    }

    switch (stackIndex)
    {
        case LEFT_STACK:
        {
            if(stackPtr->leftStackTop == -1)
            {
                return;
            }

            *bufferForPoppedElem = stackPtr->data[stackPtr->leftStackTop--];
            return;
        }
            
        case RIGHT_STACK:
        {
            if(stackPtr->rightStackTop == stackPtr->maxSize)
            {
                return;
            }

            *bufferForPoppedElem = stackPtr->data[stackPtr->rightStackTop++];
            return;
        }
        
        default:
        {
            return;
        }
    }
}

int main()
{
    SharedStack* stack = initSharedStack(50);
    if(isSharedStackEmpty(stack))
    {
        printf("Stack Empty\n");
    }

    pushElem(stack, LEFT_STACK, 65535);
    pushElem(stack, LEFT_STACK, 255);
    pushElem(stack, RIGHT_STACK, 65534);

    elemType leftStackTopElem;
    elemType rightStackTopElem;
    getTopElem(stack, LEFT_STACK, &leftStackTopElem);
    getTopElem(stack, RIGHT_STACK, &rightStackTopElem);
    printf("%d\n", leftStackTopElem);
    printf("%d\n", rightStackTopElem);

    elemType poppedElem;
    popElem(stack, LEFT_STACK, &poppedElem);
    printf("%d\n", poppedElem);

    getTopElem(stack, LEFT_STACK, &leftStackTopElem);
    printf("%d\n", leftStackTopElem);

    return 0;
}