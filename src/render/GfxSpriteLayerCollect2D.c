// bdc 0x089f4d68 GfxSpriteLayerCollect2D
#include "bdc.h"

/* Collects the drawable sprites of a 2D layer for sorting: walks the sprite chain from `sprite`
   (`next` at `+4`) and, for each sprite that is visible (`flags & 1`) and shares a layer bit with
   the layer's mask (`GfxSpriteLayerGetLayerMask`), calls its `preDrawCallback` (`+0xec`) and
   stores the pair `{sprite, posZ}` in `out`. Returns the number of pairs. */

typedef struct SpriteSortPair {
  GfxSprite *sprite;
  float posZ;
} SpriteSortPair;

int GfxSpriteLayerCollect2D(GfxSpriteLayer *self, GfxSprite *sprite, void *out)

{
  SpriteSortPair *pair = (SpriteSortPair *)out;
  int n = 0;

  while (sprite != (GfxSprite *)0x0) {
    if ((GfxSpriteLayerGetLayerMask(self) & sprite->layerMask) != 0 && (sprite->flags & 1) != 0) {
      if (sprite->preDrawCallback != (void *)0x0) {
        ((void (*)(GfxSprite *))sprite->preDrawCallback)(sprite);
      }
      pair->sprite = sprite;
      pair->posZ = sprite->posZ;
      n++;
      pair++;
    }
    sprite = sprite->next;
  }
  return n;
}
