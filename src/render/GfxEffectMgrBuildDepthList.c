// bdc 0x08823844 GfxEffectMgrBuildDepthList
#include "bdc.h"

/* Builds the draw list of an effect manager (`GfxEffectMgrCtor`)'s effects: walks the effect list
   starting at `first` (next `base.next`) and, for each effect with a `model`, stores the
   camera-relative vector `camera->eye − effect->pos` in `toCamera` (lane 3 is the camera eye's `w`)
   and appends the pair `{effect, key}` to `out`, where key is the squared distance (or the constant
   `g_gfxEffectFarDepthKey` for effects with flag 0x80 in `flags`). Stops after 256 entries;
   returns the number of entries written. */

u32 GfxEffectMgrBuildDepthList(void *first, GfxEffectDepthEntry *out, GfxCamera *camera)
{
  GfxEffect *effect = (GfxEffect *)first;
  s32 count = 0;
  GfxEffectDepthEntry *entry = out;

  while (effect != NULL) {
    if (effect->model != NULL) {
      float *d = effect->toCamera;
      float key;

      d[0] = camera->eye[0] - effect->pos[0];
      d[1] = camera->eye[1] - effect->pos[1];
      d[2] = camera->eye[2] - effect->pos[2];
      d[3] = camera->eye[3];
      if ((effect->flags & 0x80) != 0) {
        key = g_gfxEffectFarDepthKey;
      } else {
        key = d[0] * d[0] + d[1] * d[1] + d[2] * d[2];
      }
      entry->key = key;
      entry->effect = effect;
      count = count + 1;
      entry = entry + 1;
      if (count >= 0x100) {
        return (u32)count;
      }
    }
    effect = (GfxEffect *)effect->base.next;
  }
  return (u32)count;
}
