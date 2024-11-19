extern void printiln(int n);

int sum(int a[5], int n)
{
    int sum = 0, i;
    for (i = 0; i < n; i++)
    {
        sum += a[i];
    }

    return sum;
}

int main() {
    int a[5];
    int i;
    for (i = 0; i < 5; i++)
    {
        a[i] = i + 1;
    }

    int s = sum(a, 5);
    printiln(s);
    return 0;
}