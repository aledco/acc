extern void printiln(int n);

int main()
{
    int x;
    int y;

    x = 0;
    y = 1;

    if ((!(x == y) && x <= 0 && y < 1) || x == -1)
    {
        printiln(1);
    }
    else
    {
        printiln(0);
    }

    return 0;
}