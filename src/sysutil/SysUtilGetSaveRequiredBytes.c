// bdc 0x089cc618 SysUtilGetSaveRequiredBytes
#include "bdc.h"

/* Returns `g_saveRequiredBytes`, the Memory Stick space the save needs; the save tasks print it
   in KB (`>> 10`) in the 'not enough space' message. */

u32 SysUtilGetSaveRequiredBytes(void)

{
  return g_saveRequiredBytes;
}

