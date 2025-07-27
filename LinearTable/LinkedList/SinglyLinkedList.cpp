#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#define elemType int
#define getArrayLength(arr) (sizeof(arr) / sizeof((arr[0])))
using namespace std;

// Pure C
struct linkedListNode
{
    elemType data;
    struct linkedListNode* next;
};

/**
 * 返回分配了内存的单链表头节点
 */
linkedListNode* initLinkedList()
{
    linkedListNode* headNode = (linkedListNode*)malloc(sizeof(linkedListNode));
    if (!headNode) {return NULL;}
    headNode -> next = NULL;
    return headNode;
}

/**
 * 求数据节点个数 不含head
 */
int getLinkedListLength(linkedListNode* head)
{
    int len = 0;
    linkedListNode* p = head -> next;
    while(p)
    {
        p = p->next;
        len++;
    }

    return len;
}

/**
 * 求第index个数据节点地址, 不含head
 * index从0开始, 即 head->index0->index1->...
 */
linkedListNode* getLinkedListNodeByIndex(linkedListNode* head, int index)
{
    if(getLinkedListLength(head) < index)
    {
        return NULL;
    }
    
    linkedListNode* p = head->next;
    while(index--)
    {
        p = p->next;
    }

    return p;
}

/**
 * 求节点data字段值为value的所有节点地址
 */
linkedListNode** getLinkedListNodesByValues(linkedListNode* head, elemType value)
{
    if(!head)
    {
        return NULL;
    }
    int linkedListLength = getLinkedListLength(head);

    linkedListNode** allFoundAddrs = (linkedListNode**)malloc(linkedListLength*sizeof(linkedListNode*));
    if(!allFoundAddrs)
    {
        return NULL;
    }
    int addrIndex = 0;

    linkedListNode* p = head->next;
    while(p)
    {
        if(p->data == value)
        {
            allFoundAddrs[addrIndex++] = p;
        }
        p = p->next;
    }

    return allFoundAddrs;
}

/**
 * 在index位置节点插入一个值为value的节点并将原节点置为index+1位置, index从0开始, 不含head
 * 返回插入的节点的地址
 */
linkedListNode* insertLinkedListNodeByIndex(linkedListNode* head, int index, elemType value)
{
    if(getLinkedListLength(head) < index)
    {
        return NULL;
    }
    
    linkedListNode* p = head;
    while(index--)
    {
        p = p->next;
    } // 定位到插入位置的前一个节点
    linkedListNode* insertedNode = (linkedListNode*)malloc(sizeof(linkedListNode));
    *insertedNode = {.data=value, .next=p->next};
    p->next = insertedNode;

    return insertedNode;
}

/**
 * 删除第index个数据节点, index从0开始, 不含head
 */
void deleteLinkedListNodeByIndex(linkedListNode* head, int index)
{
    if(getLinkedListLength(head) <= index)
    {
        return;
    }

   linkedListNode* p = head;
   while (index--)
   {
        p = p->next;
   } // 定位到待删除节点的前一个
   linkedListNode* deletePtr = p->next;
   p->next = p->next->next;
   free(deletePtr);

   return;
}

/**
 * 删除所有值为value的节点
 */
void deleteLinkedListNodesByValue(linkedListNode* head, elemType value)
{
   linkedListNode* prevNode = head;
   linkedListNode* currentNode = head->next;
   while(currentNode)
   {
        if(currentNode->data == value)
        {
            prevNode -> next = currentNode->next;
            linkedListNode* deletePtr = currentNode;
            currentNode = currentNode->next;
            free(deletePtr);
            continue;
        }
        prevNode = currentNode;
        currentNode = currentNode -> next;
   }
}

/**
 * 根据给定的数组, 建立一个单链表, 数组元素将被依次放在数据节点部分, 头节点不带数据
 * 默认使用尾插, 单链表数据顺序和数组一致
 * 使用reversed=true以使用头插, 单链表数据顺序和数组相反
 * 返回建立的单链表的头节点地址
 */
linkedListNode* buildLinkedListByArray(elemType* arr, int arrLength, bool reversed=false)
{
    linkedListNode* head = initLinkedList();
    switch (reversed)
    {
        case true:
        {
            for (int i = 0; i < arrLength; i++)
            {
                linkedListNode* newNode = (linkedListNode*)malloc(sizeof(linkedListNode));
                *newNode = {.data=arr[i], .next=NULL};
                newNode->next = head->next;
                head->next = newNode;
            }
            break;
        }      

        default:
        {
            linkedListNode* currentNode = head;
            for (int i = 0; i < arrLength; i++)
            {
                linkedListNode* newNode = (linkedListNode*)malloc(sizeof(linkedListNode));
                *newNode = {.data=arr[i], .next=NULL};
                currentNode->next = newNode;
                currentNode = newNode;
            }
            break;
        }
    }
        
    return head;
}

/**
 * 打印单链表每个节点的地址和值, 头节点单独打印
 */
void printLinkedList(linkedListNode* head)
{
    printf("Head Node | Addr: %p | Value: None\n", head);
    linkedListNode* p = head->next;
    while(p)
    {
        printf("Data Node | Addr: %p | Value: %d\n", p, p->data);
        p = p->next;
    }
}

int main()
{
    elemType testArray[6] = {10, 28, 32, 64, 199, 65535};
    linkedListNode* reversedArrayLinkedListHead = buildLinkedListByArray(testArray, getArrayLength(testArray), true);
    linkedListNode* ArrayLikeLinkedListHead = buildLinkedListByArray(testArray, getArrayLength(testArray));
    printLinkedList(reversedArrayLinkedListHead);
    insertLinkedListNodeByIndex(reversedArrayLinkedListHead, 6, __INT_MAX__);
    printLinkedList(reversedArrayLinkedListHead);

    printf("\n-------------------------------------\n");

    printLinkedList(ArrayLikeLinkedListHead);
    printf("Length of Linked List:%d\n", getLinkedListLength(ArrayLikeLinkedListHead));

    printf("\n-------------------------------------\n");

    insertLinkedListNodeByIndex(ArrayLikeLinkedListHead, 2, __INT_MAX__);
    insertLinkedListNodeByIndex(ArrayLikeLinkedListHead, 3, __INT_MAX__);
    insertLinkedListNodeByIndex(ArrayLikeLinkedListHead, 5, 999999);
    printLinkedList(ArrayLikeLinkedListHead);

    printf("\n-------------------------------------\n");

    deleteLinkedListNodesByValue(ArrayLikeLinkedListHead, __INT_MAX__);
    printLinkedList(ArrayLikeLinkedListHead);
    deleteLinkedListNodeByIndex(ArrayLikeLinkedListHead, 0);
    deleteLinkedListNodeByIndex(ArrayLikeLinkedListHead, 5);
    deleteLinkedListNodeByIndex(ArrayLikeLinkedListHead, 5);
    printLinkedList(ArrayLikeLinkedListHead);

    printf("\n-------------------------------------\n");

    insertLinkedListNodeByIndex(ArrayLikeLinkedListHead, 2, __INT_MAX__);
    insertLinkedListNodeByIndex(ArrayLikeLinkedListHead, 3, __INT_MAX__);
    linkedListNode** addrs = getLinkedListNodesByValues(ArrayLikeLinkedListHead, __INT_MAX__);
    printf("All Addrs: %p\t%p", addrs[0], addrs[1]);

    return 0;
}