extern void printi(int n);
extern void printiln(int n);

void cpy(char s1[], char s2[])
{
    int i = 0;
    while (s1[i] != 0)
    {
        s2[i] = s1[i];
        i++;
    }
}

int main() {
    char s1[6] = "hello";
    char s2[6];

    cpy(s1, s2);

    int i = 0;
    while (s1[i] != 0)
    {
        printi(s1[i]);
        printiln(s2[i]);
        i++;
    }

    return 0;
}