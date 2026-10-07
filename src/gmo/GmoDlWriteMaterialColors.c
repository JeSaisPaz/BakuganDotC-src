// bdc 0x089dcc70 GmoDlWriteMaterialColors
#include "bdc.h"

/* Writes the colours of the current material (`ctx->material`) to the list: diffuse `0x56`
   (`diffuse`), specular `0x57` (`specular`), ambient `0x55` with ambient alpha `0x58` (both from
   `emission`) and specular power `0x5b` (the float bits of `word3c` >> 8). When a node colour is
   active (`ctx->color` != -1) the three colours are first modulated with it
   (`GmoColorModulate`). */

void GmoDlWriteMaterialColors(GmoDlContext *self)
{
  GmoAttr *mat;
  u32 *p;
  u32 colors[3];

  mat = self->material;
  if (self->color == 0xffffffff) {
    self->cur[0] = (mat->diffuse & 0xffffff) | 0x56000000;
    self->cur[1] = (mat->specular & 0xffffff) | 0x57000000;
    self->cur[2] = (mat->emission & 0xffffff) | 0x55000000;
    self->cur[3] = (mat->emission >> 24) | 0x58000000;
    self->cur[4] = (mat->word3c >> 8) | 0x5b000000;
    self->cur += 5;
  } else {
    colors[0] = mat->diffuse;
    colors[1] = mat->specular;
    colors[2] = mat->emission;
    GmoColorModulate(&colors[0], &colors[0], &self->color);
    GmoColorModulate(&colors[1], &colors[1], &self->color);
    GmoColorModulate(&colors[2], &colors[2], &self->color);
    p = self->cur;
    self->cur = p + 1;
    *p = (colors[0] & 0xffffff) | 0x56000000;
    p = self->cur;
    self->cur = p + 1;
    *p = (colors[1] & 0xffffff) | 0x57000000;
    p = self->cur;
    self->cur = p + 1;
    *p = (colors[2] & 0xffffff) | 0x55000000;
    p = self->cur;
    self->cur = p + 1;
    *p = (colors[2] >> 24) | 0x58000000;
    p = self->cur;
    self->cur = p + 1;
    *p = (self->material->word3c >> 8) | 0x5b000000;
  }
}
