#include<iostream>
#define elemType int

struct SequenceStack
{
    int top;
    int maxSize;
    elemType* data;
};

SequenceStack* initSequenceStack(int maxSize)
{
    SequenceStack* stackPtr = (SequenceStack*)malloc(sizeof(SequenceStack));
    if(!stackPtr)
    {
        return NULL;
    }

    stackPtr->top = -1;
    stackPtr->maxSize = maxSize;
    stackPtr->data = (elemType*)malloc(sizeof(elemType) * maxSize);
    return stackPtr;
}

bool isStackEmpty(SequenceStack* stackPtr)
{
    return stackPtr->top == -1;
}

bool isStackFull(SequenceStack* stackPtr)
{
    return stackPtr->top == stackPtr->maxSize;
}

/**
 * 入栈, 栈未满返回true并压value进入作为栈顶, 栈满返回false
 */
bool pushToStack(SequenceStack* stackPtr, elemType value)
{
    if(isStackFull(stackPtr))
    {
        return false;
    } 

    stackPtr->data[++stackPtr->top] = value;
    return true;
}

/**
 * 出栈, 栈空返回false, 栈非空返回true并弹出栈顶元素给 bufferForPoppedElem
 */
bool popFromStack(SequenceStack* stackPtr, elemType* bufferForPoppedElem)
{
    if(stackPtr->top==-1 || !bufferForPoppedElem)
    {
        return false;
    }

    *bufferForPoppedElem = stackPtr->data[stackPtr->top];
    stackPtr->top--;
    return true;
}

bool getStackTopElem(SequenceStack* stackPtr, elemType* bufferForTopElem)
{
    if(isStackEmpty(stackPtr) || !bufferForTopElem)
    {
       return false;
    }

    *bufferForTopElem = stackPtr->data[stackPtr->top];
    return true;
}

int main()
{
    SequenceStack* stack = initSequenceStack(50);
    bool opSuccess = pushToStack(stack, 64);
    if(opSuccess)
    {
        elemType topElem;
        if(getStackTopElem(stack, &topElem))
        {
            printf("%d\n", topElem);
        }
    }
    elemType poppedElem;
    if(popFromStack(stack, &poppedElem))
    {
        printf("%d\n", poppedElem);
    }
}