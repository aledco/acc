extern void println(int n);

int main()
{
    if ((1 && 0) || (1 && 1))
        println(1);
    
    if (0 || 0 || 0 || 0 || 0 || 0)
        println(0);
    
    if (0 || 0 || 1 || 0 || 0 || 0)
        println(1);

    if (1 && 1 && 0 && 1 && 1 && 1)
        println(0);
    
    if (1 && 1 && 1 && 1 && 1 && 1)
        println(1);

    if (0 < 1 && 1 > 0 && 0 <= 1 && 1 <= 1 && 1 >= 0 && 1 >= 1 && 0 == 0 && 1 == 1 && 0 != 1 && 1 != 0 && !(1 == 0) && !(0 == 1))
        println(1);
    
    return 0;
}