#include <iostream>
#include <stack>
#include <string>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <sstream>

// 检查括号是否匹配
bool isBracketMatch(const char* bracketSequence)
{
    std::stack<char> stack;
    while(*bracketSequence != '\0')
    {
        if(*bracketSequence == '(' || *bracketSequence == '[' || *bracketSequence == '{')
        {
            stack.push(*bracketSequence);
        }
        else if(*bracketSequence == ')' || *bracketSequence == ']' || *bracketSequence == '}')
        {
            if(stack.empty())
            {
                return false; // 没有匹配的左括号
            }
            char top = stack.top();
            stack.pop();
            if((*bracketSequence == ')' && top != '(') ||
               (*bracketSequence == ']' && top != '[') ||
               (*bracketSequence == '}' && top != '{'))
            {
                return false; // 括号不匹配
            }
        }
        bracketSequence++;
    }

    return stack.empty(); // 如果栈为空，则所有括号都匹配
}

/**
 * 将中缀表达式转换为后缀表达式
 * 
 * 对于其中的规则, 可以这样理解:
 * 在后缀表达式中, 运算顺序按照从左到右的顺序依次取操作数和操作符进行,
 * 那么自然要把优先级高的运算放在前面,
 * 所以, 遇到带括号的子算式的时候, 左括号先入栈, 遇到右括号的时候一次性弹出去, 表示"这部分运算要先做"
 * 在一个运算符比栈顶优先级高的时候, 将它入栈, 是因为栈是先进后出的, 它成为了新的栈顶后, 会先出, 从而先参与运算, 表示它的优先级更高
 * 在一个运算符比栈顶优先级低或等于栈顶的时候, 出栈直到栈顶运算符优先级更高, 
 * 是因为要先做优先级高的运算(这体现在栈顶的运算符优先级更高)或更靠前的运算(这体现在栈顶的运算符优先级于它相同)
 */
const char* convertInfixExpressionToPostfix(const char* infixExpression)
{
    std::stack<char> stack;
    int expressionSize = sizeof(char) * (strlen(infixExpression)+1);
    char* postfixExpression = (char*)malloc(expressionSize);
    char* charPtr = postfixExpression;
    while(*infixExpression != '\0')
    {
        if(*infixExpression == '(') // 遇到左括号直接入栈
        {
            stack.push(*infixExpression);
        }
        else if(*infixExpression == ')') // 遇到右括号弹出字符到 postfix 直到遇到左括号 并删除左括号
        {
            while(stack.top() != '(')
            {
                *charPtr++ = stack.top();
                stack.pop();
            }
            stack.pop(); // 弹出左括号
        }
        else if(*infixExpression == '+' || *infixExpression == '-' ||
                *infixExpression == '*' || *infixExpression == '/')
        {
            /**
             * 遇到运算符时, 
             * 1.如果运算符优先级高于栈顶运算符直接入栈
             * 2.如果运算符的优先级低于或等于栈顶运算符,
             *   弹出栈顶运算符到 postfix 直到栈顶运算符优先级小于当前运算符或遇到左括号或栈空 之后将当前运算符入栈
             */
            while(!stack.empty() && (stack.top() == '*' || stack.top() == '/' ||
                                     (stack.top() == '+' || stack.top() == '-') &&
                                     (*infixExpression == '+' || *infixExpression == '-')))
            {
                *charPtr++ = stack.top();
                stack.pop();
            }
            stack.push(*infixExpression);
        }
        else
        {
            *charPtr++ = *infixExpression; // 遇到操作数, 直接加入后缀表达式
        }
        infixExpression++;
    }
    // 弹出栈中剩余的运算符到 postfix
    while(!stack.empty())
    {
        *charPtr++ = stack.top();
        stack.pop();
    }

    *charPtr = '\0';
    return postfixExpression; // 返回后缀表达式
}

/**
 * 用后缀表达式求值, 这里的操作数应为具体的个位整数
 * 注意, 操作数栈中, 先弹出去的是右操作数
 * 这只是一个表达式求值的简化实现
 * 可以考虑使用一个 vector<Element> 实现序列
 */
int calculatePostfixExpression(const char* postExpression)
{
    std::stack<int> stackedOprands;
    while (*postExpression != '\0')
    {
        switch (*postExpression)
        {
            case '+':
            {
                int rightOprand = stackedOprands.top(); stackedOprands.pop();
                int leftOprand = stackedOprands.top(); stackedOprands.pop();
                stackedOprands.push(leftOprand + rightOprand);
                break;
            }
            case '-':
            {
                int rightOprand = stackedOprands.top(); stackedOprands.pop();
                int leftOprand = stackedOprands.top(); stackedOprands.pop();
                stackedOprands.push(leftOprand - rightOprand);
                break;
            }
            case '*':
            {
                int rightOprand = stackedOprands.top(); stackedOprands.pop();
                int leftOprand = stackedOprands.top(); stackedOprands.pop();
                stackedOprands.push(leftOprand * rightOprand);
                break;
            }
            case '/':
            {
                int rightOprand = stackedOprands.top(); stackedOprands.pop();
                int leftOprand = stackedOprands.top(); stackedOprands.pop();
                stackedOprands.push(leftOprand / rightOprand);
                break;
            }
            default:
            {
                stackedOprands.push(*postExpression - '0');
                break;
            }
        }
        postExpression++;
    }
    
    return stackedOprands.top();
}

// 更复杂的实现
struct SequenceElement
{
    enum ElemType
    {
        NUMBER,
        OPERATOR
    };
    ElemType elemType;
    float number;
    char op;
};
std::vector<SequenceElement> sequence;

/**
 * 将一个含有小数、负号的中缀表达式的数字和运算符分开为可计算的变量 保留所有括号
 * 假设输入的中缀表达式总合法
 */
std::vector<SequenceElement> parseInfixExpression(const char* infixExpression)
{   
    std::vector<SequenceElement> parsedResult;
    unsigned int ptr = 0;
    char* currentNumberBuffer = (char*)malloc(sizeof(char)*strlen(infixExpression) + 1);
    unsigned int currentNumberBufferPtr = 0;
    auto convertNumberBufferThenPushToResult = [ 
        &currentNumberBuffer, &currentNumberBufferPtr, &parsedResult] () -> void
    {
        currentNumberBuffer[currentNumberBufferPtr] = '\0';
        if(!strlen(currentNumberBuffer)) // 符号可能连续, 因此只在Buffer非空时进行push
        { return ;}
        parsedResult.push_back(
            SequenceElement{.elemType=SequenceElement::ElemType::NUMBER,
                            .number=std::__cxx11::stof(currentNumberBuffer)});
        currentNumberBufferPtr = 0;
    };

    while (infixExpression[ptr])
    {
        if(infixExpression[ptr]=='(' || infixExpression[ptr]==')') // 遇到括号直接push进结果
        {
            convertNumberBufferThenPushToResult();
            parsedResult.push_back(
                SequenceElement
                {
                    .elemType=SequenceElement::ElemType::OPERATOR,
                    .op=infixExpression[ptr++]
                }
            );
            continue;
        }
        if('0'<=infixExpression[ptr] && infixExpression[ptr]<='9') // 遇到数字, 追加到NumberBuffer
        {
            currentNumberBuffer[currentNumberBufferPtr++] = infixExpression[ptr++];
            continue;
        }
        switch (infixExpression[ptr]) // 遇到符号, 分运算符和小数点
        {
            case '-':
            {
                if(ptr==0 || infixExpression[ptr-1]=='(') // 减号的前面没有东西, 或者减号前面是个左括号, 那么减号是负号
                {
                    currentNumberBuffer[currentNumberBufferPtr++] = infixExpression[ptr++]; // 扩充当前数字字符串
                }
                else // 否则就是减号 这时要将之前的NumberBuffer转换成数字并push
                {
                    convertNumberBufferThenPushToResult();
                    parsedResult.push_back(SequenceElement{ .elemType=SequenceElement::ElemType::OPERATOR, .op='-'});
                    ptr++;
                }
                break;
            }
            case '+':
            {
                if(ptr==0 || infixExpression[ptr-1]=='(') // 加号的前面没有东西, 或者加号前面是个左括号, 那么加号是正号
                {
                    ptr++; // 直接忽略这个正号
                }
                else // 否则就是加号 这时要将之前的NumberBuffer转换成数字并push
                {
                    convertNumberBufferThenPushToResult();
                    parsedResult.push_back(SequenceElement{ .elemType=SequenceElement::ElemType::OPERATOR, .op='+'});
                    ptr++;
                }
                break;
            }
            case '.': // 小数点
            {
                currentNumberBuffer[currentNumberBufferPtr++] = infixExpression[ptr++];
                break;
            }
            default: // 乘号或除号
            {
                convertNumberBufferThenPushToResult();
                parsedResult.push_back(
                    SequenceElement
                    {
                        .elemType=SequenceElement::ElemType::OPERATOR,
                        .op=infixExpression[ptr++]
                    }
                );
            }
        }
    }
    
    return parsedResult;
}

// 复杂版本的将中缀表达式转换为后缀表达式
std::vector<SequenceElement> convertInfixExpressionToPostfix(std::vector<SequenceElement>& infixExpression)
{
    std::stack<SequenceElement> stack;
    std::vector<SequenceElement> postfixExpression;
    for(SequenceElement& element : infixExpression)
    {
        switch (element.elemType)
        {
            case SequenceElement::OPERATOR:
            {
                if(element.op == '(') // 遇到左括号直接入栈
                {
                    stack.push(element);
                }
                else if(element.op == ')') // 遇到右括号弹出字符到 postfix 直到遇到左括号 并删除左括号
                {
                    while(stack.top().op != '(')
                    {
                        postfixExpression.push_back(stack.top());
                        stack.pop();
                    }
                    stack.pop(); // 弹出左括号
                }
                else if(element.op == '+' || element.op == '-' ||
                        element.op == '*' || element.op == '/')
                {
                    /**
                     * 遇到运算符时, 
                     * 1.如果运算符优先级高于栈顶运算符直接入栈
                     * 2.如果运算符的优先级低于或等于栈顶运算符,
                     *   弹出栈顶运算符到 postfix 直到栈顶运算符优先级小于当前运算符或遇到左括号或栈空 之后将当前运算符入栈
                     */
                    while(!stack.empty() && (stack.top().op == '*' || stack.top().op == '/' ||
                                            (stack.top().op == '+' || stack.top().op == '-') &&
                                            (element.op == '+' || element.op == '-')))
                    {
                        postfixExpression.push_back(stack.top());
                        stack.pop();
                    }
                    stack.push(element);
                }
                break;
            }
            case SequenceElement::NUMBER: // 遇到操作数, 直接加入后缀表达式
            {
                postfixExpression.push_back(element);
                break;
            }
        }
    }
    while(!stack.empty()) // 弹出栈中剩余的运算符到 postfix
    {
        postfixExpression.push_back(stack.top());
        stack.pop();
    }

    return postfixExpression; // 返回后缀表达式
}

// 复杂版本的后缀表达式求值
float calculatePostfixExpression(std::vector<SequenceElement>& postExpression)
{
    std::stack<float> stackedOprands;
    for(SequenceElement& element : postExpression)
    {
        switch (element.elemType)
        {
            case SequenceElement::NUMBER:
            {
                stackedOprands.push(element.number);
                break;
            }
            case SequenceElement::OPERATOR:
            {
                switch (element.op)
                {
                    case '+':
                    {
                        float rightOprand = stackedOprands.top(); stackedOprands.pop();
                        float leftOprand = stackedOprands.top(); stackedOprands.pop();
                        stackedOprands.push(leftOprand + rightOprand);
                        break;
                    }
                    case '-':
                    {
                        float rightOprand = stackedOprands.top(); stackedOprands.pop();
                        float leftOprand = stackedOprands.top(); stackedOprands.pop();
                        stackedOprands.push(leftOprand - rightOprand);
                        break;
                    }
                    case '*':
                    {
                        float rightOprand = stackedOprands.top(); stackedOprands.pop();
                        float leftOprand = stackedOprands.top(); stackedOprands.pop();
                        stackedOprands.push(leftOprand * rightOprand);
                        break;
                    }
                    case '/':
                    {
                        float rightOprand = stackedOprands.top(); stackedOprands.pop();
                        float leftOprand = stackedOprands.top(); stackedOprands.pop();
                        stackedOprands.push(leftOprand / rightOprand);
                        break;
                    }
                }
                break;
            }
        }
    }
    
    return stackedOprands.top();
}

int main()
{
    const char* infixExpression = "(1)+(2)*((6)-(4))-(8)/(2)";
    const char* infixExpression2 = "1+2.3*(-6-4)-8/(-2)";
    std::vector<SequenceElement> parsedInfixExpression = parseInfixExpression(infixExpression2); 
    const char* postfixExpression = convertInfixExpressionToPostfix(infixExpression);
    printf("Infix2Postfix Expression: %s\n", postfixExpression);
    printf("Value: %d\n", calculatePostfixExpression(postfixExpression));
    std::cout << std::endl;
    std::vector<SequenceElement> postfixExpression2 = convertInfixExpressionToPostfix(parsedInfixExpression);
    std::cout << calculatePostfixExpression(postfixExpression2) << std::endl;
    return 0;
}