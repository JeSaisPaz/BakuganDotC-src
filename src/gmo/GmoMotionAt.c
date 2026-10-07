// bdc 0x089d9dac GmoMotionAt
#include "bdc.h"

/* Returns the `index`-th entry behind the root of `g_gmoMotionRegistry` (0-based), or NULL when the
   chain is shorter. Ghidra shows it as `void` because the result is left in a register. */

CoreNode *GmoMotionAt(void *mgr, s32 index)

{
  CoreNode *node;
  s32 i;

  node = g_gmoMotionRegistry->next;
  for (i = 0; node != (CoreNode *)0x0 && i != index; i++) {
    node = node->next;
  }
  return node;
}

