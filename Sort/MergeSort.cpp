#include<iostream>

/**
 * Merge两个有序的部分数组为一个有序的数组
 * 注意 这里的Arr左右部分经过子任务分解并归并后 天然有序, 只是左右部分的大小关系不确定
 */
void merge(int* arr, int left, int mid, int right)
{
    if(left >= right) { return; }
    int* sortedArray = new int[right-left+1];
    int ptrLeft = left;
    int ptrRight = mid + 1;
    int ptrSorted = 0;
    while (ptrLeft <= mid && ptrRight <= right)
    {
        if(arr[ptrLeft] <= arr[ptrRight])
        {
            sortedArray[ptrSorted++] = arr[ptrLeft++];
        }
        else
        {
            sortedArray[ptrSorted++] = arr[ptrRight++];
        }
    }
    while (ptrLeft <= mid)
    {
        sortedArray[ptrSorted++] = arr[ptrLeft++];
    }
    while (ptrRight <= right)
    {
        sortedArray[ptrSorted++] = arr[ptrRight++];
    }

    for (int i=left, ptrSorted=0; i <= right; i++, ptrSorted++)
    {
        arr[i] = sortedArray[ptrSorted];
    }
    
    delete[] sortedArray;
    return;
}

/**
 * 归并排序 按升序排
 */
void mergeSort(int* arr, int left, int right)
{
    if(left >= right) { return; }
    int mid = left + ((right-left) >> 1);
    mergeSort(arr, left, mid);
    mergeSort(arr, mid+1, right);
    merge(arr, left, mid, right);

    return;
}

int main()
{
    int arr[10] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    int arr2[10] = {0, -32, -2, 1, 999, 87, 1, 4, 1, 0};
    mergeSort(arr, 0, 9);
    for(int i = 0; i < 10; i++)
    {
        std::cout << arr[i] << ' ';
    }

    std::cout << std::endl;
    mergeSort(arr2, 0, 9);
    for(int i = 0; i < 10; i++)
    {
        std::cout << arr2[i] << ' ';
    }

    return 0;
}