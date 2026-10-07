// bdc 0x0886058c BtlBakuganNotifyContactStageObj
#include "bdc.h"

/* If the unit touches a stage object (`BtlBakuganGetContactStageObj` non-NULL), calls the empty
   stage-object hook `ActorStageObjNop` (the binary passes the object and the unit in a0/a1, which
   the empty callee ignores). */
void BtlBakuganNotifyContactStageObj(BtlBakugan *self)
{
    if (BtlBakuganGetContactStageObj(self) != NULL) {
        ActorStageObjNop();
    }
}
