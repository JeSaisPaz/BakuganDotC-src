// bdc 0x089cc018 SysUtilIsInit
#include "bdc.h"

/* Returns whether the system-utility manager exists and has its selection cell (`g_sysUtilMng !=
   NULL && *g_sysUtilMng != 0`). */

bool SysUtilIsInit(void)

{
  bool result;

  result = false;
  if ((g_sysUtilMng != (SysUtilMng *)0x0) && (g_sysUtilMng->cell != (u32 *)0x0)) {
    result = true;
  }
  return result;
}
