#include <stdio.h>

#define NUM_SLABS 3
#define FIXED_CHARGE_PAISE 5000L

/* Upper limit of each slab (the last one has no limit) and its rate in paise */
static const long slab_limit[NUM_SLABS] = {100, 200, 1000000L};
static const long slab_rate[NUM_SLABS]  = {0, 225, 450};

/* Version 1: fixed if statements */
long bill_if(long units)
{
    long paise = FIXED_CHARGE_PAISE;
    if (units > 200)
        paise += (units - 200) * 450;
    if (units > 100)
        paise += ((units > 200 ? 200 : units) - 100) * 225;
    return paise;
}

/* Version 2: loop over the slab arrays */
long bill_loop(long units)
{
    long paise = FIXED_CHARGE_PAISE;
    long lower = 0;
    int s;
    for (s = 0; s < NUM_SLABS && units > lower; s++) {
        long upper = slab_limit[s];
        long in_slab = (units < upper ? units : upper) - lower;
        paise += in_slab * slab_rate[s];
        lower = upper;
    }
    return paise;
}

int main(void)
{
    long u;
    int mismatches = 0;
    for (u = 0; u <= 5000; u++) {
        if (bill_if(u) != bill_loop(u)) {
            printf("MISMATCH at %ld units\n", u);
            mismatches++;
        }
    }
    printf("150 units: if = %ld paise, loop = %ld paise\n", bill_if(150), bill_loop(150));
    printf("350 units: if = %ld paise, loop = %ld paise\n", bill_if(350), bill_loop(350));
    printf("Checked 0..5000 units, mismatches = %d\n", mismatches);
    return 0;
}
