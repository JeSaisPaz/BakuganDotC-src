// bdc 0x089d9f20 GmoMotionCount
#include "bdc.h"

/* Number of registered motions: the length of the registry chain `g_gmoMotionRegistry`
   (`CoreNodeChainCount`) minus the payload-less root; 0 when the registry does not exist. */

s32 GmoMotionCount(void)

{
  s32 n;

  if (g_gmoMotionRegistry != (CoreNode *)0x0) {
    n = CoreNodeChainCount(g_gmoMotionRegistry);
    return n - 1;
  }
  return 0;
}

