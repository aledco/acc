extern void printiln(int n);

int main() {
    int a[5];
    int i;
    for (i = 0; i < 5; i++)
    {
        a[i] = i + 1;
    }

    for (i = 0; i < 5; i++)
    {
        int x = a[i];
        printiln(x);
    }

    return 0;
}