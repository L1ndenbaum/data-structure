#include<iostream>

/**
 * 计数排序, 适用于小整数
 * 根据每个数字出现的次数进行统计
 * 不能就地排序, 需要返回新的排序后数组
 * 如果有负数, 由于数组地址不能是负的, 要采用偏移量
 * 时间复杂度 O(numElems+maxValue) 空间复杂度O(maxValue)
 * 是稳定的, 最后的for从后往前遍历以保证相对位置不变
 */
int* countSort(int* arr, int numElems)
{
    int maxValue = 0x80000000;
    for (int i = 0; i < numElems; i++)
    {
        maxValue = std::max(arr[i], maxValue);
    }
    int* count = new int[maxValue+1];

    for (int i = 0; i < numElems; i++) // 统计每个数字出现了多少次
    {
        count[arr[i]]++;
    }
    
    for (int i = 1; i < numElems; i++) // 计算前缀和, 计算后count[i]表示小于等于i的数有多少个
    {
        count[i] += count[i-1];
    }
    
    int* sortedArray = new int[numElems];
    for (int i = numElems-1; i >= 0 ; i--)
    {
        sortedArray[count[arr[i]]-1] = arr[i]; //比arr[i]小的数有count[arr[i]-1]个, 那么把arr[i]放在这个位置
        count[arr[i]]--;
    }
    
    delete[] count;
    return sortedArray;
}

int main()
{
    int arr[10] = {2, 4, 5, 3 , 1, 8 , 9, 7, 0, 2};
    int* sorted = countSort(arr, 10);
    for (int i = 0; i < 10; i++)
    {
        std::cout << sorted[i] << ' ';
    }

    return 0;
}