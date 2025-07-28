#include <iostream>
#define elemType int

struct LinkedListDNode
{
    elemType data;
    LinkedListDNode* prev;
    LinkedListDNode* next;
};

LinkedListDNode* initDoublyLinkedList()
{
    LinkedListDNode* head = (LinkedListDNode*)malloc(sizeof(LinkedListDNode));
    if(!head)
    {
        return NULL;
    }

    return head; 
}

/**
 * 求链表所有数据节点长度, 不包括head
 */
int getDoublyLinkedListLength(LinkedListDNode* head)
{
    if(!head)
    {
        return 0;
    }
    LinkedListDNode* p = head->next;
    int len = 0;
    while (p)
    {
        len++;
        p = p->next;
    }

    return len;
}

/**
 * 在第index个数据节点插入值为value的节点, 数据节点不包括head, index从0开始
 * 返回被插入节点的地址
 */
LinkedListDNode* insertLinkedListDNodeByIndex(LinkedListDNode* head, int index, elemType value)
{
    if(getDoublyLinkedListLength(head) < index)
    {
        return NULL;
    }
    
    LinkedListDNode* p = head;
    while (index--)
    {
        p = p->next;
    } // 定位到插入位置前一个
    LinkedListDNode* insertedDNode = (LinkedListDNode*)malloc(sizeof(LinkedListDNode));
    *insertedDNode = {.data=value, .prev=p, .next=p->next};
    if(p->next)
    {
        p->next->prev = insertedDNode;
    } // 插入位置前一个 p可能是最后一个节点, 做一个检查
    p->next = insertedDNode;

    return insertedDNode;
}

/**
 * 删除第index个数据节点, 数据节点不包括head, index从0开始
 */
void deleteLinkedListDNodeByIndex(LinkedListDNode* head, int index)
{
    if(getDoublyLinkedListLength(head) <= index)
    {
        return;
    }

    LinkedListDNode* p = head->next;
    while (index--)
    {
        p = p->next;
    } // 定位到删除位置
    p->prev->next = p->next;
    p->next->prev = p->prev;
    free(p);

    return;
}

/**
 * 删除所有值为value的数据节点
 */
void deleteLinkedListDNodesByValue(LinkedListDNode* head, elemType value)
{
    if(!head)
    {
        return;
    }

    LinkedListDNode* p = head;
    while (p)
    {
        if(p->data == value)
        {
            LinkedListDNode* deletePtr = p;
            p->prev->next = p->next;
            if(p->next)
            {
                p->next->prev = p->prev;
            } // 同样地, 要检查待删除节点p是否为最后一个节点
            p = p->next;
            free(deletePtr);
            continue;
        }
        p = p->next;
    }
    
    return;
}

/**
 * 打印单链表每个节点的地址和值, 头节点单独打印
 */
void printLinkedList(LinkedListDNode* head)
{
    printf("\n-------------------------------------\n");
    printf("Head Node | Addr: %p | Value: None\n", head);
    LinkedListDNode* p = head->next;
    while(p)
    {
        printf("Data Node | Addr: %p | Value: %d\n", p, p->data);
        p = p->next;
    }
    printf("-------------------------------------\n");
}

int main()
{
    LinkedListDNode* head = initDoublyLinkedList();
    insertLinkedListDNodeByIndex(head, 0, 16);
    insertLinkedListDNodeByIndex(head, 1, 32);
    insertLinkedListDNodeByIndex(head, 2, 64);
    insertLinkedListDNodeByIndex(head, 3, 128);
    insertLinkedListDNodeByIndex(head, 2, 65535);
    insertLinkedListDNodeByIndex(head, 5, 65535);
    printLinkedList(head);

    deleteLinkedListDNodeByIndex(head, 0);
    printLinkedList(head);
    deleteLinkedListDNodesByValue(head, 65535);
    printLinkedList(head);

    return 0;
}