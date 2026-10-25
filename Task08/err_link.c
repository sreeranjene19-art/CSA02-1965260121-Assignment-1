#include <stdio.h>
long compute_bill(int units);   /* declared but never defined */
int main(void)
{
    printf("%ld\n", compute_bill(150));
    return 0;
}
