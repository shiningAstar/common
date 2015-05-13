#include "stdio.h"
#ifndef BASE_H_INCLUDED
#define BASE_H_INCLUDED
#define unprintable(x) x < ' ' || x > '~'

//判断一个字符串是否合法
inline bool corr_string(const char *str, int length)
{
    if(str == NULL || length <= 0)
    {
        return false;
    }

    for(int i = length - 1; i >= 0; i--)
    {
        if(str[i] != 0)
        {
            continue;
        }
        goto correct;
    }
    return false;
correct:
    return true;
}

//
//
/**
    1、分割长度为len的字符串str，以split_char为分隔符
    2、最多max_num_str个缓冲区, 每个缓冲区最大max_len_str个字节空间
    3、调用函数生成二位数组split[max_num_str][max_len_str]，第四个参数调用时，传入 &split_str[0][0]，
    4、num_str为生成的字符串个数

*/
inline bool split_string(const char *str, int len, char split_char,
                         char *split_str, int max_num_str, int max_len_str,
                         int *num_str)
{
    int num = 0, pos = 0, i;
    if(str == NULL || len <= 0 || !corr_string(str, len) || unprintable(split_char) || split_str == NULL ||
       max_num_str <= 0 || max_len_str <= 0 || num_str == NULL)
    {
        return false;
    }
    for(i = 0; i < len; i++)
    {
        if(str[i] == split_char)
        {
            if(pos == 0)
            {
                continue;
            }
            *(split_str + num * max_len_str + pos) = 0;
            num++;
            pos = 0;
            continue;
        }
        if(str[i] == 0)
        {
            if(pos == 0)
            {
                break;
            }
            *(split_str + num * max_len_str + pos) = 0;
            num++;
            pos = 0;
            break;
        }
        if(unprintable(str[i]))
        {
           printf("the char in string is unprintable!\n");
            return false;
        }
        if(num == max_num_str)
        {
            printf("the value of max_num_str too small!\n");
            return false;
        }
        *(split_str + num * max_len_str + pos) = str[i];
        pos++;
        if(pos == max_len_str + 1)
        {
            printf("the value of max_len_str is too small!\n");
            return false;
        }
    }
    if(i == len)
    {
        printf("the value of max_num_str is too small!\n");
        return false;
    }
    *num_str = num;
    return true;
}


#endif // BASE_H_INCLUDED
