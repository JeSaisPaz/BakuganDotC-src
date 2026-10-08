// bdc 0x089a5754 UiPulseInit
#include "bdc.h"

/* Prepares a cursor pulse: copies `src` into the ghost sprite `ghost` (`GfxSpriteCopy`), shows it
   one unit in front at full alpha and fills the 0x28-byte record `pulse` (source `+0x24`, alpha
   `+0x20`, phase `+0x1c`, target scale `+0x14 = 1 + 24/width`, at least 1.1). `UiPulseStep` then
   grows and fades the ghost. */

void UiPulseInit(GfxSprite *src, GfxSprite *ghost, UiPulse *self)
{
  float width;
  float scale;

  GfxSpriteCopy(src, ghost);
  scale = 1.0f;
  ghost->flags = ghost->flags | 1;
  ghost->alpha = 1.0f;
  ghost->posZ = src->posZ + 1.0f;
  self->phase = 0.0f;
  self->source = PspAddr(src);
  self->level = ghost->alpha;
  self->waiting = '\0';
  width = GfxSpriteGetWidth(src);
  if (width != 0.0f) {
    scale = 24.0f / width + scale;
  }
  self->targetScale = scale;
  if (scale < 1.1f) {
    self->targetScale = 1.1f;
  }
}
