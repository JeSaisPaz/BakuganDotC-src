// bdc 0x08a1e31c GmoModelInitBaseMotion
#include "bdc.h"

/* Calls `GmoModelBuildBaseMotion` and ignores its result. */

void GmoModelInitBaseMotion(GmoModel *self)

{
  GmoModelBuildBaseMotion(self);
  return;
}

