#include <stdio.h>
int main(void)
{
    int unused_total;
    long big = 5000000000L;
    int minus_one = -1;
    unsigned int one = 1u;
    printf("%d\n", big);
    if (minus_one < one)
        printf("less\n");
    return 0;
}
