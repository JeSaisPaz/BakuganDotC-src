// bdc 0x089df238 GfxModelMotionIndexOfName
#include "bdc.h"

/* Returns the index of the motion called `name` for a GMO model object: when the model uses the
   global motion registry (`+0x13c`, set by `GfxModelEnableMotion`) the index comes from
   `GmoMotionIndexOfName` on `GmoMotionMgrGet`; otherwise the model's own motion chunk name
   table (`+0x108`, `+0xf4` entries, built by `GfxModelIndexChunks`) is searched with
   `strcasecmp`. -1 when not found. */

s32 GfxModelMotionIndexOfName(GfxModel *self, const char *name)

{
  void *mgr;
  s32 index;
  int cmp;
  int i;

  if (self->registryMotions != '\0') {
    mgr = GmoMotionMgrGet();
    index = GmoMotionIndexOfName(mgr,name);
    return index;
  }
  for (i = 0; i < self->motionCount; i++) {
    cmp = strcasecmp((const char *)self->motionChunks[i],name);
    if (cmp == 0) {
      return i;
    }
  }
  return -1;
}

