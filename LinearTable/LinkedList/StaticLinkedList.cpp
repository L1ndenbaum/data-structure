#include <iostream>
#define elemType int
#define LEN 100

/**
 * 静态链表用数组描述线性表的链式结构
 * 以 next=-1作为结束标志
 */
struct StaticLinkedList
{
    elemType data;
    int next; // 下一个元素数组下标
} staticLinkedList[LEN];

