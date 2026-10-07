// bdc 0x0884c21c BtlMainSetFocusPointGlobal
#include "bdc.h"

/* Finds the battle main task (id 100, `CoreTaskFind`) and forwards to `BtlMainSetFocusPoint`.
    */

void BtlMainSetFocusPointGlobal(int value, float *pos)

{
  BtlMain *self = CoreTaskFind(100);

  if (self != NULL) {
    BtlMainSetFocusPoint(self, value, pos);
  }
}
