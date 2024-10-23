extern void println(int n);

int main()
{
    int x;
    for (x = 0; x < 100; x++)
        if (x % 5 == 0)
            println(x);

    return 0;
}
