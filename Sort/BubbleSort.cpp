#include<iostream>
#include<algorithm>

/**
 * 冒泡排序 升序排
 * 时间复杂度 O(N^2)
 * 空间复杂度 O(1)
 */
void bubbleSort(int* arr, int numElements)
{
    for (int i = 0; i < numElements-1; i++)
    {
        bool swaped = false;
        for (int j = 0; j < numElements-i-1; j++)
        {
            if(arr[j+1] < arr[j])
            { 
                std::swap(arr[j+1], arr[j]); 
                swaped = true;
            }
        }
        if(!swaped) { break; }
    }

    return;
}

int main()
{
    int arr[10] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    int arr2[10] = {0, -32, -2, 1, 999, 87, 1, 4, 1, 0};
    bubbleSort(arr, 10);
    for(int i = 0; i < 10; i++)
    {
        std::cout << arr[i] << ' ';
    }

    std::cout << std::endl;
    bubbleSort(arr2, 10);
    for(int i = 0; i < 10; i++)
    {
        std::cout << arr2[i] << ' ';
    }

    return 0;
}