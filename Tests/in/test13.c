extern void printiln(int n);

int add(int x, int y)
{
    x = 3;
    y = 4;
    return x + y;
}

int main()
{
    int x;
    x = add(1, 2);
    printiln(x);
    return x;
}