// bdc 0x0890cd6c UiLoadingShowTip
#include "bdc.h"

/* Shows the loading tip of the now-loading screen (task 10100, `UiLoadingCtor`). Does nothing
   until the tip-picture request `tipData` of `g_uiLoadingShared` exists and is done
   (`IoDataIsDone`). The first time (`noTip` clear) it makes shared sprite 0 visible at alpha 0,
   looks up tip `theme` in `g_loadingTipKinds` (`{kind, number}`), formats its texture name
   (`"tips_t_%03d"` for kind 1, `"tips_%03d"` for kind 2; other kinds leave the name buffer
   unset), allocates a 0x140-byte texture from the low end of the heap and builds it from the
   loaded TIM2 data (`GfxTextureCtor`), stores it in `tipTexture` (NULL when the allocation
   fails), puts it on the sprite with the UV rectangle (0, 0, 352, 176) and sets `noTip`.
   Later calls fade it in: `tipScrollTimer` grows by 0.1 per call, clamped to [0, pi/2]
   (a negative timer is reset to 0), and `tipScroll` and the sprite's alpha become
   sin(timer) (VFPU `vsin` of timer * S703 = 2/pi, i.e. sinf(timer)). */

void UiLoadingShowTip(UiLoading *self)
{
  char name[36];
  GfxSprite *sprite;
  CoreObject *tex;
  s32 *tip;
  bool fromLow;
  float t;
  float angle;

  if (g_uiLoadingShared->tipData == NULL || !IoDataIsDone(g_uiLoadingShared->tipData)) {
    return;
  }
  angle = 0.0f;
  sprite = g_uiLoadingShared->sprites[0];
  if (!self->noTip) {
    sprite->alpha = angle;
    sprite->flags |= 1;
    tip = g_loadingTipKinds[self->theme];
    if (tip[0] < 2) {
      if (tip[0] > 0) {
        sprintf(name, "tips_t_%03d", tip[1]);
      }
    } else if (tip[0] < 3) {
      sprintf(name, "tips_%03d", tip[1]);
    }
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    tex = (CoreObject *)MemAlloc(sizeof(GfxTexture), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (tex != NULL) {
      GfxTextureCtor(tex, name, IoDataGetBuffer(g_uiLoadingShared->tipData), 0);
    }
    g_uiLoadingShared->tipTexture = tex;
    sprite->texture = g_uiLoadingShared->tipTexture;
    UiSpriteSetUvRectAndSize(angle, angle, 352.0f, 176.0f, self, sprite);
    self->noTip = 1;
    return;
  }
  t = self->tipScrollTimer + 0.1f;
  self->tipScrollTimer = t;
  if (t < 0.0f) {
    self->tipScrollTimer = angle;
  } else {
    angle = 1.57079637f;
    if (t <= angle) {
      angle = t;
    }
    self->tipScrollTimer = angle;
  }
  t = __builtin_sinf(angle);
  self->tipScroll = t;
  sprite->alpha = t;
}
