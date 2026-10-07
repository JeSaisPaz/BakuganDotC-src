// bdc 0x08870234 BtlCountKindsHasDuplicate
#include "bdc.h"

/* Counts how often each unit kind occurs in `kinds` (at most 4 entries, stopping early at -1) into
   the 0x15-byte array `counts` (zeroed first); kinds 0 and >= 0x15 are skipped. Returns 1 when any
   counted kind occurs more than once, else 0. Used by `BtlBakuganTexLoaderTaskUpdate` and
   `BtlKindsHaveDuplicate`. */
int BtlCountKindsHasDuplicate(int *kinds, u8 *counts)
{
    int dup = 0;
    int i;

    memset(counts, 0, 0x15);
    for (i = 0; i < 4; i++) {
        int kind = kinds[i];

        if (kind == -1) {
            break;
        }
        if (kind < 0x15 && kind != 0) {
            counts[kind]++;
            if (counts[kind] > 1) {
                dup = 1;
            }
        }
    }
    return dup;
}
