extern void printiln(int n);

int main()
{
    int i;
    i = 0;
    while (i < 5)
    {
        int j;
        j = i;
        while (j < 10)
        {
            printiln(j);
            j = j + 2;
        }

        i = i + 1;
    }

    return 0;
}