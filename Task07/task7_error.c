#include <stdio.h>

int main(void)
{
    long sizes[3];
    int k;
    sizes[0] = 4000L; sizes[1] = 60000L; sizes[2] = 800000L;

    printf("%-9s %-18s %-18s %-14s %-14s\n", "n bills", "exact (paise)", "float total (Rs)", "float error", "double error");
    for (k = 0; k < 3; k++) {
        long n = sizes[k];
        long i;
        long long exact_paise = 0;
        float f = 0.0f;
        double d = 0.0;
        for (i = 0; i < n; i++) {
            exact_paise += 16235LL;
            f += 162.35f;
            d += 162.35;
        }
        printf("%-9ld %-18lld %-18.2f %-14.2f %-14.6f\n", n, exact_paise, f,
               (double)f - exact_paise / 100.0, d - exact_paise / 100.0);
    }
    return 0;
}
