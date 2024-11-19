extern void printiln(int n);

int main()
{
    int x;
    x = 0;

    int y;
    y = x++;
    printiln(y);

    y = ++x;
    printiln(y);

    y = x--;
    printiln(y);

    y = x--;
    printiln(y);

    return 0;
}