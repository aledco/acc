extern void println(int n);

int main()
{
    int x;
    int y;

    x = 0;
    y = 1;

    if ((!(x == y) && x <= 0 && y < 1) || x == -1)
    {
        println(1);
    }
    else
    {
        println(0);
    }

    return 0;
}