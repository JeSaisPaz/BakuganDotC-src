// bdc 0x08858e88 ActorCrystalModeIdle
#include "bdc.h"

/* Crystal mode 0 (vtable slot 26, idle): when auto-fire `+0xa3d` is set, raises the fire command
   (control `+0x1c` bit 0x10) every 120 frames (`+0x5c8`); on the fire command switches to mode 1
   (`ActorCrystalSetMode`). */

void ActorCrystalModeIdle(ActorCrystal *self)

{
  if (self->autoFire != 0) {
    int frame = self->base.attackFrame;

    self->base.attackFrame = frame + 1;
    if (frame >= 0x78) {
      self->base.input->aiActions |= 0x10;
      self->base.attackFrame = 0;
    }
  }
  if ((self->base.commands & 0x10) != 0) {
    ActorCrystalSetMode(self, 1, false);
  }
}
