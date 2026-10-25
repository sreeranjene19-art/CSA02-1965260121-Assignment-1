#include <stdio.h>
#include "tariff.h"

long compute_bill(int units)
{
    long paise = FIXED_CHARGE_PAISE;
    long slab2 = 0;
    long slab3 = 0;

    if (units > SLAB2_UNITS)
        slab3 = (long)(units - SLAB2_UNITS) * RATE3_PAISE;
    if (units > SLAB1_UNITS)
        slab2 = (long)((units > SLAB2_UNITS ? SLAB2_UNITS : units) - SLAB1_UNITS) * RATE2_PAISE;

    DEBUG_PRINT("units", units);
    DEBUG_PRINT("slab 2 charge (paise)", slab2);
    DEBUG_PRINT("slab 3 charge (paise)", slab3);

    paise += slab2 + slab3;
    DEBUG_PRINT("total (paise)", paise);
    return paise;
}

int main(void)
{
    int tests[] = {0, 150, 350};
    int n = sizeof tests / sizeof tests[0];
    int i;

    for (i = 0; i < n; i++) {
        long p = compute_bill(tests[i]);
        printf("%3d units -> Rs %ld.%02ld\n", tests[i], p / 100, p % 100);
    }
    return 0;
}
