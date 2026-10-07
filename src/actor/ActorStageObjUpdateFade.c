// bdc 0x088abf68 ActorStageObjUpdateFade
#include "bdc.h"

/* Per-frame visibility/fade step of a stage object (scenery prop, `ActorStageObjBaseCtor`).
   Returns 0 at once (nothing touched) when `self` is NULL or the field task exists
   (`GameFieldTaskExists`). Otherwise marks the model visible and picks a fade state:
   - profile flag clear (`SaveGetProfileFlag0`): projects the position with the active camera
     (`GfxCameraProjectPointFacing`), measures the distance to the camera eye and, on stages 10 and
     12 (script variable 1, `g_scriptGlobalVars`), overrides `farDist` with 2700. On screen
     (`GfxScreenPointIsVisible` with margin `radius`): beyond `farDist` fade out (3), else an
     occlusion candidate that fades in (1) beyond 1000 or keeps its state (0) within it. Off screen:
     beyond `farDist` snaps to alpha 0 (2), within 1000 an occlusion candidate (state 0);
   - profile flag set (network): any unit of `g_btlBakuganList` with `isPlayer` within 2000 of the
     object makes it an occlusion candidate fading in (1) and clears collider flag 2, otherwise the
     collider flag 2 is set and it fades out (3).
   An occlusion candidate with a collider runs `ActorStageObjOccludesPlayer` on the player unit
   (`BtlGetPlayerBakugan`), which may lower `*alpha` and change the state. State 1/3 moves `*alpha`
   by +/-0.1 (clamped to 0..1), state 2 sets 0, state 0 sets 1 unless occluded. While the alpha is
   strictly between 0 and 1 the object counts as fading; if `allowTranslucent` and virtual 13 (vtable
   `+0x68`) returns 0, a fading object whose alpha changed this frame switches its materials to
   translucent (`ActorStageObjMaterialSetTranslucent`, `*translucent = 1`) and a non-fading one
   that is translucent back to opaque (`ActorStageObjMaterialSetOpaque`, `*translucent = 0`).
   Returns 0 and clears `visible` when `*alpha <= 0`, else 1. */

/* |a - b| over xyz (the binary's vsub.q / vdot.t / vsqrt.s sequence). */
static inline float StageObjFadeDistance(const float *a, const float *b)
{
  float dx = a[0] - b[0];
  float dy = a[1] - b[1];
  float dz = a[2] - b[2];

  return __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
}

int ActorStageObjUpdateFade(float radius, float farDist, void *unused, ActorStageObjBase *self, float *alpha, char *translucent, char allowTranslucent)
{
  float screen[4] __attribute__((aligned(16)));
  float dist;
  float alphaPrev;
  const VtblEntry *vt;
  BtlBakugan *unit;
  void *list;
  float a;
  int candidate;
  int near;
  int fading;
  int stage;
  u8 state;

  (void)unused;
  if (self == NULL || GameFieldTaskExists() != 0) {
    return 0;
  }
  self->base.visible = 1;
  state = 0;
  candidate = 0;
  if (SaveGetProfileFlag0() != 0) {
    near = 0;
    unit = NULL;
    list = BtlGetBakuganList();
    if (list != NULL) {
      unit = *(BtlBakugan **)list;
    }
    for (; unit != NULL; unit = (BtlBakugan *)unit->base.base.next) {
      if (unit->isPlayer == 0) {
        continue;
      }
      dist = StageObjFadeDistance(unit->base.pos, self->base.pos);
      if (dist <= 2000.0f) {
        candidate = 1;
        near = 1;
      }
    }
    if (self->collider != NULL) {
      if (near) {
        ((CollisionCollider *)self->collider)->flags &= ~2u;
      } else {
        ((CollisionCollider *)self->collider)->flags |= 2;
      }
    }
    state = near ? 1 : 3;
  } else {
    GfxCameraProjectPointFacing(g_gfxActiveCamera, screen, self->base.pos);
    dist = StageObjFadeDistance(g_gfxActiveCamera->eye, self->base.pos);
    stage = g_scriptGlobalVars[1];
    if (stage == 10 || stage == 12) {
      farDist = 2700.0f;
    }
    if (GfxScreenPointIsVisible(radius, screen) != 0) {
      if (!(dist <= farDist)) {
        state = 3;
      } else {
        candidate = 1;
        state = !(dist <= 1000.0f);
      }
    } else if (!(dist <= farDist)) {
      state = 2;
    } else if (dist <= 1000.0f) {
      candidate = 1;
    }
  }
  alphaPrev = *alpha;

  fading = 0;
  if (self->collider != NULL && candidate) {
    fading = ActorStageObjOccludesPlayer(self, self, BtlGetPlayerBakugan(), alpha, (char *)&state) != 0;
  }
  switch (state) {
  case 1:
    fading = 1;
    a = *alpha + 0.1f;
    *alpha = a;
    if (!(a <= 1.0f)) {
      *alpha = 1.0f;
    }
    break;
  case 2:
    *alpha = 0.0f;
    break;
  case 3:
    fading = 1;
    a = *alpha - 0.1f;
    *alpha = a;
    if (a < 0.0f) {
      *alpha = 0.0f;
    }
    break;
  default:
    if (!fading) {
      *alpha = 1.0f;
    }
    break;
  }
  if (!(*alpha < 1.0f) || *alpha <= 0.0f) {
    fading = 0;
  }

  if (allowTranslucent != 0) {
    vt = &((const VtblEntry *)self->base.base.vtable)[13];
    if (fading) {
      if (((int (*)(void *))vt->fn)((u8 *)self + vt->delta) == 0 && alphaPrev != *alpha) {
        GfxModelForEachMaterial(&self->base, ActorStageObjMaterialSetTranslucent, NULL);
        *translucent = 1;
      }
    } else {
      if (((int (*)(void *))vt->fn)((u8 *)self + vt->delta) == 0 && *translucent != 0) {
        GfxModelForEachMaterial(&self->base, ActorStageObjMaterialSetOpaque, NULL);
        *translucent = 0;
      }
    }
  }
  if (*alpha <= 0.0f) {
    self->base.visible = 0;
    return 0;
  }
  return 1;
}
