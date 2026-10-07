// bdc 0x08910c20 UiPauseSetCursorEntry
#include "bdc.h"

/* Moves the pause-menu highlight from entry `prev` to entry `next` (either may be -1): the old
   entry's frame/label sprites (sprite table `data[5 + i]` / `data[13 + i]`) shrink back
   (x 0.9090909), the frame gets texture `p_waku_02` and a cleared add colour; the new ones grow
   10 % and the frame gets `p_waku_01`, and the cursor sprite `data[20]` is made visible and moved to
   the new frame's position at z -200. Finally sprite `data[36]` gets alpha -1. */

void UiPauseSetCursorEntry(UiPause *self, int next, int prev)
{
  GfxSprite *sprite;
  GfxSprite *cursor;
  float w;

  if (prev != -1) {
    sprite = ((GfxSprite **)self->base.data)[prev + 5];
    w = GfxSpriteGetWidth(sprite) * 0.9090909f;
    UiSpriteSetSize(w, GfxSpriteGetHeight(sprite) * 0.9090909f, sprite);
    sprite->texture = GfxFindTexture("p_waku_02");
    sprite->addColor[0] = 0.0f;
    sprite->addColor[1] = 0.0f;
    sprite->addColor[2] = 0.0f;
    sprite->addColor[3] = 1.0f;
    sprite = ((GfxSprite **)self->base.data)[prev + 13];
    w = GfxSpriteGetWidth(sprite) * 0.9090909f;
    UiSpriteSetSize(w, GfxSpriteGetHeight(sprite) * 0.9090909f, sprite);
  }
  if (next != -1) {
    sprite = ((GfxSprite **)self->base.data)[next + 5];
    w = GfxSpriteGetWidth(sprite) * 1.1f;
    UiSpriteSetSize(w, GfxSpriteGetHeight(sprite) * 1.1f, sprite);
    sprite->texture = GfxFindTexture("p_waku_01");
    sprite = ((GfxSprite **)self->base.data)[next + 13];
    w = GfxSpriteGetWidth(sprite) * 1.1f;
    UiSpriteSetSize(w, GfxSpriteGetHeight(sprite) * 1.1f, sprite);
    cursor = ((GfxSprite **)self->base.data)[20];
    cursor->flags |= 1;
    sprite = ((GfxSprite **)self->base.data)[next + 5];
    cursor->posX = sprite->posX;
    cursor->posY = sprite->posY;
    cursor->posZ = sprite->posZ;
    cursor->posW = sprite->posW;
    cursor->posZ = -200.0f;
  }
  ((GfxSprite **)self->base.data)[36]->alpha = -1.0f;
}
