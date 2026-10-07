// bdc 0x08a26040 GmoGimCheckHeader
#include "bdc.h"

/* Returns 1 when `data` starts with the GIM signature `"MIG.00.1PSP\0"` (little-endian words
   `0x2e47494d`, `0x312e3030`, `0x00505350`), else 0 (also for sizes 1..31). */

s32 GmoGimCheckHeader(const u32 *data, u32 size)

{
  if (data == NULL || size - 1 < 0x1f) {
    return 0;
  }
  if (data[0] == 0x2e47494d && data[1] == 0x312e3030 && data[2] == 0x505350) {
    return 1;
  }
  return 0;
}
