// bdc 0x089f4a60 GfxSpriteSetCell
#include "bdc.h"

/* Selects which cell of a sprite-sheet texture a sprite shows: rewrites the UVs of the sprite's 4
   vertices (`+0x120`) to the rectangle `[column*w, (column+1)*w] x [row*h, (row+1)*h]` in texels,
   where `w`/`h` are the sprite's base width/height (`+0x70`/`+0x74`), normalised by the texture's
   1/width and 1/height (`texture+0xa4`/`+0xa8`, texture pointer at sprite `+0xd4`). No-op for quad
   modes 0 and 1 (`+0xe4`), which use the shared read-only template vertices. */

void GfxSpriteSetCell(GfxSprite *sprite, float column, float row)

{
  GfxTexture *tex = (GfxTexture *)sprite->texture;
  float invW = tex->invWidth;
  float invH = tex->invHeight;
  GfxSpriteVertex (*q)[4] = (GfxSpriteVertex (*)[4])sprite->vertices;
  float f;

  if (sprite->quadMode >= 0 && sprite->quadMode < 2) {
    return;
  }
  f = sprite->width * column * invW;
  (*q)[2].u = f;
  (*q)[0].u = f;
  f = sprite->width * (column + 1.0f) * invW;
  (*q)[3].u = f;
  (*q)[1].u = f;
  f = sprite->height * row * invH;
  (*q)[1].v = f;
  (*q)[0].v = f;
  f = sprite->height * (row + 1.0f) * invH;
  (*q)[3].v = f;
  (*q)[2].v = f;
}
