#ifndef TARIFF_H
#define TARIFF_H

/* Tariff constants (all money in paise) */
#define SLAB1_UNITS        100
#define SLAB2_UNITS        200
#define RATE1_PAISE        0
#define RATE2_PAISE        225
#define RATE3_PAISE        450
#define FIXED_CHARGE_PAISE 5000L

/* Debug mode: compile with  gcc -DDEBUG ...  to switch it on */
#ifdef DEBUG
#define DEBUG_PRINT(msg, value) printf("[DEBUG] %s = %ld\n", msg, (long)(value))
#else
#define DEBUG_PRINT(msg, value) ((void)0)
#endif

/* Function prototype */
long compute_bill(int units);

#endif /* TARIFF_H */
