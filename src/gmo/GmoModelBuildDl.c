// bdc 0x089dafcc GmoModelBuildDl
#include "bdc.h"

/* Builds the GE display list that draws a GMO model into the caller's buffer (`*buf`, capacity
   `*size`): models with a prebuilt list (`dlCache` set and flag 8 in `flags28`) go to
   `GmoModelBuildCachedDl`; otherwise a 0x1e0-byte draw context is set up on the stack
   (`GmoDlCtxInit`), the nodes are drawn (`GmoDlDrawModel`, dirty mask -1), the GE state reset
   list is appended (`GmoDlWriteResetState`) and the list is closed (`GmoDlCtxFinish`, whose
   result is returned). Returns 0 for a null model. */

u32 GmoModelBuildDl(GmoModel *self, void **buf, u32 *size, u32 arg, s16 *nodes, s32 count, s32 stride)

{
  GmoDlContext ctx;

  if (self == NULL) {
    return 0;
  }
  if (self->dlCache != NULL && (self->flags28 & 8) != 0) {
    return GmoModelBuildCachedDl(self, (u32 **)buf, size);
  }
  GmoDlCtxInit(&ctx, buf, size, arg);
  GmoDlDrawModel(&ctx, self, 0xffffffff, nodes, count, stride);
  GmoDlWriteResetState(&ctx, self);
  return GmoDlCtxFinish(&ctx, buf, (s32 *)size);
}
