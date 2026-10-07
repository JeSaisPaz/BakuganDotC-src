// bdc 0x0895c1e0 UiEquipLayoutPlayerMarkers
#include "bdc.h"

/* Positions each player's marker sprites (`markerSpriteA[p]`, `markerSpriteB[p]`) of the
   Bakugan/gear loadout screen before a battle (task 302, `UiEquipCtor`) relative to the player's
   base marker sprite `markerBase[p]`: position = base position + `markerOfsA/B[p]` times the base's
   scale, with the X offset negated for odd players (mirrored layout). */

void UiEquipLayoutPlayerMarkers(UiEquip *self)
{
  int p;
  GfxSprite **sprites;
  GfxSprite *base;
  GfxSprite *marker;

  for (p = 0; p < self->playerCount; p++) {
    sprites = (GfxSprite **)self->base.data;
    base = sprites[self->markerBase[p]];
    marker = sprites[self->markerSpriteA[p]];
    if ((p & 1) == 0) {
      marker->posX = base->posX + self->markerOfsA[p][0] * base->scaleX;
    } else {
      marker->posX = base->posX + -(self->markerOfsA[p][0] * base->scaleX);
    }
    sprites = (GfxSprite **)self->base.data;
    base = sprites[self->markerBase[p]];
    marker = sprites[self->markerSpriteA[p]];
    marker->posY = base->posY + self->markerOfsA[p][1] * base->scaleY;
  }

  for (p = 0; p < self->playerCount; p++) {
    sprites = (GfxSprite **)self->base.data;
    base = sprites[self->markerBase[p]];
    marker = sprites[self->markerSpriteB[p]];
    if ((p & 1) == 0) {
      marker->posX = base->posX + self->markerOfsB[p][0] * base->scaleX;
    } else {
      marker->posX = base->posX + -(self->markerOfsB[p][0] * base->scaleX);
    }
    sprites = (GfxSprite **)self->base.data;
    base = sprites[self->markerBase[p]];
    marker = sprites[self->markerSpriteB[p]];
    marker->posY = base->posY + self->markerOfsB[p][1] * base->scaleY;
  }
}
