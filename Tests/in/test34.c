extern void println(int n);

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
    println(x);
    println(y);
    println(z);
    println(a);
    println(b);
    return 0;
}