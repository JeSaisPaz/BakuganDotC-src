// bdc 0x089dcf30 GmoDlWriteTexMapMode
#include "bdc.h"

/* Writes the texture mapping mode `0xc0` (UV, texture matrix or environment map, from the global
   override `0x08ac5c4c` or the material's `+0xa` when flag 0x1000000 is set; type 0x86 selects
   env-map) and `0xc1` (light sources for env-mapping); remembers in `ctx+0x44` whether the texture
   matrix must be refreshed. */

void GmoDlWriteTexMapMode(GmoDlContext *self)

{
  u32 mode;
  uint word;
  u32 dirty;
  uint kind;
  u32 *p;
  
  kind = g_gmoTexMapOverride & 0xf00;
  mode = g_gmoTexMapOverride;
  if (g_gmoTexMapOverride == 0) {
    kind = 0;
    if ((self->material->flags & 0x1000000) != 0) {
      mode = self->material->texMapMode;
      kind = mode & 0xf00;
    }
  }
  dirty = 0x2000000;
  if (kind == 0) {
    dirty = 0;
  }
  self->texMapDirty = dirty;
  kind = 0;
  if (mode == 0) {
    word = 0xc0000100;
    if (self->material->type == 0x86) {
      kind = 2;
    }
  }
  else {
    kind = 1;
    word = (int)g_gmoBlendFactorMap[(mode & 0xf) + 10] << 8 | 0xc0000000;
  }
  p = self->cur;
  self->cur = p + 1;
  *p = word | kind;
  p = self->cur;
  self->cur = p + 1;
  *p = 0xc1000302;
  return;
}

