#include <stdio.h>

/* Safe macros: every argument and the whole expression is parenthesised */
#define RUPEES_TO_PAISE(r) ((r) * 100L)
#define SQUARE(x)          ((x) * (x))
#define MAX(a, b)          (((a) > (b)) ? (a) : (b))

/* Unsafe versions, kept only to demonstrate the pitfalls */
#define BAD_SQUARE(x)      x * x
#define BAD_MAX(a, b)      a > b ? a : b

/* Function versions (static inline) */
static inline int square_fn(int x) { return x * x; }
static inline int max_fn(int a, int b) { return (a > b) ? a : b; }

int main(void)
{
    int a = 4;
    int i, j;
    int r;

    printf("=== Safe macros ===\n");
    printf("RUPEES_TO_PAISE(50)   = %ld paise\n", RUPEES_TO_PAISE(50));
    printf("RUPEES_TO_PAISE(2+3)  = %ld paise (expected 500)\n", RUPEES_TO_PAISE(2 + 3));
    printf("SQUARE(a+1), a=4      = %d (expected 25)\n", SQUARE(a + 1));
    printf("MAX(150, 200)         = %d\n", MAX(150, 200));

    printf("\n=== Pitfall 1: missing parentheses ===\n");
    printf("BAD_SQUARE(a+1), a=4  = %d (expands to 4+1*4+1, expected 25)\n", BAD_SQUARE(a + 1));

    printf("\n=== Pitfall 2: argument evaluated twice ===\n");
    i = 5; j = 3;
    r = MAX(i++, j);
    printf("MAX(i++, j) with i=5, j=3 -> result = %d, i = %d (expected result 5, i 6)\n", r, i);
    i = 5; j = 3;
    r = max_fn(i++, j);
    printf("max_fn(i++, j) with i=5, j=3 -> result = %d, i = %d (correct)\n", r, i);

    printf("\n=== Function versions ===\n");
    printf("square_fn(a+1) = %d\n", square_fn(a + 1));
    printf("max_fn(150, 200) = %d\n", max_fn(150, 200));
    return 0;
}
