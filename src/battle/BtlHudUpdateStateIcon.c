// bdc 0x0882e314 BtlHudUpdateStateIcon
#include "bdc.h"

/* Shows at most one state icon for `unit` on the battle HUD: hides sprites 0x6c, 0xfe, 0xff and
   0x100 (clears flag bit 0), then shows sprite 0xff when status slot 0 is active, else 0x100 for
   slot 2, else 0xfe for slot 17, else the species icon 0x6c when the unit kind is 1..20. Does
   nothing when `unit` is NULL. */
void BtlHudUpdateStateIcon(BtlHud *self, BtlBakugan *unit)
{
  u32 kind;

  if (unit == NULL) {
    return;
  }
  self->sprites[0x6c]->flags &= ~1u;
  self->sprites[0xfe]->flags &= ~1u;
  self->sprites[0xff]->flags &= ~1u;
  self->sprites[0x100]->flags &= ~1u;
  if (unit->combat.status[0].active != 0) {
    self->sprites[0xff]->flags |= 1;
    return;
  }
  if (unit->combat.status[2].active != 0) {
    self->sprites[0x100]->flags |= 1;
    return;
  }
  if (unit->combat.status[17].active != 0) {
    self->sprites[0xfe]->flags |= 1;
    return;
  }
  kind = unit->base.base.unk08;
  if (kind != 0 && kind < 0x15) {
    self->sprites[0x6c]->flags |= 1;
  }
}
