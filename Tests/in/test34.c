extern void printiln(int n);

int main()
{
    int x;
    x = 4;

    int y;
    int z;
    int a;
    int b;
    a = 0;
    b = 1;
    y = z = a += x += ++b;
    printiln(x);
    printiln(y);
    printiln(z);
    printiln(a);
    printiln(b);
    return 0;
}