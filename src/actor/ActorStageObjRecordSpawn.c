// bdc 0x088b22f4 ActorStageObjRecordSpawn
#include "bdc.h"

/* Spawns what a stage layout spawn record (`ActorStageObjRecordCtor`: position `+0x20`, id
   `+0x30`, kind `+0x32`, type `+0x34`, done `+0x3a`, spawned `+0x3b`) describes, and returns the
   created stage object (NULL when none; the caller keeps it in `liveCount`). Every path that ends
   in a spawn attempt (also a failed one) bumps `g_stageObjLiveCount`; the early "done" exits do
   not. Type 5 records are skipped. In battle (field task 500 absent):
   landmark-category records only spawn in event battles (script global 8 == 1) as type-4 slots
   7..9, which create the attribute landmark saved in profile byte `+0x84 + (id-7)`
   (`ActorStageObjRecordMapAttrLandmarkKind`, `ActorStageObjAttrLandmarkCreate`); kind 0x58 is
   skipped; type 0xc records in mode 2 become item spawners (`BtlItemSpawnerCtor`); everything
   else is built with `ActorStageObjCreateByKind` and linked to its record
   (`ActorStageObjSetRecord`, type stored at `+0x28c`). In the field scene, kind 0xce (`SMOKE_L`)
   just spawns effect 0x34 and other non-landmark records are built likewise. */

void *ActorStageObjRecordSpawn(ActorStageObjRecord *rec)
{
  ActorStageObjBase *obj;
  CoreObject *spawner;
  CoreObjectList *list;
  void *mem;
  bool fromLow;
  u32 value;
  s32 kind;
  float pos[4] __attribute__((aligned(16)));

  obj = NULL;
  if (rec->field32[1] == 5) {
    rec->doneFlags[0] = 1;
    return NULL;
  }
  if (CoreTaskExists(500) != 0) {
    /* field scene */
    if (ActorStageObjGetCategory(rec->field32[0]) == 0xb) {
      if (rec->field32[0] == 0xce) {
        GfxEffectSpawn(g_worldEffectMgr, 0x34, rec->pos);
        rec->doneFlags[0] = 1;
      }
    }
    else {
      if (ActorStageObjRecordIsNotLandmark(rec) == 0) {
        rec->doneFlags[0] = 1;
        return NULL;
      }
      /* lv.q/sv.q: copy the record position to a stack quad */
      pos[0] = rec->pos[0];
      pos[1] = rec->pos[1];
      pos[2] = rec->pos[2];
      pos[3] = rec->pos[3];
      obj = (ActorStageObjBase *)ActorStageObjCreateByKind(rec->field32[0], pos);
      if (obj != NULL) {
        ActorStageObjSetRecord(obj, rec);
        rec->doneFlags[1] = 1;
      }
    }
  }
  else if (ActorStageObjRecordIsNotLandmark(rec) == 0) {
    /* battle, landmark record: only event battles place the saved attribute landmarks */
    if (g_scriptGlobalVars[8] == 1) {
      if (rec->field32[1] != 4) {
        rec->doneFlags[0] = 1;
        return NULL;
      }
      if (rec->field30 < 7 || rec->field30 >= 10) {
        rec->doneFlags[0] = 1;
        return NULL;
      }
      value = SaveGetProfile()->data->placedHolograms[(u8)(rec->field30 - 7)];
      if (value < 0xe || value > 0x1f) {
        value = 0;
      }
      if (value == 0 || value == 0x20) {
        rec->doneFlags[0] = 1;
        return NULL;
      }
      kind = ActorStageObjRecordMapAttrLandmarkKind(value);
      obj = (ActorStageObjBase *)ActorStageObjAttrLandmarkCreate(kind, rec->pos, rec->field32[0]);
      ActorStageObjSetRecord(obj, rec);
      rec->field32[3] = (s16)kind;
      rec->doneFlags[1] = 1;
    }
  }
  else {
    /* battle, ordinary record */
    if (rec->field32[0] == 0x58) {
      rec->doneFlags[0] = 1;
      return NULL;
    }
    if (g_scriptGlobalVars[8] == 2 && rec->field32[1] == 0xc) {
      spawner = NULL;
      MemLock();
      fromLow = MemIsAllocFromLow();
      MemSetAllocFromLow(true);
      mem = MemAlloc(sizeof(BtlItemSpawner), NULL, 0);
      MemSetAllocFromLow(fromLow);
      MemUnlock();
      if (mem != NULL) {
        BtlItemSpawnerCtor(mem, rec);
        spawner = (CoreObject *)mem;
      }
      /* the list pointer is read before the lazy init, so the first spawner is appended to NULL */
      list = g_btlItemSpawnerList;
      if (list == NULL) {
        BtlItemSpawnerInitList();
      }
      CoreObjectListAppend(spawner, list);
      rec->doneFlags[0] = 1;
      return NULL;
    }
    ActorStageObjGetCategory(rec->field32[0]);
    /* lv.q/sv.q: copy the record position to a stack quad */
    pos[0] = rec->pos[0];
    pos[1] = rec->pos[1];
    pos[2] = rec->pos[2];
    pos[3] = rec->pos[3];
    obj = (ActorStageObjBase *)ActorStageObjCreateByKind(rec->field32[0], pos);
    if (obj == NULL) {
      rec->doneFlags[0] = 1;
    }
    else {
      rec->doneFlags[1] = 1;
      ActorStageObjSetRecord(obj, rec);
      obj->recordArg = rec->field32[1];
    }
  }
  g_stageObjLiveCount = g_stageObjLiveCount + 1;
  return obj;
}
