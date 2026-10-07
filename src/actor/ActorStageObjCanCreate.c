// bdc 0x088a9364 ActorStageObjCanCreate
#include "bdc.h"

/* Always returns 1 (calls `SaveGetProfileFlag0` and ignores the result);
   `ActorStageObjCreateByKind` checks it before building. */

int ActorStageObjCanCreate(void)

{
  SaveGetProfileFlag0();
  return 1;
}

