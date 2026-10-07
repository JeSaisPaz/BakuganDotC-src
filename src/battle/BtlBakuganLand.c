// bdc 0x0887167c BtlBakuganLand
#include "bdc.h"

/* Landing reaction: when `enterState4` is set first switches to state 4 (`BtlBakuganSetState`,
   keep 0); plays landing motion 0xf5, or 0xf7 when `motionIdPair` equals
   `g_btlBakuganLandFlipMotionIds` (`BtlBakuganPlayMotion`, blend 0, no loop, not forced);
   spawns the landing effect (`BtlBakuganSpawnLandingEffect` with `splash`), plays the
   floor-dependent landing sound (`BtlBakuganPlayLandingSound`), clears `subWait`, resets
   `orient` to `g_vecUp`, shakes `g_gfxActiveCamera` (`GfxCameraStartShake`, amplitude 8,
   speed 0.8, 15 frames) and sets `subTimer` to 1. Called by `BtlBakuganState04Update`. */

void BtlBakuganLand(BtlBakugan *self, char enterState4, char splash)
{
  int motion;

  if (enterState4 != 0) {
    BtlBakuganSetState(self, 4, 0);
  }
  motion = 0xf5;
  if (self->motionIdPair[0] == g_btlBakuganLandFlipMotionIds[0] &&
      self->motionIdPair[1] == g_btlBakuganLandFlipMotionIds[1]) {
    motion = 0xf7;
  }
  BtlBakuganPlayMotion(0.0f, self, motion, 0, 0);
  BtlBakuganSpawnLandingEffect(self, splash);
  BtlBakuganPlayLandingSound(self);
  self->subWait = 0;
  self->orient[0] = g_vecUp.x;
  self->orient[1] = g_vecUp.y;
  self->orient[2] = g_vecUp.z;
  self->orient[3] = g_vecUp.w;
  GfxCameraStartShake(8.0f, 0.800000012f, g_gfxActiveCamera, 15); /* 0x41000000, 0x3f4ccccd */
  self->subTimer = 1;
}
