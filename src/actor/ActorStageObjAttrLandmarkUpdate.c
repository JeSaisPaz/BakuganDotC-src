// bdc 0x088a7da4 ActorStageObjAttrLandmarkUpdate
#include "bdc.h"

/* Update method (vtable `0x08af27b4` slot 7) of the attribute landmark (`ActorStageObjAttrLandmarkCtor`):
   runs `ActorStageObjUpdate` and `ActorStageObjAttrLandmarkUpdateFade`. The first time it is found dead
   (`deathHandled` clear) it marks its companion unit dead, sets its effects on `g_btlUnitEffectMgr` to
   state 2, clears status `auraType + 11` from every unit of `g_btlBakuganList` whose virtual slot 14
   returns 0, flags its layout record, releases the model's motions, sets `deathHandled` and returns.
   Otherwise it advances the motion for `auraType` 1 and 5, copies the world position of bone 8
   (`kindIndex` 7) or bone 1 (`kindIndex` 8) into `effectPos[0]`, advances `scroll` and `phase`, steps the
   `mode` machine (1 fade down, 2 fade up, 3 shrink and fade out then stop all its effects, 4 hold, any
   other: reset scale/position, alpha 0.7, mode 0) and finally runs the aura handler
   `g_actorStageObjAttrLandmarkAuraFns``[auraIndex]` when `auraIndex` is 0..6. */

void ActorStageObjAttrLandmarkUpdate(ActorStageObjAttrLandmark *self)
{
  ScePspFVector4 bone8Pos;
  ScePspFVector4 bone1Pos;
  int i;
  float value;
  GmoModel *data;

  ActorStageObjUpdate(&self->base);
  ActorStageObjAttrLandmarkUpdateFade(self);
  if (self->base.dead != 0 && self->deathHandled == 0) {
    CoreObjectList *list;
    CoreObject *unit;

    if (self->unit != (void *)0x0) {
      ((BtlBakugan *)self->unit)->combat.dead = 1;
    }
    GfxEffectSetStateOwned(g_btlUnitEffectMgr, -1, self, 2);
    list = (CoreObjectList *)BtlGetBakuganList();
    if (list != (CoreObjectList *)0x0 && (unit = list->head) != (CoreObject *)0x0) {
      do {
        const VtblEntry *ent = &((const VtblEntry *)unit->vtable)[14];

        if (((int (*)(void *))ent->fn)((char *)unit + ent->delta) == 0) {
          BtlCombatClearStatus(&((BtlBakugan *)unit)->combat, self->auraType + 0xb);
        }
        unit = unit->next;
      } while (unit != (CoreObject *)0x0);
    }
    if (self->base.record != (void *)0x0) {
      ((ActorStageObjRecord *)self->base.record)->doneFlags[0] = 1;
    }
    if (self->base.base.data->motions != (void *)0x0) {
      data = self->base.base.data;
      GmoMotionArrayRelease(data->motions, data->motionCount);
    }
    self->deathHandled = 1;
    return;
  }

  if (self->auraType == 1 || self->auraType == 5) {
    GfxModelUpdateAndApplyMotion(&self->base.base);
  }
  if (self->kindIndex == 7) {
    GfxModelGetNodeWorldPosByIndex(&self->base.base, &bone8Pos, 8);
    self->effectPos[0][0] = bone8Pos.x;
    self->effectPos[0][1] = bone8Pos.y;
    self->effectPos[0][2] = bone8Pos.z;
    self->effectPos[0][3] = bone8Pos.w;
  }
  else if (self->kindIndex == 8) {
    GfxModelGetNodeWorldPosByIndex(&self->base.base, &bone1Pos, 1);
    self->effectPos[0][0] = bone1Pos.x;
    self->effectPos[0][1] = bone1Pos.y;
    self->effectPos[0][2] = bone1Pos.z;
    self->effectPos[0][3] = bone1Pos.w;
  }

  value = self->scroll;
  value = value + (value * value * -0.002f - 0.02f);
  self->scroll = value;
  if (value <= 0.0f) {
    self->scroll = self->scroll + 16.0f;
  }
  value = self->phase + 0.423f;
  self->phase = value;
  if (!(value <= 1.0f)) {
    self->phase = self->phase - 1.0f;
  }

  switch (self->mode) {
  case 1:
    value = self->alpha - 0.1f;
    self->alpha = value;
    if (value < 0.1f) {
      self->mode = self->mode + 1;
    }
    break;
  case 2:
    value = self->alpha + 0.1f;
    self->alpha = value;
    if (!(value <= 0.7f)) {
      self->alpha = 0.7f;
      self->mode = 0;
    }
    break;
  case 3:
    self->alpha = self->alpha - 0.04f;
    data = self->base.base.data;
    /* rows 0..2 of the root matrix scaled by 0.87, 1.0 and 0.87 */
    for (i = 0; i < 4; i++) {
      data->rootMatrix[i] = data->rootMatrix[i] * 0.87f;
      data->rootMatrix[4 + i] = data->rootMatrix[4 + i] * 1.0f;
      data->rootMatrix[8 + i] = data->rootMatrix[8 + i] * 0.87f;
    }
    if (self->alpha < 0.0f) {
      GfxEffectStopAttached(g_worldEffectMgr, -1, &self->base.base.data->rootMatrix[12]);
      GfxEffectStopAttached(g_worldEffectMgr, -1, self->effectPos[0]);
      GfxEffectStopAttached(g_worldEffectMgr, -1, self->effectPos[1]);
      GfxEffectStopAttached(g_worldEffectMgr, -1, self->effectPos[2]);
      GfxEffectStopOwned(g_worldEffectMgr, -1, self);
      GfxEffectStopOwned(g_btlUnitEffectMgr, -1, self);
      self->alpha = 0.0f;
      self->mode = self->mode + 1;
    }
    break;
  case 4:
    break;
  default:
    /* modes 5..19 and out-of-range modes bump the mode before the reset overwrites it */
    if (self->mode != 20) {
      self->mode = self->mode + 1;
    }
    data = self->base.base.data;
    data->rootMatrix[10] = 1.0f;
    data->rootMatrix[5] = 1.0f;
    data->rootMatrix[0] = 1.0f;
    data = self->base.base.data;
    data->rootMatrix[12] = self->base.base.pos[0];
    data->rootMatrix[13] = self->base.base.pos[1];
    data->rootMatrix[14] = self->base.base.pos[2];
    data->rootMatrix[15] = self->base.base.pos[3];
    self->alpha = 0.7f;
    self->mode = 0;
    break;
  }

  if (self->auraIndex >= 0 && (u32)self->auraIndex < 7) {
    const MemberFnPtr *member = &g_actorStageObjAttrLandmarkAuraFns[self->auraIndex];
    u8 *obj = (u8 *)self + member->delta;
    void *fn = member->pfn;

    if (member->index != 0) {
      const VtblEntry *entry = &(*(const VtblEntry **)(obj + (uintptr_t)member->pfn))[member->index];

      fn = entry->fn;
      obj += entry->delta;
    }
    ((void (*)(void *))fn)(obj);
  }
}
