// bdc 0x089dafa8 GmoModelBuildDlWrapper
#include "bdc.h"

/* Thin wrapper that forwards its arguments to `GmoModelBuildDl`; called by
   `GfxModelDlWriteState`. */

u32 GmoModelBuildDlWrapper(GmoModel *self, void **buf, u32 *size, u32 arg)

{
  return GmoModelBuildDl(self, buf, size, arg, (s16 *)0x0, 0, 0);
}

