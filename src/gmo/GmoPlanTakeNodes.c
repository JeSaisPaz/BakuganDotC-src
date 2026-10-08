// bdc 0x08a145d4 GmoPlanTakeNodes
#include "bdc.h"

/* Carves `n` 0xc0-byte node records (alignment 0x40, ctor `0x08a13ff4`) from a model plan. */

void *GmoPlanTakeNodes(int n, void *plan)

{
  return GmoPlanTakeArray(plan,0,0x40,(int)sizeof(GmoNode),n,GmoNodeCtor);
}
