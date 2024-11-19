extern void printiln(int n);

int main()
{
    int x;
    for (x = 0; x < 100; x++)
        if (x % 5 == 0)
            printiln(x);

    return 0;
}
