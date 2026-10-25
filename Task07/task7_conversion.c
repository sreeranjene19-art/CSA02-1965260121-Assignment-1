#include <stdio.h>

int main(void)
{
    int units = 199;
    int minus_one = -1;
    unsigned int one = 1u;
    long long district = 22000000000LL;   /* 2.2 x 10^10 paise */
    double a = 0.1, b = 0.2;
    double charge = 0.29;                 /* rupees */

    printf("=== Surprise 1: integer division ===\n");
    printf("units / 100          = %d   (expected 1.99)\n", units / 100);
    printf("units / 100.0        = %.2f (one operand is double)\n", units / 100.0);

    printf("\n=== Surprise 2: signed and unsigned compared ===\n");
    printf("-1 < 1u is %d   (expected 1, because -1 is smaller)\n", minus_one < one);
    printf("-1 < 1  is %d\n", minus_one < 1);

    printf("\n=== Surprise 3: casting a large value to int ===\n");
    printf("district total (long long) = %lld\n", district);
    printf("(int) district total       = %d\n", (int)district);

    printf("\n=== Surprise 4: 0.1 + 0.2 in double ===\n");
    printf("0.1 + 0.2 == 0.3 is %d   (expected 1)\n", (a + b) == 0.3);
    printf("0.1 + 0.2 = %.17f\n", a + b);

    printf("\n=== Rounding fractional rupees to paise ===\n");
    printf("(long)(0.29 * 100)         = %ld   (truncated, wrong)\n", (long)(charge * 100));
    printf("(long)(0.29 * 100 + 0.5)   = %ld   (rounded half up, correct)\n", (long)(charge * 100 + 0.5));
    return 0;
}
