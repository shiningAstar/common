#ifndef BASE_H_INCLUDED
#define BASE_H_INCLUDED

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


#endif // BASE_H_INCLUDED
