// bdc 0x088f2a08 GameEventCamTweenCtor
#include "bdc.h"

/* Constructor of the camera tween action (vtable `0x08af430c`, 0x10 bytes): key record `keys`,
   duration `frames`, frame 0, kind (0 eye, 1 target, 2 fov). */

GameEventCamTween *GameEventCamTweenCtor(GameEventCamTween *self, GameEventCamKeys *keys, u16 frames, u8 kind)

{
  (self->base).vtbl = g_gameEventCamTweenVtbl;
  self->keys = keys;
  self->frames = frames;
  self->frame = 0;
  self->kind = kind;
  return self;
}

