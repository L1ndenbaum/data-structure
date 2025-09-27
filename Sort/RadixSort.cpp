#include<iostream>

/**
 * 基数排序中比较每一位时的底层排序方法
 * 这里使用计数排序
 * exp为当前位的权, 个位是1, 十位是10, ...
 */
void countSort(int* arr, int numElems, int exp)
{
    // 这个函数处理的十进制位 是 (arr[i]/exp) %10
    int count[10] = {0}; // count[i]表示当前位的值为i的数字有多少
    for (int i = 0; i < numElems; i++)
    {
        count[(arr[i]/exp) % 10]++;
    }
    for (int i = 1; i < 10; i++)
    {
        count[i] += count[i-1];
    }
    
    int* sorted = new int[numElems];
    for (int i = numElems-1; i >= 0; i--)
    {
        int bitValue = (arr[i]/exp)%10;
        sorted[count[bitValue]-1] = arr[i];
        count[bitValue]--;
    }
    for (int i = 0; i < numElems; i++)
    {
        arr[i] = sorted[i];
    }
    
    return;
}

void radixSort(int* arr, int numElems)
{
    int maxValue = 0x80000000;
    for (int i = 0; i < numElems; i++)
    {
        maxValue = std::max(arr[i], maxValue);
    }

    for (int exp = 1; maxValue/exp > 0; exp*=10) // 按照当前位的大小排序, 从个位到最高位
    {
        countSort(arr, numElems, exp);
    }

    return;
}

int main()
{
    int arr[] = { 170, 45, 75, 90, 802, 24, 2, 66 };
    int n = sizeof(arr) / sizeof(arr[0]);

    radixSort(arr, n);
    for (int i = 0; i < n; i++)
    {
        std::cout << arr[i] << ' ';
    }
    
    return 0;
}