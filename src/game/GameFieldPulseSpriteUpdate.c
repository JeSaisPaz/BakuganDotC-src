// bdc 0x088c1d08 GameFieldPulseSpriteUpdate
#include "bdc.h"

/* Per-frame pulse of the marker sprite `+0x6b8` of the field task (id 500, `GameFieldCtor`):
   hidden for 15 frames, then grows by 2 % per frame and fades by 1/15, restarting after 30 frames
   (`GameFieldPulseSpriteReset`). */

void GameFieldPulseSpriteUpdate(CoreTask *task)
{
  GameFieldTask *field = (GameFieldTask *)task;
  u32 timer;
  float scale;

  field->locationPulse->flags = field->locationPulse->flags & ~1u;
  timer = field->pulseTimer;
  if (!((int)timer < 0x10)) {
    scale = (float)(int)(timer - 0xf) * 0.020000001f + 1.0f;
    GfxSpriteSetScaleRotation(field->locationPulse, scale, scale, 0.0f, false);
    field->locationPulse->alpha = field->locationPulse->alpha - 0.06666667f;
    field->locationPulse->flags = field->locationPulse->flags | 1;
    timer = field->pulseTimer;
  }
  field->pulseTimer = timer + 1;
  if (!((int)(timer + 1) < 0x1e)) {
    GameFieldPulseSpriteReset(task);
  }
}
