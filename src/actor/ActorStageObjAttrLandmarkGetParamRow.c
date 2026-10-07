// bdc 0x088a6b30 ActorStageObjAttrLandmarkGetParamRow
#include "bdc.h"

/* Maps the attribute-landmark index `idx` (`kind - 0xbb`, clamped 0..17) to its parameter row
   through the 18-entry table `0x08abd544` (`0..5, 15..17, 6..14`: rows grouped per element and
   level). */

int ActorStageObjAttrLandmarkGetParamRow(ActorStageObjAttrLandmark *self, int idx)

{
  int rows [18];
  
  memcpy(rows,g_attrLandmarkParamRowTable,0x48);
  if (idx < 0) {
    idx = 0;
  }
  else if (0x11 < idx) {
    idx = 0x11;
  }
  return rows[idx];
}

