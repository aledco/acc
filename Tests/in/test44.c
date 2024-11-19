extern void printiln(int n);

int main() {
    int a[2];
    int i;
    for (i = 0; i < 2; i++)
    {
        a[i] = i;
    }

    printiln(a[0]);
    printiln(a[1]);

    return 0;
}
