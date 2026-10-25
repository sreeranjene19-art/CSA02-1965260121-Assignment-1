# Task 1: Understand the Problem and Identify the Data (K4 - Analyse)

## 1. Inputs and outputs

| Item | Kind | Why it is needed | Nature of the value 
| Household number | Input | Identifies whose bill it is. About 800,000 houses at district scale. | Whole number, never negative, too big for 16 bits |
| Category (e.g. D = domestic) | Input | Lets the program apply the right tariff later | Single character |
| Previous reading | Input | Start of the billing period | Whole number 0 to 65535 (16-bit register) |
| Current reading | Input | End of the billing period | Whole number 0 to 65535 |
| Units consumed | Output | Basis for the slab charge | Whole number 0 to 65535, never negative |
| Energy charge | Output | Slab-wise charge for the units used | Fractional in rupees (Rs 2.25), so whole in paise |
| Fixed charge | Output | Charge applied to every bill (Rs 50) | Constant, 5000 paise |
| Total bill | Output | Amount the household pays | Fixed charge + energy charge, in paise |

## 2. Values that never change during a run (constants)

| Constant | Value | Reason |
| First slab limit | 100 units | Tariff rule |
| Second slab limit | 200 units | Tariff rule |
| Rate, slab 1 | 0 paise/unit | First 100 units are free |
| Rate, slab 2 | 225 paise/unit | Rs 2.25 |
| Rate, slab 3 | 450 paise/unit | Rs 4.50 |
| Fixed charge | 5000 paise | Rs 50 |
| Paise per rupee | 100 | Needed to print rupees and paise |
| Meter maximum | 65535 | 16-bit register limit, where rollover happens |

These are written once, so a tariff revision changes one place only. This avoids the earlier problem where the tariff was typed in many places and applied only partly.

## 3. Which values can be negative, fractional or very large

- **Negative:** `current - previous` is negative when the meter rolls over (65400 to 120 gives -65280). The program must treat this as a rollover, not as a negative consumption. The final bill must never be negative.
- **Fractional:** the rate Rs 2.25 is fractional in rupees. Storing money as float rupees causes errors of a few paise, which is what happened in the spreadsheet. Storing whole paise (225) removes the fraction.
- **Very large:**
  - Maximum units per bill = 65535.
  - Maximum single bill = 5000 + 100 x 225 + 65335 x 450 = 29,428,250 paise (about Rs 2,94,282.50). This fits in a 32-bit `long`.
  - District total: 800,000 bills x an average bill of about Rs 275 (27,500 paise) = 2.2 x 10^10 paise. This is more than the 32-bit limit of about 2.147 x 10^9, so the total needs `long long`.

## 4. Rules every correct bill must satisfy

1. Units consumed >= 0 and <= 65535.
2. Total = fixed charge + energy charge (the total equals the sum of its parts).
3. Total >= fixed charge, so a bill is never negative.
4. The energy charge never decreases as units increase.
5. Bills are exact to the paisa (integer paise, no rounding error). The paise part printed is always 00 to 99.
6. A meter rollover (e.g. 65535 to 0) gives the correct units, not a negative bill.
7. Invalid input (letters, negative reading, reading outside 0 to 65535) is rejected, and no bill is produced.
8. Changing one tariff constant changes all affected bills consistently.

## 5. Assumptions

- Readings are whole units (kWh) in the range 0 to 65535.
- At most one rollover happens in a billing month.
- A current reading smaller than the previous one is accepted as a rollover only if the implied consumption is plausible (not above a set limit). Otherwise it is treated as invalid input.
- Money is calculated in paise throughout and converted to rupees only when printed.
- All households in this assignment use one tariff category. The category is stored so other tariffs can be added.
- Tariff slabs and the fixed charge are the same for every household in the run.

## 6. Alternative concept and complexity analysis: readings stored as text

**How the solution would change.** Each reading would be stored as a character string such as "01530" (d = 5 digits plus a terminating '\0'). To compute units, the program would convert both strings to numbers (a loop such as `value = value*10 + (ch - '0')`), then subtract. Comparing two readings would use string comparison, or conversion first.

Pseudocode:

    for each reading string s:
        value = 0
        for i = 0 to d-1:
            value = value*10 + (s[i] - '0')
    units = (current - previous) mod 65536

**Time (d = number of digits):**

| Operation | Integer | Text |
|---|---|---|
| Read the value | O(1) | O(d) to convert |
| Compare two readings | O(1) | O(d) character by character |
| Subtract | O(1) | O(d) after conversion (or digit-by-digit) |

Since d = 5 is fixed here, both are effectively constant, but the text version does about 5 times more work per reading and still has to convert before computing the bill.

**Space per reading:**

| Representation | Bytes per reading | 4,000 houses (2 readings each) | 800,000 houses (2 readings each) |
|---|---|---|---|
| `unsigned short` | 2 | 16,000 B | 3.2 MB |
| String of 5 digits + '\0' | 6 | 48,000 B | 9.6 MB |

Space is O(1) per reading for the integer and O(d) for the string, so the text form uses 3 times as much memory.

**Conclusion.** Integers are faster, smaller and make rollover arithmetic simple. Text is only useful for preserving leading zeros and for catching non-digit characters at input time. Because it still needs conversion, the integer representation is the better choice for this program.
