#include<stdio.h>

int fun(int x, int y, int z)
{
    int val, u;
    val = z-y;
    u = (val >> 31) & (~1);
    val += u;
    val >>= 1;
    u = val + y;
    if(u == x)
        return 0;
    if(u > x)
    {
        z = u-1;
        val = fun(x, y, z);
        val *= 2;
    }
    else
    {
        y = u+1;
        val = fun(x, y, z);
        val = val*2+1;
    }
    return val;
}

int main()
{
    int res;
    for(int i = 0;i <= 14; ++i)
    {
        res = fun(i, 0, 14);
        if(res == 0)
            printf("%d ", i);
    }
    printf("\n");
    //system("pause");
    return 0;
}