// bdc 0x08892d68 BtlAiUnitHasCommandC00
#include "bdc.h"

/* Threat test of `BtlAi` used by `BtlAiRunReactionRules` / `BtlAiGuardLayerRun`:
   returns 1 when `unit` is non-NULL, the AI's current target is not occluded (`targetOccluded`
   clear), the unit's virtual slot 10 returns non-zero, and the unit's frame command flags
   (`commands`) have any of bits 0xc00 set; otherwise 0. */
s32 BtlAiUnitHasCommandC00(BtlAi *self, void *unit)
{
  BtlBakugan *bakugan = (BtlBakugan *)unit;
  const VtblEntry *entry;

  if (bakugan == NULL || self->targetOccluded != 0) {
    return 0;
  }
  entry = &((const VtblEntry *)bakugan->base.base.vtable)[10];
  if (((s32 (*)(void *))entry->fn)((u8 *)bakugan + entry->delta) == 0) {
    return 0;
  }
  if ((bakugan->commands & 0xc00) != 0) {
    return 1;
  }
  return 0;
}
