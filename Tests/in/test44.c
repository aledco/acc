extern void println(int n);

int main() {
    int a[2];
    int i;
    for (i = 0; i < 2; i++)
    {
        a[i] = i;
    }

    println(a[0]);
    println(a[1]);

    return 0;
}
