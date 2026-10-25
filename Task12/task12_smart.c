#define __USE_MINGW_ANSI_STDIO 1
#include <stdio.h>
#include <string.h>
#include <limits.h>

/* ---------- Tariff loaded at run time (no recompiling to change it) ---------- */
struct Tariff {
    long long slab1, slab2;            /* slab limits in kWh */
    long long rate1, rate2, rate3;     /* paise per kWh */
    long long fixed;                   /* paise */
    int peak_start, peak_end;          /* hours, peak is start <= hour < end */
    long long peak_percent;            /* extra percent during peak */
};

int load_tariff(const char *filename, struct Tariff *t)
{
    FILE *fp = fopen(filename, "r");
    char name[64];
    long long v;
    int found = 0;
    if (fp == NULL) { printf("Cannot open tariff file '%s'\n", filename); return 0; }
    while (fscanf(fp, "%63s %lld", name, &v) == 2) {
        if      (!strcmp(name, "SLAB1_UNITS"))        { t->slab1 = v; found++; }
        else if (!strcmp(name, "SLAB2_UNITS"))        { t->slab2 = v; found++; }
        else if (!strcmp(name, "RATE1_PAISE"))        { t->rate1 = v; found++; }
        else if (!strcmp(name, "RATE2_PAISE"))        { t->rate2 = v; found++; }
        else if (!strcmp(name, "RATE3_PAISE"))        { t->rate3 = v; found++; }
        else if (!strcmp(name, "FIXED_CHARGE_PAISE")) { t->fixed = v; found++; }
        else if (!strcmp(name, "PEAK_START_HOUR"))    { t->peak_start = (int)v; found++; }
        else if (!strcmp(name, "PEAK_END_HOUR"))      { t->peak_end = (int)v; found++; }
        else if (!strcmp(name, "PEAK_PERCENT"))       { t->peak_percent = v; found++; }
    }
    fclose(fp);
    if (found != 9) { printf("Tariff file incomplete: %d of 9 values\n", found); return 0; }
    if (t->slab1 < 0 || t->slab2 <= t->slab1 || t->rate1 < 0 || t->rate2 < 0 || t->rate3 < 0 ||
        t->fixed < 0 || t->peak_percent < 0 || t->peak_start < 0 || t->peak_end > 24 ||
        t->peak_start >= t->peak_end) {
        printf("Tariff file has invalid values\n");
        return 0;
    }
    return 1;
}

/* Rounded division, half up, for non-negative values */
long long div_round(long long num, long long den) { return (num + den / 2) / den; }

/* Monthly bill in paise. Energy is billed from watt-hours:
   paise = Wh * rate_per_kWh / 1000, rounded half up once per charge. */
long long bill_month(unsigned long long total_wh, unsigned long long peak_wh, const struct Tariff *t)
{
    long long wh = (long long)total_wh;
    long long s1 = t->slab1 * 1000, s2 = t->slab2 * 1000;
    long long in1 = wh < s1 ? wh : s1;
    long long in2 = wh > s1 ? (wh > s2 ? s2 : wh) - s1 : 0;
    long long in3 = wh > s2 ? wh - s2 : 0;
    long long energy = div_round(in1 * t->rate1 + in2 * t->rate2 + in3 * t->rate3, 1000);
    long long surcharge = 0;
    if (wh > 0)
        surcharge = div_round(energy * (long long)peak_wh * t->peak_percent, wh * 100);
    return t->fixed + energy + surcharge;
}

/* Simulates one month: 96 readings per day, every 15 minutes, each in Wh */
void simulate_month(int days, unsigned int off_wh, unsigned int peak_wh_per_reading,
                    const struct Tariff *t, unsigned long long *total, unsigned long long *peak)
{
    int d, i;
    *total = 0; *peak = 0;
    for (d = 0; d < days; d++)
        for (i = 0; i < 96; i++) {
            int hour = i / 4;
            unsigned int wh = (hour >= t->peak_start && hour < t->peak_end) ? peak_wh_per_reading : off_wh;
            *total += wh;
            if (hour >= t->peak_start && hour < t->peak_end)
                *peak += wh;
        }
}

int main(void)
{
    struct Tariff t;
    unsigned long long total, peak;
    long long bill, expected[3];
    int k;
    unsigned int off_wh[3], pk_wh[3], days[3];
    const char *names[3];

    if (!load_tariff("tariff12.cfg", &t)) return 1;

    printf("=== Part 1: largest values with smart meters (assume 10 kW maximum load) ===\n");
    {
        unsigned long long per_reading = 2500ULL;                 /* 10 kW x 0.25 h = 2500 Wh */
        unsigned long long per_year    = 35040ULL * per_reading;  /* one house, one year */
        unsigned long long taluk_year  = 60000ULL * per_year;
        unsigned long long dist_year   = 800000ULL * per_year;
        unsigned long long dist_reads  = 800000ULL * 35040ULL;
        unsigned long long month_wh    = 96ULL * 31ULL * per_reading;  /* one house, 31 days */
        printf("readings per house per year   : 365 x 96 = %d\n", 365 * 96);
        printf("max Wh in one reading         : %llu  (fits 16-bit 65535: %s)\n", per_reading, per_reading <= 65535ULL ? "yes" : "NO");
        printf("max Wh per house per month    : %llu  (fits unsigned int %u: %s)\n", month_wh, UINT_MAX, month_wh <= UINT_MAX ? "yes" : "NO");
        printf("max Wh per house per year     : %llu  (fits unsigned int: %s)\n", per_year, per_year <= UINT_MAX ? "yes" : "NO");
        printf("max Wh per taluk per year     : %llu  (fits unsigned int: %s)\n", taluk_year, taluk_year <= UINT_MAX ? "yes" : "NO");
        printf("max Wh per district per year  : %llu  (fits long long: %s)\n", dist_year, dist_year <= (unsigned long long)LLONG_MAX ? "yes" : "NO");
        printf("readings per district per year: %llu  (fits unsigned int: %s)\n", dist_reads, dist_reads <= UINT_MAX ? "yes" : "NO");
        printf("month Wh x rate 450 = %llu  (fits int %d: %s)\n", month_wh * 450ULL, INT_MAX, month_wh * 450ULL <= (unsigned long long)INT_MAX ? "yes" : "NO");
    }

    printf("\n=== Part 2: the original design in Wh ===\n");
    {
        unsigned long long seven_hours = 70000ULL;      /* 10 kW for 7 hours */
        unsigned short reg = (unsigned short)seven_hours;
        printf("70000 Wh stored in a 16-bit register = %u  (wrong, wrapped)\n", (unsigned)reg);
        printf("16-bit register rolls over every 65.535 kWh = 6.5 hours at 10 kW\n");
    }

    printf("\n=== Part 3: new bill with peak rule (tariff read from tariff12.cfg) ===\n");
    printf("Tariff: peak %02d:00-%02d:00 costs %lld%% more\n", t.peak_start, t.peak_end, t.peak_percent);
    names[0] = "Hand-checked month A (100 Wh off-peak, 400 Wh peak, 30 days)";
    names[1] = "Flat month B (100 Wh every reading, 30 days)";
    names[2] = "Zero consumption C (30 days)";
    off_wh[0] = 100; pk_wh[0] = 400; days[0] = 30; expected[0] = 143180;
    off_wh[1] = 100; pk_wh[1] = 100; days[1] = 30; expected[1] = 69170;
    off_wh[2] = 0;   pk_wh[2] = 0;   days[2] = 30; expected[2] = 5000;
    for (k = 0; k < 3; k++) {
        simulate_month((int)days[k], off_wh[k], pk_wh[k], &t, &total, &peak);
        bill = bill_month(total, peak, &t);
        printf("%s\n  total %llu Wh, peak %llu Wh -> Rs %lld.%02lld | expected (default tariff) Rs %lld.%02lld | %s\n",
               names[k], total, peak, bill / 100, bill % 100, expected[k] / 100, expected[k] % 100,
               bill == expected[k] ? "PASS" : "DIFFERENT (tariff file was changed)");
    }
    getchar();
    return 0;
}
