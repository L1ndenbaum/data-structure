#include<iostream>

/**
 * 升序直接插入排序
 * 这里的插入是通过将比待插入元素大的有序部分向右移动实现的
 * 时间复杂度最好O(N), 最坏O(N^2), 平均O(N^2)
 * 空间复杂度O(1)
 */
void insertionSort(int* arr, int numElements)
{
    for (int i = 0; i < numElements; i++)
    {
        int elemToSort = arr[i];
        int j = i-1;
        while (j>=0 && arr[j]>elemToSort)
        {
            // 从有序序列最大值开始, 比elemToSort大的时候, 就将比elemToSort大的有序子序列向右移动
            // 每移动一位, 就会空出来一个位置, 给elemToSort插入
            arr[j+1] = arr[j];
            j--;
        }
        arr[j+1] = elemToSort; // 最后的插入位置就是j+1
    }
}