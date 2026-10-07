// bdc 0x089df8a0 GfxModelDlWriteState
#include "bdc.h"

/* Writes a model's per-draw GE state into the display list `*list` (advancing it) when the model is
   visible (`visible`), remembering it as the current model (`g_gfxCurrentModel`): optional fog
   (`fogEnabled`: `fogColor` as `0xcf`, the top 24 bits of the floats `fogNear`/`fogFar` as
   `0xcd`/`0xce`, the colour word mirrored in `g_gfxFogColorCmd`, else that is 0), the ambient
   colour/alpha packed from the float RGBA `ambient` (`0x5c`/`0x5d`, cached in `g_gfxModelAmbient` /
   `g_gfxMaterialAlpha`), the lighting-enable byte `lighting` (`0x17`, cached in
   `g_gfxLightingCmd`) and the colour packed from `color` (`0x90`, cached in `g_gfxModelEmissive`),
   then the model data's display list (`GmoModelBuildDlWrapper`). When fog is on and a scene fog
   record `g_gfxFogParams` is set, re-emits that record's fog commands afterwards. Colours are
   clamped to [0, 1], scaled by 255 and truncated to a byte (R in the low byte). About 28 callers
   among the actor/battle model draw methods. */

void GfxModelDlWriteState(GfxModel *self, u32 **list)

{
  union { float f; u32 u; } bits;
  u32 *dl;
  u32 packed;
  s32 i;

  if (self->visible == 0) {
    return;
  }
  g_gfxCurrentModel = self;
  if (self->fogEnabled != 0) {
    dl = *list;
    dl[0] = (self->fogColor & 0xffffff) | 0xcf000000;
    bits.f = self->fogNear;
    dl[1] = (bits.u >> 8) | 0xcd000000;
    bits.f = self->fogFar;
    dl[2] = (bits.u >> 8) | 0xce000000;
    *list = dl + 3;
    g_gfxFogColorCmd = (self->fogColor & 0xffffff) | 0xcf000000;
  }
  else {
    g_gfxFogColorCmd = 0;
  }
  packed = 0;
  for (i = 0; i < 4; i++) {
    packed |= (u32)VfI2uc(VfF2iz(VfSat0(self->ambient[i]) * 255.0f, 23)) << (i * 8);
  }
  g_gfxModelAmbient = packed;
  g_gfxMaterialAlpha = packed >> 24;
  dl = *list;
  dl[0] = (packed & 0xffffff) | 0x5c000000;
  dl[1] = (g_gfxModelAmbient >> 24) | 0x5d000000;
  *list = dl + 2;
  packed = self->lighting | 0x17000000;
  g_gfxLightingCmd = packed;
  **list = packed;
  *list = *list + 1;
  packed = 0;
  for (i = 0; i < 4; i++) {
    packed |= (u32)VfI2uc(VfF2iz(VfSat0(self->color[i]) * 255.0f, 23)) << (i * 8);
  }
  g_gfxModelEmissive = packed;
  **list = (packed & 0xffffff) | 0x90000000;
  *list = *list + 1;
  GmoModelBuildDlWrapper(self->data, (void **)list, (u32 *)0, 0xffff);
  if (self->fogEnabled != 0 && g_gfxFogParams != (BtlArenaFog *)0) {
    BtlArenaFog *fog;

    dl = *list;
    fog = g_gfxFogParams;
    dl[0] = (fog->color & 0xffffff) | 0xcf000000;
    bits.f = fog->range;
    dl[1] = (bits.u >> 8) | 0xcd000000;
    bits.f = fog->scale;
    dl[2] = (bits.u >> 8) | 0xce000000;
    *list = dl + 3;
  }
}
