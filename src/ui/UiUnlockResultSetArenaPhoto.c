// bdc 0x0893bd9c UiUnlockResultSetArenaPhoto
#include "bdc.h"

/* Points `sprite`'s texture at the arena opponent photo `"arena_com_pho_%03d"` with number `index`
   (`GfxFindTexture`). */

void UiUnlockResultSetArenaPhoto(UiUnlockResult *self, GfxSprite *sprite, u8 index)
{
  char name[64];

  sprintf(name, "arena_com_pho_%03d", index);
  sprite->texture = GfxFindTexture(name);
}
