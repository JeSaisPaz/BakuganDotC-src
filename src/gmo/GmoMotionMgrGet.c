// bdc 0x089d9644 GmoMotionMgrGet
#include "bdc.h"

/* Returns the motion manager singleton `g_gmoMotionMgr` (NULL before `GmoMotionMgrGetOrCreate` ran). */

void *GmoMotionMgrGet(void)
{
  return g_gmoMotionMgr;
}
