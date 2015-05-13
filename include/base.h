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

//分割字符串str，以split_char为分隔符
//最多max_num_str个缓冲区, 每个缓冲区最大max_len_str + 1个字节空间
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
            return false;
        }
        if(num == max_num_str)
        {
            return false;
        }
        *(split_str + num * max_len_str + pos) = str[i];
        pos++;
        if(pos == max_len_str + 1)
        {
            return false;
        }
    }
    if(i == len)
    {
        return false;
    }
    *num_str = num;
    return true;
}


#endif // BASE_H_INCLUDED
