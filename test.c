#include <stdio.h>

// int fact_itr(int n)
// {
//     int i;
//     i = 1;
//     while (n > 0)
//     {
//         i = i * n;
//         n = n - 1;
//     }

//     return i;
// }

int fact_itr(int n)
{
    int i;
    int m;
    i = 1;
    m = n;
    while (m > 0)
    {
        i = i * m;
        m = m - 1;
    }

    return i;
}

int main()
{
    for (int i = 0; i < 10; i++)
    {
        printf("%d\n", fact_itr(i));
    }

    return 0;
}