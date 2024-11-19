#include <stdio.h>

int main()
{
    char s1[] = "hello";
    char *s2 = "world";
    s1[0] = 'e';
    printf("%s, %s\n", s1, s2);
    return 0;
}
