// bdc 0x0886450c BtlBakuganIsGuardableHit
#include "bdc.h"

/* Returns 1 for hit ids `BtlBakuganOnHit` treats as guardable: basic hits below 0x18, 0xb3
   while the unit's `guardAge` is below 5, 0xb8, 0x20, attack hits 0x23..0xb2 whose
   `BtlAttackParams` `noGuardFlag` is clear (`BtlAttackParamsIsFlag08Clear` on `hitId - 0x23`),
   and 0x94; 0x53 and every other id return 0. */
int BtlBakuganIsGuardableHit(BtlBakugan *self, int hitId)
{
    if (hitId < 0x18) {
        return 1;
    }
    if (self->guardAge < 5 && hitId == 0xb3) {
        return 1;
    }
    if (hitId == 0xb8) {
        return 1;
    }
    if (hitId == 0x53) {
        return 0;
    }
    if (hitId == 0x20) {
        return 1;
    }
    if (hitId > 0x22 && hitId < 0xb3 && BtlAttackParamsIsFlag08Clear(hitId - 0x23) != 0) {
        return 1;
    }
    return hitId == 0x94;
}
