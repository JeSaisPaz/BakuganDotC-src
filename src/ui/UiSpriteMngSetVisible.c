// bdc 0x089ee5f8 UiSpriteMngSetVisible
#include "bdc.h"

/* Shows/hides a sprite by setting/clearing bit 0 of its flags (`sprite+0xd0`). With a negative
   `slot` it instead stores `visible` as the manager-wide flag at `mng+0x18`. `ScriptOpSprite` cmd
   2. */

void UiSpriteMngSetVisible(UiSpriteMng *self, int slot, bool visible)
{
  GfxSprite *sprite;

  if (slot < 0) {
    self->visible = visible;
    return;
  }
  if (self->layer == (void *)0x0) {
    return;
  }
  sprite = self->sprites[slot];
  if (sprite == (GfxSprite *)0x0) {
    return;
  }
  if (visible) {
    sprite->flags = sprite->flags | 1;
    return;
  }
  sprite->flags = sprite->flags & 0xfffffffe;
}
