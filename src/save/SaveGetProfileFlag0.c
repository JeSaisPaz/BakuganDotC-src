// bdc 0x0880d314 SaveGetProfileFlag0
#include "bdc.h"

/* Returns the cached byte `g_profileFlag0` — bit 0 of profile word 0 — set by
   `SaveRefreshProfileFlag0`. */

u8 SaveGetProfileFlag0(void)

{
  return g_profileFlag0;
}

