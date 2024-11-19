extern void printiln(int n);

void print_array_elem(int a[], int i)
{
    printiln(a[i]);
}

int main() {
    int a[3];
    a[0] = 0;
    a[1] = 1;
    a[2] = 2;

    print_array_elem(a, 1);
    return 0;
}