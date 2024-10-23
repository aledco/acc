extern void println(int n);

int main()
{
    int x;
    x = 0;

    int y;
    y = x++;
    println(y);

    y = ++x;
    println(y);

    y = x--;
    println(y);

    y = x--;
    println(y);

    return 0;
}