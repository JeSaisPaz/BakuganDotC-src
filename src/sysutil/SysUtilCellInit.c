// bdc 0x089cc050 SysUtilCellInit
#include "bdc.h"

/* Initialises the 4-byte selection cell of the system-utility manager (`g_sysUtilMng``+0x0`):
   `*cell = 0`. Returns `cell`. */

u32 *SysUtilCellInit(u32 *cell)

{
  *cell = 0;
  return cell;
}

