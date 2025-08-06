#include <iostream>
#define elemType int

/**
 * 循环单链表的节点, 与普通单链表一样,
 * 不同的是循环单链表在build时需要将最后一个节点的next置为head
 * head不存data, 但最后一个节点存data
 */
struct RecurrentLinkedListNode
{
    elemType data;
    RecurrentLinkedListNode* next;
};

RecurrentLinkedListNode* initRecurrentLinkedList()
{
    RecurrentLinkedListNode* head = (RecurrentLinkedListNode*)malloc(sizeof(RecurrentLinkedListNode));
    if(!head)
    {
        return NULL;
    }    

    head->next = head;
    return head;
}