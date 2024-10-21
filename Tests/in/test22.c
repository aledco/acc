extern void println(int n);

int main()
{
    int i;
    i = 0;
    while (i < 5)
    {
        int j;
        j = i;
        while (j < 10)
        {
            if (j == 4 || j == 8)
            {
                println(j);
            }
            
            j = j + 1;
        }

        i = i + 1;
    }

    return i;
}