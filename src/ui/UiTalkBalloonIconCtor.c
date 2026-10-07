// bdc 0x088c7650 UiTalkBalloonIconCtor
#include "bdc.h"

/* Constructor of a talk balloon sprite (`UiTalkBalloonSprite`, vtable `g_uiTalkBalloonSpriteVtbl`)
   for the non-glyph kinds (`kind` 1 frame, 2 icon, 3 cursor, 4 fading icon): base sprite
   constructor `GfxSpriteCtor`, owner, kind, state 0, texture `texture`, linear filtering, white
   tint with alpha 0, position zeroed (VFPU bank C720 = 0), appends it to the owner's printer
   layer (`GfxSpriteLayerAdd`) and sets kind-specific size/UVs: kind 1 takes the owner's text-area
   size `textSize` (phase 1.0), kind 2 a 24x32 cell, kind 3 a 32x32 cell placed at the selected
   choice line, kind 4 32x32 full texture with size 28 and scaleY at the choice line. Other kinds
   only get the common setup. Returns `sprite`. */

GfxSprite *UiTalkBalloonIconCtor(GfxSprite *sprite, void *texture, s32 kind, UiTalkBalloon *owner)
{
  UiTalkBalloonSprite *self = (UiTalkBalloonSprite *)sprite;
  float rect[4];
  s32 k;

  GfxSpriteCtor(sprite);
  sprite->vtable = g_uiTalkBalloonSpriteVtbl;
  self->owner = owner;
  self->kind = kind;
  self->state = 0;
  sprite->texture = texture;
  sprite->flags |= 0x20;
  sprite->tint[0] = g_colorWhite.x;
  sprite->tint[1] = g_colorWhite.y;
  sprite->tint[2] = g_colorWhite.z;
  sprite->alpha = g_colorWhite.w;
  self->frame = 0;
  self->holdTimer = 0;
  sprite->alpha = 0.0f;
  self->keep = 0;
  /* VFPU bank C720 = (0, 0, 0, 0) */
  sprite->posX = 0.0f;
  sprite->posY = 0.0f;
  sprite->posZ = 0.0f;
  sprite->posW = 0.0f;
  GfxSpriteLayerAdd(&owner->printer->layer, (CoreObject *)sprite);

  k = self->kind;
  if (k < 3) {
    if (k <= 0) {
      return sprite;
    }
    if (k < 2) {
      self->phase = 1.0f;
      GfxSpriteSetQuadMode(sprite, 3);
      sprite->width = self->owner->textSize[0];
      sprite->height = self->owner->textSize[1];
      sprite->depth = self->owner->textSize[2];
      sprite->maybe_sizeW = self->owner->textSize[3];
      UiSpriteSetSize(sprite->width, sprite->height, sprite);
      GfxSpriteSetUvFull(sprite);
      sprite->flags &= ~1u;
      sprite->angle = 0.0f;
    } else {
      GfxSpriteSetQuadMode(sprite, 3);
      GfxSpriteSetSize(sprite, 24.0f, 32.0f);
      rect[0] = 0.0f;
      rect[1] = 0.0f;
      rect[2] = 24.0f;
      rect[3] = 32.0f;
      GfxSpriteSetUvRectXYWH(sprite, rect);
      sprite->flags &= ~1u;
    }
  } else if (k < 4) {
    GfxSpriteSetQuadMode(sprite, 3);
    GfxSpriteSetSize(sprite, 32.0f, 32.0f);
    rect[0] = 0.0f;
    rect[1] = 0.0f;
    rect[2] = 32.0f;
    rect[3] = 32.0f;
    GfxSpriteSetUvRectXYWH(sprite, rect);
    sprite->posX = owner->origin[0] + 6.0f;
    sprite->posY = owner->choiceTop + owner->lineHeight * (float)owner->choiceIndex;
  } else if (k < 5) {
    GfxSpriteSetQuadMode(sprite, 3);
    GfxSpriteSetSize(sprite, 32.0f, 32.0f);
    GfxSpriteSetUvFull(sprite);
    self->phase = 0.0f;
    self->size = 28.0f;
    sprite->scaleY = owner->choiceTop + owner->lineHeight * (float)owner->choiceIndex;
  }
  return sprite;
}
