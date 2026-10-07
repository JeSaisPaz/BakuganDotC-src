// bdc 0x089cc040 SysUtilGetCell
#include "bdc.h"

/* Returns the selection cell pointer of the manager (`*g_sysUtilMng`). Only valid after
   `SysUtilIsInit`. */

u32 *SysUtilGetCell(void)

{
  return g_sysUtilMng->cell;
}

