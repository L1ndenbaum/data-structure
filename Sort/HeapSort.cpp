#include<iostream>

/**
 * 将以下标为current的节点代表的子树变成小顶堆
 */
void heapify(int* arr, int numElements, int currentIdx)
{
    int smallestIdx = currentIdx;
    int leftChildIdx = 2*currentIdx + 1;
    int rightChildIdx = 2*currentIdx + 2;
    if(leftChildIdx<numElements && arr[leftChildIdx]<arr[smallestIdx]) // 左孩子比根(最小的元素)小, 更新最小索引
    {
        smallestIdx = leftChildIdx;
    }
    if(rightChildIdx<numElements && arr[rightChildIdx]<arr[smallestIdx]) // 右孩子比根小, 更新最小索引
    {
        smallestIdx = rightChildIdx;
    }
    if(smallestIdx != currentIdx) // 当当前子树的根不是最小的时候, 把最小的换上来
    {
        std::swap(arr[currentIdx], arr[smallestIdx]);
        // 交换后的孩子所在的子树可能破坏了堆的性质, 也需要heapify
        heapify(arr, numElements, smallestIdx);
    }

    return;
}

/**
 * 用小顶堆的堆排序
 * 最后将数组输出的结果是从大到小的
 */
void heapSort(int* arr, int numElements)
{
    for (int i = (numElements>>1)-1; i >= 0; i--)
    {
        // 叶子节点本身就满足堆性质, 从第一个非叶子节点开始
        // 从下到上建堆, 可以保证
        heapify(arr, numElements, i);
    }
    for (int i = numElements-1; i > 0; i--) // 把最小值一个个从堆顶拿下来 放到数组尾部
    {
        std::swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
    
    return;
}

template<typename T>
struct PriorityQueue
{
    // 大顶堆
    T* storage;
    int storagePtr = -1;
    PriorityQueue(int maxSize)
    {
        this->storage = new T[maxSize];
    }
    ~PriorityQueue()
    {
        delete[] this->storage;
    }

    inline int size()
    {
        return storagePtr;
    }

    inline bool empty()
    {
        return storagePtr == -1;
    }

    inline T top()
    {
        return storage[0];
    }

    void push(T x)
    {
        int idx = ++storagePtr;
        storage[idx] = x;

        // 上滤
        while (idx > 0)
        {
            int parent = (idx-1) / 2;
            if (storage[parent] >= storage[idx]) { break; }
            std::swap(storage[parent], storage[idx]);
            idx = parent;
        }
    }

    void pop()
    {
        if (storagePtr < 0) return;
        std::swap(storage[0], storage[storagePtr]);
        storagePtr--;

        // 下滤
        int idx = 0;
        while (true)
        {
            int leftChildIdx = 2*idx + 1;
            int rightChildIdx = 2*idx + 2;
            int largest = idx;

            if (leftChildIdx <= storagePtr && storage[leftChildIdx] > storage[largest])
            {
                largest = leftChildIdx;
            }
            if (rightChildIdx <= storagePtr && storage[rightChildIdx] > storage[largest])
            {
                largest = rightChildIdx;
            }
            if (largest == idx) { break; }
            std::swap(storage[idx], storage[largest]);
            idx = largest;
        }
    }
};

int main()
{
    int arr[7] = { 9, 4, 3, 8, 10, 2, 5 };

    heapSort(arr, 7);

    std::cout << "Sorted array is \n";
    for(int i = 0;i < 7; i++)
    {
        std::cout << arr[i]<< ' ';
    }
    std::cout<< std::endl;

    PriorityQueue<int> pq(10);
    for (int i = 0; i < 9; i++)
    {
        pq.push(i);
    }
    for (int i = 0; i < 9; i++)
    {
        std::cout << pq.top() << ' ';
        pq.pop();
    }
    
    return 0;
}