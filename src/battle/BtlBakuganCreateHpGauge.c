// bdc 0x08862b64 BtlBakuganCreateHpGauge
#include "bdc.h"

/* Allocates the unit's HUD hit-point gauge: 0xa0 bytes from the low end of the heap
   (`MemSetAllocFromLow``(true)` under `MemLock`, previous policy restored), constructs it with
   `UiHpGaugeInit``(gauge, self)` and stores the gauge, or NULL when the allocation failed, in
   `self->hpGauge`. */

void BtlBakuganCreateHpGauge(BtlBakugan *self)
{
    bool wasFromLow;
    UiHpGauge *gauge;

    MemLock();
    wasFromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    gauge = MemAlloc(0xa0, NULL, 0);
    MemSetAllocFromLow(wasFromLow);
    MemUnlock();
    if (gauge != NULL) {
        UiHpGaugeInit(gauge, self);
    }
    self->hpGauge = gauge;
}
