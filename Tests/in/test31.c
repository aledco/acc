extern void printiln(int n);

int main()
{
    int x;
    for (x = 0; x < 10; x++)
    {
        printiln(x);
    }

    for (x = 0; x > -10; --x)
    {
        printiln(x);
    }

    return 0;
}