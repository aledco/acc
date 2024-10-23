extern void println(int n);

int main()
{
    int x;
    for (x = 0; x < 10; x++)
    {
        println(x);
    }

    for (x = 0; x > -10; --x)
    {
        println(x);
    }

    return 0;
}