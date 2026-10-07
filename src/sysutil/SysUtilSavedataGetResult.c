// bdc 0x089cc758 SysUtilSavedataGetResult
#include "bdc.h"

/* Returns the result word of the last savedata request (`+0x6b0` of the savedata parameter block
   `0x08ac5908`). The save tasks store it in profile word 1 (`SaveProfileSetWord`); 1 means
   success, 7-8 'not enough space' (they then show the required size from
   `SysUtilGetSaveRequiredBytes`). */

s32 SysUtilSavedataGetResult(void)

{
  return g_savedataParams->result;
}

