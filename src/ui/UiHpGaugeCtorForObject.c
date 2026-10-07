// bdc 0x08889f3c UiHpGaugeCtorForObject
#include "bdc.h"

/* Constructs the 0xa0-byte HUD HP gauge for a non-unit object (a stage object): runs
   `CoreNodeCtor`, installs vtable `0x08af2194`, stores `obj` at `+0x24` and binds with
   `UiHpGaugeBind``(gauge, 2)`, i.e. mode 2 reading HP from `obj+0x200`/`+0x204`. Returns `gauge`.
    */

UiHpGauge *UiHpGaugeCtorForObject(UiHpGauge *self, void *obj)

{
  CoreNodeCtor(&self->base,(CoreNode *)0x0);
  (self->base).vtable = g_uiHpGaugeVtbl;
  self->source = obj;
  UiHpGaugeBind(self,2);
  return self;
}

