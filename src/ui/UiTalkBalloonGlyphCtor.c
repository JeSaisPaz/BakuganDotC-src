// bdc 0x088c78cc UiTalkBalloonGlyphCtor
#include "bdc.h"

/* Constructor of a talk balloon sprite (`UiTalkBalloonSprite`, vtable `g_uiTalkBalloonSpriteVtbl`)
   of kind 0 for one character `code`: appends it to the owner's printer layer, picks the font
   texture page `code / 1024` from the printer's font, the 16×16 cell `code % 1024` in a 32-column
   sheet, uses texture slot 1 above code 0x205, places it at `pos` rounded to whole pixels (VFPU
   `vf2in`/`vi2f`, w kept), copies the printer's outline colour into the tint, the position into
   the scale vector, and starts it transparent. Returns `sprite`. */

/* vf2in + vi2f: round to nearest, ties to even (pixel coordinates, well inside the int range). */
static inline float RoundHalfEven(float x)
{
  s32 i = (s32)x;
  float frac = x - (float)i;

  if (frac > 0.5f || (frac == 0.5f && (i & 1) != 0)) {
    i++;
  } else if (frac < -0.5f || (frac == -0.5f && (i & 1) != 0)) {
    i--;
  }
  return (float)i;
}

GfxSprite *UiTalkBalloonGlyphCtor(GfxSprite *sprite, s32 code, float *pos, UiTalkBalloon *owner)
{
  UiTalkBalloonSprite *self = (UiTalkBalloonSprite *)sprite;
  s32 cell;
  float col;
  float row;
  float rect[4];

  GfxSpriteCtor(sprite);
  sprite->vtable = g_uiTalkBalloonSpriteVtbl;
  self->owner = owner;
  self->code = code;
  self->state = 0;
  sprite->flags |= 0x20;
  GfxSpriteLayerAdd(&owner->printer->layer, (CoreObject *)sprite);
  self->frame = 0;
  self->holdTimer = 0;
  self->kind = 0;
  self->keep = 0;
  cell = code % 1024;
  col = (float)(cell % 32 * 16);
  row = (float)(cell / 32 * 16);
  sprite->texture = owner->printer->fontTextures[code / 1024];
  GfxSpriteSetQuadMode(sprite, 3);
  GfxSpriteSetSize(sprite, 16.0f, 16.0f);
  rect[0] = col;
  rect[1] = row;
  rect[2] = 16.0f;
  rect[3] = 16.0f;
  GfxSpriteSetUvRectXYWH(sprite, rect);
  sprite->textureSlot = code > 0x205;
  sprite->posX = pos[0];
  sprite->posY = pos[1];
  sprite->posZ = pos[2];
  sprite->posW = pos[3];
  sprite->posX = RoundHalfEven(sprite->posX);
  sprite->posY = RoundHalfEven(sprite->posY);
  sprite->posZ = RoundHalfEven(sprite->posZ);
  sprite->tint[0] = owner->printer->outlineColor[0];
  sprite->tint[1] = owner->printer->outlineColor[1];
  sprite->tint[2] = owner->printer->outlineColor[2];
  sprite->alpha = owner->printer->outlineColor[3];
  sprite->scaleX = sprite->posX;
  sprite->scaleY = sprite->posY;
  sprite->scaleZ = sprite->posZ;
  sprite->angle = sprite->posW;
  sprite->alpha = 0.0f;
  self->phase = 0.0f;
  return sprite;
}
