#include <stdio.h>

/* ---------- Tariff constants (all money in paise) ---------- */
#define SLAB1_UNITS         100
#define SLAB2_UNITS         200
#define RATE1_PAISE         0
#define RATE2_PAISE         225
#define RATE3_PAISE         450
#define FIXED_CHARGE_PAISE  5000L

#define METER_MAX           65535L
#define MAX_PLAUSIBLE_UNITS 5000L   /* largest monthly consumption accepted */

#define MAX(a, b) (((a) > (b)) ? (a) : (b))

/* Bill in paise for a given number of units */
long compute_bill(long units)
{
    long paise = FIXED_CHARGE_PAISE;
    if (units > SLAB2_UNITS)
        paise += (units - SLAB2_UNITS) * RATE3_PAISE;
    if (units > SLAB1_UNITS)
        paise += ((units > SLAB2_UNITS ? SLAB2_UNITS : units) - SLAB1_UNITS) * RATE2_PAISE;
    paise += (units < SLAB1_UNITS ? units : SLAB1_UNITS) * RATE1_PAISE;
    return paise;
}

/* Units consumed, allowing one meter rollover.
   Returns 1 if valid, 0 if the readings are not acceptable. */
int get_units(long previous, long current, long *units)
{
    long u;
    if (previous < 0 || previous > METER_MAX || current < 0 || current > METER_MAX)
        return 0;
    if (current >= previous)
        u = current - previous;
    else
        u = (METER_MAX + 1 - previous) + current;   /* rolled over past 65535 */
    if (u > MAX_PLAUSIBLE_UNITS)                    /* "rollover" would be implausible */
        return 0;
    *units = u;
    return 1;
}

int main(void)
{
    char line[128];
    long household, previous, current, units, paise;
    char category;
    long long grand_total = 0;
    int valid = 0, invalid = 0, lineno = 0;

    printf("Grama Jyothi Rural Electricity Co-operative - Billing\n");
    printf("Enter: household category previous current   (Ctrl+Z then Enter to finish)\n\n");

    while (fgets(line, sizeof line, stdin) != NULL) {
        lineno++;
        if (sscanf(line, "%ld %c %ld %ld", &household, &category, &previous, &current) != 4) {
            printf("Line %d: INVALID INPUT (not four values, or letters where a number is needed)\n", lineno);
            invalid++;
            continue;
        }
        if (previous < 0 || current < 0) {
            printf("Line %d: house %ld: INVALID - negative reading\n", lineno, household);
            invalid++;
            continue;
        }
        if (!get_units(previous, current, &units)) {
            printf("Line %d: house %ld: INVALID - reading out of range, or current less than previous without roll-over\n",
                   lineno, household);
            invalid++;
            continue;
        }
        paise = compute_bill(units);
        grand_total += paise;
        valid++;
        printf("House %-5ld %c  prev %5ld  curr %5ld  units %4ld  fixed Rs %ld.%02ld  energy Rs %ld.%02ld  TOTAL Rs %ld.%02ld\n",
               household, category, previous, current, units,
               FIXED_CHARGE_PAISE / 100, FIXED_CHARGE_PAISE % 100,
               (paise - FIXED_CHARGE_PAISE) / 100, (paise - FIXED_CHARGE_PAISE) % 100,
               paise / 100, paise % 100);
    }

    printf("\nValid bills: %d   Rejected lines: %d\n", valid, invalid);
    printf("Total of valid bills: Rs %lld.%02lld\n", grand_total / 100, grand_total % 100);
    return 0;
}
