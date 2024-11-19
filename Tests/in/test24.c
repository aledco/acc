extern void printiln(int n);

int fact_rec(int n)
{
    if (n == 0)
    {
        return 1;
    }

    return n * fact_rec(n - 1);
}

int fact_itr(int n)
{
    int i;
    i = 1;
    while (n > 0)
    {
        i = i * n;
        n = n - 1;
    }

    return i;
}

int main()
{
    int x;
    int y;

    x = fact_rec(5);
    y = fact_itr(5);
    printiln(x);
    printiln(y);
    if (x != y)
    {
        return 1;
    }

    x = fact_rec(0);
    y = fact_itr(0);
    printiln(x);
    printiln(y);
    if (x != y)
    {
        return 1;
    }

    x = fact_rec(10);
    y = fact_itr(10);
    printiln(x);
    printiln(y);
    if (x != y)
    {
        return 1;
    }

    return 0;
}