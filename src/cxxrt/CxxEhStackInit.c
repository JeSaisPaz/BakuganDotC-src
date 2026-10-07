// bdc 0x08a034ec CxxEhStackInit
#include "bdc.h"

/* Initialises the exception-object stack: the static region `g_cxxEhStaticRegion` gets the
   0x2000-byte buffer `g_cxxEhStaticBuffer` and becomes the current region
   (`g_cxxEhAllocRegion`). */

void CxxEhStackInit(void)

{
  CxxEhRegionInit(&g_cxxEhStaticRegion);
  g_cxxEhStaticRegion.base = g_cxxEhStaticBuffer;
  g_cxxEhStaticRegion.size = 0x2000;
  g_cxxEhStaticRegion.used = 0;
  g_cxxEhStaticRegion.heap = 0;
  g_cxxEhAllocRegion = &g_cxxEhStaticRegion;
}
