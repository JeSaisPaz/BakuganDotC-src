// bdc 0x08871864 BtlBakuganApplyHover
#include "bdc.h"

/* Hover support for floating units: when the stat hover height is nonzero and the unit sits
   below ground + hover height, sets the vertical speed to `gravity + 0.3 × deficit`. Returns 1
   for hovering units (whether or not the speed was set), 0 when the hover height is 0. */
int BtlBakuganApplyHover(BtlBakugan *self)
{
  float hoverHeight = self->combat.stats->hoverHeight;
  float deficit;

  if (hoverHeight == 0.0f) {
    return 0;
  }
  deficit = (self->groundY + hoverHeight) - self->base.pos[1];
  if (!(deficit <= 0.0f)) {
    self->base.velocity[1] = self->gravity + deficit * 0.300000012f;
  }
  return 1;
}
