// bdc 0x08a14534 GmoPlanTakeAttrs
#include "bdc.h"

/* Carves `n` 0x40-byte material attribute records (ctor `0x08a141c0`) from a model plan. */

void *GmoPlanTakeAttrs(int n, void *plan)
{
  return GmoPlanTakeArray(plan,0,0x10,0x40,n,GmoAttrCtor);
}

