#include<iostream>
#include<string>
#include<vector>
#include<stack>

// 给定一个入栈序列, 入栈后允许出栈, 输出所有可能的出栈序列
std::vector<std::string> allOrders;
void getAllPopOrders
(
    std::string pushOrder,
    std::stack<char> currentStack=std::stack<char>{}, 
    std::string tempPopSeq=""
)
{
    if(pushOrder.empty() && currentStack.empty())
    {
        allOrders.push_back(tempPopSeq);
        return;
    }
    if(!pushOrder.empty()) // pushOrder非空时可以尝试入栈
    {
        char next = pushOrder[0];
        pushOrder.erase(0, 1);
        currentStack.push(next);
        getAllPopOrders(pushOrder, currentStack, tempPopSeq);
        currentStack.pop();
        pushOrder = next + pushOrder;
    }
    if(!currentStack.empty()) // stack不空, 可以尝试出栈
    {
        char poppedElem = currentStack.top();
        currentStack.pop();
        getAllPopOrders(pushOrder, currentStack, tempPopSeq+poppedElem);
        currentStack.push(poppedElem);
    }
}

int main()
{
    std::string pushOrder = "abc";
    getAllPopOrders(pushOrder);
    for(std::string& order : allOrders)
    {
        std::cout << order << std::endl;
    }
}