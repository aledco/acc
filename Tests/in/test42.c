extern void println(int n);

int x;
int y;

void f()
{
    x = x + 1;
}

void g()
{
    y = y * 2;
}

void h()
{
    x = x + y;
    y = y - x;
}

int main()
{
    x = 1;
    y = 1;
    int i;
    for (i = 0; i < 10; i++)
    {
        f();
        g();
        h();
    }

    println(x);
    println(y);
    return 0;
}
