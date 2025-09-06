#include<string>
#include<vector>
#include<iostream>
#include<cstdlib>
#include<cstring>

/**
 * 最简单的匹配, 若匹配到模式串, 返回所有出现的模式串在原始串中的起始下标
 * 时间复杂度 O(N*M), N为原始字符串规模, M为模式串规模
 */
std::vector<int> searchPattern(std::string& str, std::string& pattern)
{
    std::vector<int> locationsMatched;
    int strLength = str.length();
    int patternLength = pattern.length();
    for (int i = 0; i <= strLength-patternLength; i++)
    {
        int j = 0;
        for (; j < patternLength; j++)
        {
            if(str[i+j] != pattern[j])
            {
                break;
            }
        }
        if (j == patternLength)
        {
            locationsMatched.push_back(i);
        }
    }

    return locationsMatched;
}

/**
 * KMP匹配, 若匹配到模式串, 返回所有出现的模式串在原始串中的起始下标
 * KMP使用一个 Longest Prefix Suffix, 最长相等前后缀数组, 对模式串的每个Proper前缀子串(Proper前缀指的是串的前缀不等于串本身), 
 * 存储最长的、同时也是其后缀的子串的长度(但不能等于整个子串本身), 避免无意义的回溯
 */

// 构建模式串的最长相等前后缀数组
std::vector<int> constructLPS(std::string& pattern)
{
    int patternLength = pattern.length();
    std::vector<int> lps(patternLength, 0);
    // len记录在模式串 pattern[0:i] 的子串中, 前缀和后缀匹配的最大长度
    // i 遍历整个模式串, 确定lps[i]的值
    int len = 0, i = 1;
    while (i < patternLength)
    {
        if(pattern[i] == pattern[len])
        {
            // 这里, i前面长度为len的子串和 len位置前面长度为len的子串都匹配了, i位置也能和len位置匹配, 扩充即可
            len++;
            lps[i] = len;
            i++;
        }
        else
        {
            if(len!=0)
            {
                // 这里len>0, 说明pattern[0:len]能和pattern[i-len:i]配上, 但是i位置配不上
                // 那么需要在pattern[i-len:i]中找一个更短的后缀, 使其为pattern的Proper前缀
                // pattern[0:len]和pattern[i-len:i]是一样的, 所以不用在pattern[i-len:i]找, 只需要在pattern[0:len]找
                // 访问lps[len-1], 找到一个最长的, 重新检查能否扩充i和len
                len = lps[len-1];
            }
            else
            {
                // 这里, len在pattern最开始的位置, 无法和i位置配上,
                // 表示之前没有匹配的子串, 此位置也没有, 那么此位置最长相等前后缀就是0
                lps[i] = 0;
                i++;
            }  
        }
    }
    
    return lps;
}

std::vector<int> searchPatternKMP(std::string& str, std::string& pattern)
{
    std::vector<int> lpsArray = constructLPS(pattern);
    std::vector<int> locationsMatched;
    int strLength = str.length();
    int patternLength = pattern.length();
    int i = 0, j = 0;
    while (i < strLength)
    {
        if(str[i] == pattern[j])
        {
            // str 和 pattern的位置相等时, 移动指针到下一位
            i++, j++; // i遍历原串, j遍历模式串, i总是往后走, j做回溯
            if(j == patternLength)
            {
                // 模式串遍历完, 说明完全匹配, 记录位置
                locationsMatched.push_back(i-j);
                // j 之前++了, 所以这里j回溯到lps[j-1]
                // 不需要回溯到0, 因为在lps[j-1]所指示的位置之前, 能保证匹配
                j = lpsArray[j-1];
            }
        }
        else
        {
            // str 和 pattern的位置不匹配时, 保证主串不回溯
            if(j != 0)
            {
                // lps[j-1]所指示的位置之前是匹配的, 从这里开始
                j=lpsArray[j-1];
            }
            else
            {
                // 模式串最开始就和主串不匹配, 直接从主串的下一个位置开始匹配
                i++;
            }
        }
    }
    
    return locationsMatched;
}


// C++的库函数也可以直接进行匹配
std::vector<int> searchPatternLibFunction(std::string& str, std::string& pattern)
{
    char* firstOccurrence = strstr(str.c_str(), pattern.c_str()); // strstr只能查找子串第一次出现的位置
    std::vector<int> locationsMatched; // 用 std::string::find
    int pos = 0;
    while ((pos=str.find(pattern, pos)) != std::string::npos)
    {
        locationsMatched.push_back(pos);
        pos++;
    }
    
    return locationsMatched;
}

int main()
{
    std::string str = "ApatternBCDPatternpattpattern"; // 匹配结果: 1 22 
    std::string pattern = "pattern";

    std::vector<int> locationsMatched = searchPattern(str, pattern);
    for (int& elem : locationsMatched)
    {
        std::cout << elem << ' ';
    }
    std::cout << std::endl;

    locationsMatched = searchPatternKMP(str, pattern);
    for (int& elem : locationsMatched)
    {
        std::cout << elem << ' ';
    }
    std::cout << std::endl;

    return 0;
}


// aabaabc
// aabaabd