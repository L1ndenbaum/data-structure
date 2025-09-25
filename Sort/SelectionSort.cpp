#include<iostream>

/**
 * 升序选择排序
 * 时间复杂度 O(N^2)
 * 空间复杂度 O(1)
 */
void selectionSort(int* arr, int numElements)
{
    for (int i = 0; i < numElements-1; i++)
    {
        int minElementIdx = i;
        for (int j = i+1; j < numElements; j++)
        {
            if(arr[j] < arr[minElementIdx])
            {
                minElementIdx = j;
            }
        }
        std::swap(arr[minElementIdx], arr[i]);
    }
}

int main()
{
    int arr[5] = {64, 25, 12, 22, 11};

    std::cout << "Original array: "<< std::endl;
    for (int i = 0; i < 5; i++)
    {
        std::cout << arr[i] << ' ';
    }
    std::cout<<std::endl;
    
    selectionSort(arr, 5);

    std::cout << "Sorted array: " << std::endl;
    for (int i = 0; i < 5; i++)
    {
        std::cout << arr[i] << ' ';
    }
    std::cout << std::endl;

    return 0;
}