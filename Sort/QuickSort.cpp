#include <iostream>

/**
 * 一次对arr[left:right]的partition
 * 每次选择arr[right]作为pivot
 * 经过一次partition, 所有比pivot小的元素在pivot左边
 * 所有大于或等于pivot的元素在pivot右边
 * 返回pivot的下标位置
 */
int partition(int* arr, int left, int right)
{
    int pivot = arr[right];
    int i = left-1; // 表示小于pivot的元素所在区间的尾部位置
    for (int j = left; j < right; j++)
    {
        if(arr[j] < pivot)
        {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }
    
    std::swap(arr[i+1], arr[right]); // 把pivot放到分界点上
    return i+1;
}

/**
 * 将arr[left]到arr[right]排好序
 */
void quickSort(int* arr, int left, int right)
{
    if(left >= right) { return; }
    int pivotIdx = partition(arr, left, right);
    quickSort(arr, left, pivotIdx-1);
    quickSort(arr, pivotIdx+1, right);

    return;
}

int main()
{
    int arr[10] = {9, 18, 2, 58, 22, 3, 1, 4, 0, 6};
    quickSort(arr, 0, 9);
    for (int i = 0; i < 10; i++)
    {
        std::cout << arr[i] << ' ';
    }
    
    return 0;
}