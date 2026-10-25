#include <stdio.h>

#define SLAB1_UNITS        100
#define SLAB2_UNITS        200
#define RATE2_PAISE        225
#define RATE3_PAISE        450
#define FIXED_CHARGE_PAISE 5000L

long compute_bill(long units)
{
    long paise = FIXED_CHARGE_PAISE;
    if (units > SLAB2_UNITS)
        paise += (units - SLAB2_UNITS) * RATE3_PAISE;
    if (units > SLAB1_UNITS)
        paise += ((units > SLAB2_UNITS ? SLAB2_UNITS : units) - SLAB1_UNITS) * RATE2_PAISE;
    return paise;
}

/* Reads a whole number between 0 and max. Returns 1 on success, 0 on end of input. */
int read_number(const char *prompt, long max, long *out)
{
    long v;
    int r, ch;
    for (;;) {
        printf("%s", prompt);
        r = scanf("%ld", &v);
        if (r == EOF)
            return 0;
        if (r != 1) {                          /* letters or symbols typed */
            printf("  Invalid input: please type digits only.\n");
            while ((ch = getchar()) != '\n' && ch != EOF)
                ;                              /* throw away the bad line */
            continue;
        }
        if (v < 0 || v > max) {
            printf("  Invalid input: value must be between 0 and %ld.\n", max);
            continue;
        }
        *out = v;
        return 1;
    }
}

int main(void)
{
    long household, previous, current, units, paise;
    char category;
    long long big = 5000000000LL;

    printf("=== Part A: format specifiers ===\n");
    printf("%%d  int            : %d\n", -45);
    printf("%%u  unsigned int   : %u\n", 65535u);
    printf("%%ld long           : %ld\n", 29428250L);
    printf("%%lld long long     : %lld\n", big);
    printf("%%f  float          : %f\n", 2.25f);
    printf("%%lf double         : %lf\n", 162.5);
    printf("%%c  char           : %c\n", 'D');
    printf("%%s  string         : %s\n", "Grama Jyothi");

    printf("\n=== Part B: wrong specifier test ===\n");
    printf("long long value is 5000000000\n");
    printf("printed with %%lld (correct) : %lld\n", big);
    printf("printed with %%d  (wrong)   : %d\n", (int)big);

    printf("\n=== Part C: validated input and bill ===\n");
    if (!read_number("Household number (0-999999): ", 999999L, &household)) return 1;
    printf("Category (one letter, e.g. D): ");
    if (scanf(" %c", &category) != 1) return 1;
    if (!read_number("Previous reading (0-65535): ", 65535L, &previous)) return 1;
    if (!read_number("Current reading  (0-65535): ", 65535L, &current)) return 1;

    units = (long)(unsigned short)(current - previous);
    paise = compute_bill(units);

    printf("\n--- BILL ---\n");
    printf("Household : %ld   Category: %c\n", household, category);
    printf("Units     : %ld\n", units);
    printf("Fixed     : Rs %ld.%02ld\n", FIXED_CHARGE_PAISE / 100, FIXED_CHARGE_PAISE % 100);
    printf("Energy    : Rs %ld.%02ld\n", (paise - FIXED_CHARGE_PAISE) / 100, (paise - FIXED_CHARGE_PAISE) % 100);
    printf("TOTAL     : Rs %ld.%02ld\n", paise / 100, paise % 100);
    return 0;
}
