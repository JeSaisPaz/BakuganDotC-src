// bdc 0x089cbdb0 SysUtilHandlerGetResult
#include "bdc.h"

/* Returns the SCE result word (`pspUtilityDialogCommon.result`, `+0x1c`) of the handler's parameter
   block (`*handler`), or 0 when the handler has none. Used by `SaveAutoSaveTaskUpdate`. */

s32 SysUtilHandlerGetResult(pspUtilityDialogCommon **handler)

{
  s32 result;

  result = 0;
  if (*handler != (pspUtilityDialogCommon *)0x0) {
    result = (*handler)->result;
  }
  return result;
}
