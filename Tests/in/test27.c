extern void printiln(int n);

int main()
{
    int i;
    for (i = 0; i < 5; i = i + 1)
    {
        int j;
        for (j = i; j < 10; j = j + 1)
        {
            if (j == 4 || j == 8)
            {
                printiln(j);
            }
        }
    }

    return i;
}