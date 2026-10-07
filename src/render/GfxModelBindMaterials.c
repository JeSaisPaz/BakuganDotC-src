// bdc 0x089e1e88 GfxModelBindMaterials
#include "bdc.h"

/* Binds the materials of a GMO model object to its GMO data (from `GfxModelBindGmo`): points each
   material's `info` (`GmoModelGetMaterial`) at its 16-byte `GfxMaterialState` in the per-model
   table `materialStates` (NULL when the model has none), then for every material resets its layer
   flags (`GmoMaterialSetLayerFlags14`/`GmoMaterialSetLayerFlags16`, mask `0x14a`), copies
   `g_gfxMaterialStateDefault` into its state and parses the render codes after `"__"` in the
   material name (`GfxModelGetMaterialName`) into it: `A<d>` alpha-test reference from
   `g_gfxAlphaRefTable`, `CN`/`CF` culling (`shadeFlags` bits 0..1), `FD` (`renderFlags` bits
   2..3 = 2: fog off, lighting off), `LE`/`LD` lighting on/off (bit 3), `BR`/`BA`/`BS` blend mode
   (bits 6..7), `Z<d>` sort key (`d * 8`), `P<d>` (`texSlot = d + 1`), `D<d>`/`DE<d>`/`DM<d>`/
   `DS<d>` depth bias (`d * 8 + 1`, negated for `DS`; `DE`/`DM` default 1, `DS` default -30, `DM`
   also sets depth write off), `R<m><d>`/`T<m><d>` billboard slots (`shadeFlags` bits 5..7 / 2..4)
   plus pass blend mode `m` (R 0, M 1, A 2, S 3, other 1) in `renderFlags` bits 0..1. Finally
   computes the node sort keys (`GfxModelInitNodeSortKeys`). */

/* digit value of a code character, -1 when it is not '0'..'9' */
#define MATERIAL_CODE_DIGIT(c) \
  ((c) - '0' < 0 || (c) - '0' > 9 ? -1 : (c) - '0')

void GfxModelBindMaterials(GfxModel *self)
{
  GmoModel *data = self->data;
  GfxMaterialState *states = (GfxMaterialState *)self->materialStates;
  s32 count = self->materialCount;
  s32 i;

  if (states != NULL) {
    for (i = 0; i < count; i++) {
      GmoMaterial *mat = (GmoMaterial *)GmoModelGetMaterial(data, i);
      mat->info = (u8 *)states;
      states++;
    }
  } else {
    for (i = 0; i < count; i++) {
      GmoMaterial *mat = (GmoMaterial *)GmoModelGetMaterial(data, i);
      mat->info = NULL;
    }
  }

  for (i = 0; i < self->materialCount; i++) {
    GmoMaterial *mat = (GmoMaterial *)GmoModelGetMaterial(self->data, i);
    GfxMaterialState *state;
    const char *name;
    char *p;

    GmoMaterialSetLayerFlags14(mat, 0x14a, 0xffff);
    GmoMaterialSetLayerFlags16(mat, 0x14a, 0);
    state = (GfxMaterialState *)mat->info;
    state->animCallback = NULL;
    state->animArg = NULL;
    name = GfxModelGetMaterialName(self, i);
    ((GfxMaterialState *)self->materialStates)[i] = g_gfxMaterialStateDefault;
    p = strstr(name, g_gfxMaterialCodePrefix);
    if (p != NULL) {
      s32 more = 1;

      p += 2;
      do {
        char code[5];
        s32 d;

        code[0] = p[0];
        code[1] = p[1];
        code[2] = p[2];
        code[3] = '*';
        code[4] = '\0';
        if (strlen(code) < 3) {
          more = 0;
        }
        if (code[0] == 'A' && (d = MATERIAL_CODE_DIGIT(code[1])) >= 0) {
          state->alphaRef = g_gfxAlphaRefTable[d];
          p += 2;
        } else if (code[0] == 'C') {
          if (code[1] == 'N') {
            state->shadeFlags &= ~3;
          }
          if (code[1] == 'F') {
            state->shadeFlags = (state->shadeFlags & ~3) | 2;
          }
          p += 2;
        } else if (code[0] == 'F') {
          if (code[1] == 'D') {
            state->renderFlags = (state->renderFlags & 0xf3) | 8;
          }
          p += 2;
        } else if (code[0] == 'L') {
          if (code[1] == 'E') {
            state->renderFlags &= ~8;
          }
          if (code[1] == 'D') {
            state->renderFlags |= 8;
          }
          p += 2;
        } else if (code[0] == 'B') {
          if (code[1] == 'R') {
            state->renderFlags &= 0x3f;
          }
          if (code[1] == 'A') {
            state->renderFlags = (state->renderFlags & 0x3f) | 0x80;
          }
          if (code[1] == 'S') {
            state->renderFlags |= 0xc0;
          }
          p += 2;
        } else if (code[0] == 'Z') {
          d = MATERIAL_CODE_DIGIT(code[1]);
          if (d >= 0) {
            state->sortKey = d << 3;
          }
          p += 2;
        } else if (code[0] == 'P') {
          d = MATERIAL_CODE_DIGIT(code[1]);
          if (d >= 0) {
            state->texSlot = d + 1;
          }
          p += 2;
        } else if (code[0] == 'D') {
          s32 negate = 0;

          if (code[1] == 'E') {
            state->depthBias = 1;
          } else if (code[1] == 'M') {
            state->renderFlags |= 0x10;
            state->depthBias = 1;
          } else if (code[1] == 'S') {
            negate = 1;
            state->depthBias = -0x1e;
          } else {
            d = MATERIAL_CODE_DIGIT(code[1]);
            if (d >= 0) {
              /* D<d>: one digit, no suffix */
              state->depthBias = (d << 3) + 1;
              p += 2;
              continue;
            }
          }
          d = MATERIAL_CODE_DIGIT(code[2]);
          if (d >= 0) {
            state->depthBias = (d << 3) + 1;
            if (negate) {
              state->depthBias = -state->depthBias;
            }
            p += 1;
          }
          p += 2;
        } else if (code[0] == 'R' || code[0] == 'T') {
          d = MATERIAL_CODE_DIGIT(code[2]);
          if (d >= 0) {
            u8 mode;
            u8 flags;

            if (code[0] == 'R') {
              state->shadeFlags = (state->shadeFlags & 0x1f) | (((u8)d & 7) << 5);
            } else {
              state->shadeFlags = (state->shadeFlags & 0xe3) | (((u8)d & 7) << 2);
            }
            mode = 1;
            flags = state->renderFlags & ~3;
            switch (code[1]) {
            case 'R':
              mode = 0;
              break;
            case 'M':
              mode = 1;
              break;
            case 'A':
              mode = 2;
              break;
            case 'S':
              mode = 3;
              break;
            }
            state->renderFlags = flags | (mode & 3);
          }
          p += 3;
        } else {
          p += 1;
        }
      } while (more);
    }
  }
  GfxModelInitNodeSortKeys(self);
}
