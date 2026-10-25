#define __USE_MINGW_ANSI_STDIO 1
#include <stdio.h>
#include <limits.h>
#include <float.h>
#include <time.h>

#define FIXED_CHARGE_PAISE 5000L

long compute_bill(long units)
{
    long paise = FIXED_CHARGE_PAISE;
    if (units > 200)
        paise += (units - 200) * 450;
    if (units > 100)
        paise += ((units > 200 ? 200 : units) - 100) * 225;
    return paise;
}

int main(void)
{
    long sizes[3];
    int k;
    long long int_limit = INT_MAX;
    sizes[0] = 4000L; sizes[1] = 60000L; sizes[2] = 800000L;

    printf("=== Part 1: float vs paise totals (units = (i*7919) %% 600) ===\n");
    printf("%-9s %-20s %-20s %-14s %-14s\n", "bills", "exact paise (LL)", "float total (Rs)", "float diff Rs", "double diff Rs");
    for (k = 0; k < 3; k++) {
        long n = sizes[k], i;
        long long exact = 0;
        float f = 0.0f;
        double d = 0.0;
        for (i = 0; i < n; i++) {
            long p = compute_bill((i * 7919LL) % 600LL);
            exact += p;
            f += (float)p / 100.0f;
            d += (double)p / 100.0;
        }
        printf("%-9ld %-20lld %-20.2f %-14.2f %-14.6f\n", n, exact, f,
               (double)f - exact / 100.0, d - exact / 100.0);
    }

    printf("\n=== Part 2: do int, long and long long overflow? ===\n");
    printf("sizeof(int)=%d  sizeof(long)=%d  sizeof(long long)=%d bytes\n",
           (int)sizeof(int), (int)sizeof(long), (int)sizeof(long long));
    for (k = 0; k < 3; k++) {
        long n = sizes[k], i;
        long long exact = 0;
        for (i = 0; i < n; i++)
            exact += compute_bill((i * 7919LL) % 600LL);
        printf("n=%-7ld exact total = %lld paise | int max %lld: %s | long max %ld: %s | long long max: %s\n",
               n, exact,
               int_limit, exact > int_limit ? "OVERFLOWS" : "fits",
               LONG_MAX, exact > (long long)LONG_MAX ? "OVERFLOWS" : "fits",
               "fits");
        printf("          if stored in an int it would wrap to %d\n", (int)(unsigned int)exact);
    }

    printf("\n=== Part 3: C standard minimum vs this system ===\n");
    printf("%-12s %-12s %s\n", "type", "std minimum", "this system");
    printf("%-12s %-12s %d bytes (%d bits), max %d\n", "short", "16 bits", (int)sizeof(short), (int)(sizeof(short) * CHAR_BIT), SHRT_MAX);
    printf("%-12s %-12s %d bytes (%d bits), max %d\n", "int", "16 bits", (int)sizeof(int), (int)(sizeof(int) * CHAR_BIT), INT_MAX);
    printf("%-12s %-12s %d bytes (%d bits), max %ld\n", "long", "32 bits", (int)sizeof(long), (int)(sizeof(long) * CHAR_BIT), LONG_MAX);
    printf("%-12s %-12s %d bytes (%d bits), max %lld\n", "long long", "64 bits", (int)sizeof(long long), (int)(sizeof(long long) * CHAR_BIT), LLONG_MAX);
    printf("%-12s %-12s %d digits\n", "float", "6 digits", FLT_DIG);
    printf("%-12s %-12s %d digits\n", "double", "10 digits", DBL_DIG);

    printf("\n=== Part 4: district total of 800000 bills kept in double paise ===\n");
    {
        long i;
        long long exact = 0;
        double dsum = 0.0;
        float fsum = 0.0f;
        for (i = 0; i < 800000L; i++) {
            long p = compute_bill((i * 7919LL) % 600LL);
            exact += p;
            dsum += (double)p;
            fsum += (float)p;
        }
        printf("long long total : %lld\n", exact);
        printf("double total    : %.0f   (difference %.0f)\n", dsum, dsum - (double)exact);
        printf("float total     : %.0f   (difference %.0f)\n", (double)fsum, (double)fsum - (double)exact);
        printf("double is exact for whole numbers up to 2^53 = 9007199254740992 paise\n");
    }

    printf("\n=== Part 5: time per addition (50 million additions each) ===\n");
    {
        volatile int a = 0;
        volatile long long b = 0;
        volatile double c = 0.0;
        long i;
        clock_t t0;
        double t_int, t_ll, t_dbl;
        t0 = clock(); for (i = 0; i < 50000000L; i++) a += 1; t_int = (double)(clock() - t0) / CLOCKS_PER_SEC;
        t0 = clock(); for (i = 0; i < 50000000L; i++) b += 1; t_ll  = (double)(clock() - t0) / CLOCKS_PER_SEC;
        t0 = clock(); for (i = 0; i < 50000000L; i++) c += 1.0; t_dbl = (double)(clock() - t0) / CLOCKS_PER_SEC;
        printf("int: %.3f s   long long: %.3f s   double: %.3f s\n", t_int, t_ll, t_dbl);
    }
    return 0;
}
