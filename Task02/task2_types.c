#include <stdio.h>
#include <limits.h>
#include <float.h>

int main(void)
{
    printf("=== Part A: sizes and ranges on this system ===\n");
    printf("%-20s %-6s %-26s %s\n", "Type", "Bytes", "Minimum", "Maximum");
    printf("%-20s %-6zu %-26d %d\n", "char", sizeof(char), CHAR_MIN, CHAR_MAX);
    printf("%-20s %-6zu %-26d %d\n", "short", sizeof(short), SHRT_MIN, SHRT_MAX);
    printf("%-20s %-6zu %-26d %u\n", "unsigned short", sizeof(unsigned short), 0, (unsigned)USHRT_MAX);
    printf("%-20s %-6zu %-26d %d\n", "int", sizeof(int), INT_MIN, INT_MAX);
    printf("%-20s %-6zu %-26d %u\n", "unsigned int", sizeof(unsigned int), 0, UINT_MAX);
    printf("%-20s %-6zu %-26ld %ld\n", "long", sizeof(long), LONG_MIN, LONG_MAX);
    printf("%-20s %-6zu %-26lld %lld\n", "long long", sizeof(long long), LLONG_MIN, LLONG_MAX);
    printf("%-20s %-6zu %-26e %e\n", "float", sizeof(float), FLT_MIN, FLT_MAX);
    printf("%-20s %-6zu %-26e %e\n", "double", sizeof(double), DBL_MIN, DBL_MAX);
    printf("float digits of precision: %d, double: %d\n", FLT_DIG, DBL_DIG);

    printf("\n=== Part B: float rupees vs integer paise, 4000 bills of Rs 162.35 ===\n");
    float total_float = 0.0f;
    long total_paise = 0;
    for (int i = 0; i < 4000; i++) {
        total_float += 162.35f;   /* money as float rupees */
        total_paise += 16235L;    /* money as integer paise */
    }
    printf("Float total   : Rs %.2f\n", total_float);
    printf("Paise total   : Rs %ld.%02ld\n", total_paise / 100, total_paise % 100);
    printf("Correct total : Rs 649400.00\n");
    printf("Float error   : Rs %.2f\n", total_float - 649400.0f);

    printf("\n=== Part C: 16-bit register rollover ===\n");
    unsigned short m = 65535;
    printf("65535 + 1 stored in unsigned short = %u\n", (unsigned)(unsigned short)(m + 1));
    return 0;
}
