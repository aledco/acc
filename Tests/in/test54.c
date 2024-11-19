extern void println(int n);

void print_array(char a[], int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        println(a[i]);
    }
}

int main() {
    char a[10];
    int i;
    for (i = 0; i < 10; i++)
    {
        a[i] = i * i * i;
    }

    print_array(a, 10);
    return 0;
}