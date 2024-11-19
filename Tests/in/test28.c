extern void printiln(int n);

int main()
{
    if ((1 && 0) || (1 && 1))
        printiln(1);
    
    if (0 || 0 || 0 || 0 || 0 || 0)
        printiln(0);
    
    if (0 || 0 || 1 || 0 || 0 || 0)
        printiln(1);

    if (1 && 1 && 0 && 1 && 1 && 1)
        printiln(0);
    
    if (1 && 1 && 1 && 1 && 1 && 1)
        printiln(1);

    if (0 < 1 && 1 > 0 && 0 <= 1 && 1 <= 1 && 1 >= 0 && 1 >= 1 && 0 == 0 && 1 == 1 && 0 != 1 && 1 != 0 && !(1 == 0) && !(0 == 1))
        printiln(1);
    
    return 0;
}