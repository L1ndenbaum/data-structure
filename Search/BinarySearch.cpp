#include<iostream>

/**
 * 用二分查找在有序数组中查找值为key的元素索引
 * 若找不到, 则返回-1
 */
int binarySearch(int* sortedSequence, int numElems, int key)
{
    int left = 0, right = numElems-1;
    int mid;
    while (left < right)
    {   
        mid = left + ((right-left) >> 1);
        if(sortedSequence[mid] < key) { left = mid+1;}
        else if(sortedSequence[mid] > key) { right = mid-1; }
        else { return mid; }
    }
    
    return -1;
}

int main()
{
    int arr[5] = { 2, 3, 4, 10, 40 };
    int x = 10;
    int result = binarySearch(arr, 5, x);
    if(result == -1) std::cout << "Element is not present in array";
    else std::cout << "Element is present at index " << result;
    return 0;
}