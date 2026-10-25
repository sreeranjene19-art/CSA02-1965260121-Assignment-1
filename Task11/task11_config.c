#include <stdio.h>
#include <string.h>

/* Tariff values loaded at start-up instead of #define */
struct Tariff {
    long slab1, slab2;
    long rate1, rate2, rate3;
    long fixed;
};

/* Reads "NAME value" lines. Returns 1 if all six values were found and are sensible. */
int load_tariff(const char *filename, struct Tariff *t)
{
    FILE *fp = fopen(filename, "r");
    char name[64];
    long value;
    int found = 0;

    if (fp == NULL) {
        printf("Cannot open tariff file '%s'\n", filename);
        return 0;
    }
    while (fscanf(fp, "%63s %ld", name, &value) == 2) {
        if      (strcmp(name, "SLAB1_UNITS") == 0)        { t->slab1 = value; found++; }
        else if (strcmp(name, "SLAB2_UNITS") == 0)        { t->slab2 = value; found++; }
        else if (strcmp(name, "RATE1_PAISE") == 0)        { t->rate1 = value; found++; }
        else if (strcmp(name, "RATE2_PAISE") == 0)        { t->rate2 = value; found++; }
        else if (strcmp(name, "RATE3_PAISE") == 0)        { t->rate3 = value; found++; }
        else if (strcmp(name, "FIXED_CHARGE_PAISE") == 0) { t->fixed = value; found++; }
    }
    fclose(fp);

    if (found != 6) {
        printf("Tariff file incomplete: found %d of 6 values\n", found);
        return 0;
    }
    if (t->slab1 < 0 || t->slab2 <= t->slab1 || t->rate1 < 0 || t->rate2 < 0 || t->rate3 < 0 || t->fixed < 0) {
        printf("Tariff file has invalid values\n");
        return 0;
    }
    return 1;
}

long compute_bill(long units, const struct Tariff *t)
{
    long paise = t->fixed;
    if (units > t->slab2)
        paise += (units - t->slab2) * t->rate3;
    if (units > t->slab1)
        paise += ((units > t->slab2 ? t->slab2 : units) - t->slab1) * t->rate2;
    paise += (units < t->slab1 ? units : t->slab1) * t->rate1;
    return paise;
}

int main(void)
{
    struct Tariff t;
    long tests[5];
    int i;
    tests[0] = 0; tests[1] = 100; tests[2] = 150; tests[3] = 200; tests[4] = 350;

    if (!load_tariff("tariff.cfg", &t))
        return 1;

    printf("Tariff loaded: slabs %ld/%ld, rates %ld/%ld/%ld paise, fixed %ld paise\n",
           t.slab1, t.slab2, t.rate1, t.rate2, t.rate3, t.fixed);
    for (i = 0; i < 5; i++) {
        long p = compute_bill(tests[i], &t);
        printf("%3ld units -> Rs %ld.%02ld\n", tests[i], p / 100, p % 100);
    }
    return 0;
}
