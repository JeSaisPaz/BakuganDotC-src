// bdc 0x088e9b70 ActorNpcViewConeUpdate
#include "bdc.h"

/* Updates the view cone of an NPC (`ActorNpcViewCone` at `npc+0x418`, `ActorNpcViewConeCtor`).
   On the first call after `ActorNpcViewConeInit` (`flag08` set, cleared here) it scales the first
   three rows of the six effect matrices by `length * 0.1`, forces an effect update
   (`GfxEffectUpdateNow`) and sets the GE stencil words of effects 0/3 (`stencilOp` 0xdd040000) and
   1/4 (`stencilTest` 0xdd050000); afterwards, unless `mode == 2`, it resets each matrix to identity,
   scales its rows by `length * 0.1` (the third row also by `widthScale` when `mode == 1`) and forces
   an update. Then, when the cone has an effect manager: if `flag24` is set and the smaller of the
   distances from the anchor to the camera eye (`g_gfxActiveCamera`) and to the player
   (`ActorFindPlayer`) is within `drawRange` (`!(dist <= drawRange)` hides), and `visible` is set,
   it writes the direction (cos heading, 0, sin heading) into every effect's `dir` and shows effects
   3 and 4 only; otherwise it hides all six mesh objects. The anchor offset by `length` along that
   direction is computed into a stack temp and never read; the C leaves it out. */

void ActorNpcViewConeUpdate(ActorNpcViewCone *cone, s32 mode, float length, float drawRange, float heading, float widthScale)
{
  ActorNpcViewCone *vc = cone;
  float scale[4];
  float dir[4];
  float d[4];
  GfxEffect *effect;
  GfxMeshObj *mesh;
  Actor *player;
  float *m;
  float s;
  float dist;
  float playerDist;
  bool show;
  u32 i;
  u32 j;

  if (vc->flag08 != 0) {
    vc->flag08 = 0;
    s = length * 0.1f;
    for (i = 0; i < 6; i++) {
      effect = vc->effects[i];
      m = effect->matrix;
      scale[2] = s;
      scale[1] = s;
      scale[0] = s;
      scale[3] = 0.0f;
      /* Rows 0..2 (all four lanes) scaled by scale[0..2]. */
      for (j = 0; j < 4; j++) {
        m[0 + j] = m[0 + j] * scale[0];
        m[4 + j] = m[4 + j] * scale[1];
        m[8 + j] = m[8 + j] * scale[2];
      }
      GfxEffectUpdateNow(effect);
    }
    mesh = vc->effects[0]->meshObj;
    if (mesh != NULL) {
      mesh->stencilOp = 0xdd040000;
    }
    mesh = vc->effects[1]->meshObj;
    if (mesh != NULL) {
      mesh->stencilTest = 0xdd050000;
    }
    mesh = vc->effects[3]->meshObj;
    if (mesh != NULL) {
      mesh->stencilOp = 0xdd040000;
    }
    mesh = vc->effects[4]->meshObj;
    if (mesh != NULL) {
      mesh->stencilTest = 0xdd050000;
    }
  }
  else if (mode != 2) {
    s = length * 0.1f;
    for (i = 0; i < 6; i++) {
      effect = vc->effects[i];
      m = effect->matrix;
      for (j = 0; j < 16; j++) {
        m[j] = (j % 5 == 0) ? 1.0f : 0.0f;
      }
      scale[0] = s;
      scale[1] = s;
      scale[2] = s;
      scale[3] = 0.0f;
      if (mode == 1) {
        scale[2] = scale[2] * widthScale;
      }
      for (j = 0; j < 4; j++) {
        m[0 + j] = m[0 + j] * scale[0];
        m[4 + j] = m[4 + j] * scale[1];
        m[8 + j] = m[8 + j] * scale[2];
      }
      GfxEffectUpdateNow(effect);
    }
  }
  if (vc->manager == NULL) {
    return;
  }
  show = true;
  if (vc->flag24 == 0) {
    show = false;
  }
  else {
    for (j = 0; j < 3; j++) {
      d[j] = g_gfxActiveCamera->eye[j] - vc->anchor[j];
    }
    dist = __builtin_sqrtf(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]);
    player = ActorFindPlayer();
    if (player != NULL) {
      for (j = 0; j < 3; j++) {
        d[j] = player->base.pos[j] - vc->anchor[j];
      }
      playerDist = __builtin_sqrtf(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]);
      if (!(dist <= playerDist)) {
        dist = playerDist;
      }
    }
    if (!(dist <= drawRange)) {
      show = false;
    }
  }
  if (show && vc->visible != 0) {
    /* vrot [C,0,S,0] of heading * 2/pi (S703): a quarter-turn cos/sin of the angle. */
    dir[0] = __builtin_cosf(heading);
    dir[1] = 0.0f;
    dir[2] = __builtin_sinf(heading);
    dir[3] = 0.0f;
    for (i = 0; i < 6; i++) {
      for (j = 0; j < 4; j++) {
        vc->effects[i]->dir[j] = dir[j];
      }
      mesh = vc->effects[i]->meshObj;
      if (mesh != NULL) {
        mesh->visible = (s32)i >= 3;
      }
    }
    vc->effects[2]->meshObj->visible = 0;
    vc->effects[5]->meshObj->visible = 0;
  }
  else {
    for (i = 0; i < 6; i++) {
      mesh = vc->effects[i]->meshObj;
      if (mesh != NULL) {
        mesh->visible = 0;
      }
    }
  }
}
