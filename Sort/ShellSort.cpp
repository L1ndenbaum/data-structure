#include<iostream>

/**
 * 希尔排序中, 下标差gap的元素属于一组
 * 先将gap设置为 n/2, 之后逐渐缩小gap, 局部插入排序每组
 * 最后整体插入排序一次
 */
void shellSort(int* arr, int numElements)
{
    for (int gap = numElements/2; gap > 0; gap /= 2)
    {
        for (int i = gap; i < numElements; i++)
        {
            // 这部分是插入排序的逻辑, 只不过元素之间的间隔是gap
            int elemToSort = arr[i];
            int j = i;
            while (j>=gap && arr[j-gap] >= elemToSort)
            {
                arr[j] = arr[j-gap];
                j -= gap;
            }
            arr[j] = elemToSort;
        }
    }
    
    return;
}

int main()
{
    int arr[10] = {9, 8 ,7, 6, 5, 4, 3, 2, 1, 0};
    shellSort(arr, 10);
    for (int i = 0; i < 10; i++)
    {
        std::cout<< arr[i] << ' ';
    }
    
    return 0;
}