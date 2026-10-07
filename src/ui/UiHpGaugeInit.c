// bdc 0x08889ee8 UiHpGaugeInit
#include "bdc.h"

/* Constructor of the HUD hit-point gauge (a 0xa0-byte `CoreNodeCtor`-derived node allocated by
   `BtlBakuganCreateHpGauge` and stored at `unit + 0x554`): runs the node base constructor with 0,
   installs the gauge vtable `0x08af2194` at `+0x20`, stores `unit` at `+0x24` and calls
   `UiHpGaugeBind``(gauge, 1)`. Returns `gauge`. */

UiHpGauge *UiHpGaugeInit(UiHpGauge *self, void *unit)

{
  CoreNodeCtor(&self->base,(CoreNode *)0x0);
  (self->base).vtable = g_uiHpGaugeVtbl;
  self->source = unit;
  UiHpGaugeBind(self,1);
  return self;
}

