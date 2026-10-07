// bdc 0x088bf650 GameFieldLoadObjects
#include "bdc.h"

/* Loads the object layout `F%d_b%02d.fol` (field `area`, block `block`, `GameFieldLayout`) from the
   pack chain `g_ioLzsPackages` and spawns its records; returns at once when the file is missing.
   Otherwise it clears `g_gameFieldSkippedObjCount`, zeroes `field->playerStart` (VFPU bank C720),
   clears `field->collidableCount` and walks the 0x38-byte `GameGimmickRecord`s:
   - a record that `GameFieldIsPlayerStartRecord` accepts sets `playerStart` to its position x 20;
   - on stage 0x20 (`g_scriptGlobalVars[1]`) type ids 800, 0x321 and 0x327 are skipped;
   - by type id (`recordType`): trigger zones (800, 0x321, 0x323, 0x324, 0x327, 6000, 0x1773, 0x1774),
     collection box (0x322), jet door (0x325), effect markers (0x73, 0x74, 0x77), touch spot (0xbbe),
     switch (0xbc7), barrier (0xbc8), solid (0xbd1), item box (0xbdf, only while
     `GameFieldIsEventEntryFlagClear`), steam (0xbd2), camera (0xbd9), IR sensor (0xbda), collidables
     (0xbd0 kind 7 with noEffect -1, 0xbdc kind 6 with the running `collidableCount`), core points
     (0x1778, kinds 0xa..0xf from `coreSize` and `visFlags` bit 0x10), crystals (`ActorCrystalCtor`,
     0x1388..0x138d), crystal stage objects (`ActorStageObjCrystalCtor`, 0x138e..0x1399, appended to the
     field task's `stageObjs`), entry points (0x1771, into `field->entryPoints`), effect 3 at the position
     (0x1772) and effects 0xf/0x10 alternating (0x1779, yaw in the effect's `vec1d0[0]`); switch,
     barrier, solid and item box are disabled at once (virtual slot 16, argument 1) unless
     `GameFieldIsEventEntryOpen`. Any other type id increments `g_gameFieldSkippedObjCount`.
   Every spawn allocates from the low heap. Positions are the record's 20.12 ints x 20 with `w` 0 (the
   bank's S713); the crystals get the yaw in `w`. Yaws are `heading * 2pi / 65535` wrapped into
   (-pi, pi]. */

/* The original inlines this low-heap `new` sequence at every spawn. */
#define FIELD_NEW(dst, size)            \
  do {                                  \
    bool fromLow_;                      \
    MemLock();                          \
    fromLow_ = MemIsAllocFromLow();     \
    MemSetAllocFromLow(true);           \
    (dst) = MemAlloc((size), NULL, 0);  \
    MemSetAllocFromLow(fromLow_);       \
    MemUnlock();                        \
  } while (0)

/* The record's 20.12 fixed-point position scaled by 20 (`vscl.t` by 20.0f), `w` = 0. */
#define FIELD_POS(dst, rec)                                          \
  do {                                                               \
    (dst)[0] = (float)(rec)->pos[0] * 0.000244140625f * 20.0f;       \
    (dst)[1] = (float)(rec)->pos[1] * 0.000244140625f * 20.0f;       \
    (dst)[2] = (float)(rec)->pos[2] * 0.000244140625f * 20.0f;       \
    (dst)[3] = 0.0f;                                                 \
  } while (0)

void GameFieldLoadObjects(CoreTask *task, s32 area, s32 block)
{
  GameFieldTask *field = (GameFieldTask *)task;
  char name[64];
  GameFieldLayout *layout;
  GameGimmickRecord *rec;
  s32 count;
  s32 i;
  s32 type;
  s32 kind;
  s32 idx;
  s32 effectToggle;
  float heading;
  GameGimmick *gimmick;
  const VtblEntry *slot;
  GameFieldEntryPoint *entry;
  GfxEffectMgr *mgr;
  GfxEffect *effect;
  CoreObject *node;
  float pos[4];

  sprintf(name, "F%d_b%02d.fol", area, block);
  layout = CorePackChainFind(g_ioLzsPackages, name);
  if (layout == NULL) {
    return;
  }
  g_gameFieldSkippedObjCount = 0;
  count = layout->count;
  rec = layout->records;
  effectToggle = 0;
  field->playerStart[0] = 0.0f;
  field->playerStart[1] = 0.0f;
  field->playerStart[2] = 0.0f;
  field->playerStart[3] = 0.0f;
  field->collidableCount = 0;

  for (i = 0; i < count; i++, rec++) {
    if (GameFieldIsPlayerStartRecord(area, block, i) != 0) {
      FIELD_POS(field->playerStart, rec);
    }
    if (g_scriptGlobalVars[1] == 0x20 &&
        (rec->recordType == 800 || rec->recordType == 0x321 || rec->recordType == 0x327)) {
      continue;
    }
    type = rec->recordType;

    switch (type) {
    case 0x73:
    case 0x74:
    case 0x77: {
      GameGimmickEffectMarker *marker;

      FIELD_NEW(marker, 400);
      if (marker != NULL) {
        GameGimmickEffectMarkerCtor(marker, 0, rec, (u16)rec->recordType, 0);
      }
      break;
    }

    case 800:
    case 0x321:
    case 0x323:
    case 0x324:
    case 0x327:
    case 6000:
    case 0x1773:
    case 0x1774: {
      GameGimmickTriggerZone *zone;

      FIELD_NEW(zone, 0x270);
      if (zone != NULL) {
        GameGimmickTriggerZoneCtor(zone, 0, rec, (u16)rec->recordType, 0);
      }
      break;
    }

    case 0x322: {
      GameGimmickCollectionBox *box;

      FIELD_NEW(box, 0x220);
      if (box != NULL) {
        GameGimmickCollectionBoxCtor(box, 0x10, rec, (u16)rec->recordType, 0);
      }
      break;
    }

    case 0x325: {
      GameGimmickJetDoor *door;

      FIELD_NEW(door, 0x1b0);
      if (door != NULL) {
        GameGimmickJetDoorCtor(door, 0x11, rec, (u16)rec->recordType, 0);
      }
      break;
    }

    case 0xbbe: {
      GameGimmickTouchSpot *spot;

      FIELD_NEW(spot, 0x1c0);
      if (spot != NULL) {
        GameGimmickTouchSpotCtor(spot, 0, rec, (u16)rec->recordType, 0);
      }
      break;
    }

    case 0xbc7: {
      GameGimmickSwitch *sw;

      gimmick = NULL;
      FIELD_NEW(sw, 0x1e0);
      if (sw != NULL) {
        GameGimmickSwitchCtor(sw, 1, rec, (u16)rec->recordType, 0);
        gimmick = &sw->base;
      }
      if (GameFieldIsEventEntryOpen(task, rec->param) == 0) {
        slot = &((const VtblEntry *)gimmick->base.base.vtable)[16];
        ((void (*)(void *, u8))slot->fn)((u8 *)gimmick + slot->delta, 1);
      }
      break;
    }

    case 0xbc8: {
      GameGimmickBarrier *barrier;

      gimmick = NULL;
      FIELD_NEW(barrier, 0x1a0);
      if (barrier != NULL) {
        GameGimmickBarrierCtor(barrier, 2, rec, (u16)rec->recordType, 0);
        gimmick = &barrier->base;
      }
      if (GameFieldIsEventEntryOpen(task, rec->param) == 0) {
        slot = &((const VtblEntry *)gimmick->base.base.vtable)[16];
        ((void (*)(void *, u8))slot->fn)((u8 *)gimmick + slot->delta, 1);
      }
      break;
    }

    case 0xbd0: {
      GameGimmickCollidable *collidable;

      FIELD_NEW(collidable, 0x1f0);
      if (collidable != NULL) {
        GameGimmickCollidableCtor(collidable, 7, rec, (u16)rec->recordType, 0, -1);
      }
      break;
    }

    case 0xbd1: {
      GameGimmick *solid;

      gimmick = NULL;
      FIELD_NEW(solid, 0x180);
      if (solid != NULL) {
        GameGimmickSolidCtor(solid, 8, rec, (u16)rec->recordType, 0);
        gimmick = solid;
      }
      if (GameFieldIsEventEntryOpen(task, rec->param) == 0) {
        slot = &((const VtblEntry *)gimmick->base.base.vtable)[16];
        ((void (*)(void *, u8))slot->fn)((u8 *)gimmick + slot->delta, 1);
      }
      break;
    }

    case 0xbd2: {
      GameGimmickSteam *steam;

      FIELD_NEW(steam, 400);
      if (steam != NULL) {
        GameGimmickSteamCtor(steam, 5, rec, (u16)rec->recordType, 0);
      }
      break;
    }

    case 0xbd9: {
      GameGimmickCamera *camera;

      FIELD_NEW(camera, 0x260);
      if (camera != NULL) {
        GameGimmickCameraCtor(camera, 3, rec, (u16)rec->recordType, 0);
      }
      break;
    }

    case 0xbda: {
      GameGimmickIrSensor *sensor;

      FIELD_NEW(sensor, 0x200);
      if (sensor != NULL) {
        GameGimmickIrSensorCtor(sensor, 4, rec, (u16)rec->recordType, 0);
      }
      break;
    }

    case 0xbdc: {
      GameGimmickCollidable *collidable;
      s32 index;

      FIELD_NEW(collidable, 0x1f0);
      if (collidable != NULL) {
        index = field->collidableCount;
        field->collidableCount = index + 1;
        GameGimmickCollidableCtor(collidable, 6, rec, (u16)rec->recordType, 0, index);
      }
      break;
    }

    case 0xbdf: {
      GameGimmickItemBox *itemBox;

      if (GameFieldIsEventEntryFlagClear(task, rec->param) == 0) {
        break;
      }
      gimmick = NULL;
      FIELD_NEW(itemBox, 0x200);
      if (itemBox != NULL) {
        GameGimmickItemBoxCtor(itemBox, 9, rec, (u16)rec->recordType, 0);
        gimmick = &itemBox->base;
      }
      if (GameFieldIsEventEntryOpen(task, rec->param) == 0) {
        slot = &((const VtblEntry *)gimmick->base.base.vtable)[16];
        ((void (*)(void *, u8))slot->fn)((u8 *)gimmick + slot->delta, 1);
      }
      break;
    }

    case 5000:
    case 5001:
    case 5002:
    case 5003:
    case 5004:
    case 5005: {
      s32 crystalTypes[7];
      s32 crystalType;
      ActorCrystal *crystal;

      crystalTypes[0] = 2;
      crystalTypes[1] = 5;
      crystalTypes[2] = 4;
      crystalTypes[3] = 6;
      crystalTypes[4] = 0;
      crystalTypes[5] = 1;
      crystalTypes[6] = 0;
      crystalType = crystalTypes[type - 5000];
      heading = (float)rec->heading * 6.28318548f * 1.52590219e-05f;
      if (!(heading <= 3.14159274f)) {
        heading = heading - 6.28318548f;
      } else if (heading <= -3.14159274f) {
        heading = heading + 6.28318548f;
      }
      FIELD_POS(pos, rec);
      pos[3] = heading;
      FIELD_NEW(crystal, 0xa90);
      if (crystal != NULL) {
        ActorCrystalCtor(crystal, 0x21, 2, pos, 0, crystalType, crystalType);
      }
      break;
    }

    case 0x138e:
    case 0x138f:
    case 0x1390:
    case 0x1391:
    case 0x1392:
    case 0x1393:
    case 0x1394:
    case 0x1395:
    case 0x1396:
    case 0x1397:
    case 0x1398:
    case 0x1399: {
      s32 variants[7];
      ActorStageObjCrystal *stageObj;

      /* two six-id ranges with the same variant table */
      variants[0] = 0;
      variants[1] = 1;
      variants[2] = 2;
      variants[3] = 1;
      variants[4] = 3;
      variants[5] = 4;
      variants[6] = 5;
      idx = (type < 0x1394) ? type - 0x138e : type - 0x1394;
      heading = (float)rec->heading * 6.28318548f * 1.52590219e-05f;
      if (!(heading <= 3.14159274f)) {
        heading = heading - 6.28318548f;
      } else if (heading <= -3.14159274f) {
        heading = heading + 6.28318548f;
      }
      FIELD_POS(pos, rec);
      pos[3] = heading;
      FIELD_NEW(stageObj, 0x3a0);
      node = NULL;
      if (stageObj != NULL) {
        ActorStageObjCrystalCtor(stageObj, pos, variants[idx], (u16)rec->recordType);
        node = &stageObj->base.base.base;
      }
      CoreObjectListAppend(node, &((GameFieldTask *)GameFieldFindTask())->stageObjs);
      break;
    }

    case 0x1771:
      entry = &field->entryPoints[(rec->entrySlot & 0xf0) >> 4];
      entry->pos[0] = rec->pos[0];
      entry->pos[1] = rec->pos[1];
      entry->pos[2] = rec->pos[2];
      entry->heading = rec->heading;
      entry->id = rec->entryId;
      break;

    case 0x1772:
      mgr = g_worldEffectMgr;
      FIELD_POS(pos, rec);
      GfxEffectSpawn(mgr, 3, pos);
      break;

    case 0x1778: {
      GameGimmickCorePoint *corePoint;

      switch (rec->coreSize) {
      case 0:
        kind = (rec->visFlags & 0x10) != 0 ? 0xd : 0xa;
        break;
      case 1:
        kind = (rec->visFlags & 0x10) != 0 ? 0xe : 0xb;
        break;
      case 2:
        kind = (rec->visFlags & 0x10) != 0 ? 0xf : 0xc;
        break;
      default:
        kind = 0;
        break;
      }
      if (kind != 0) {
        FIELD_NEW(corePoint, 0x2e0);
        if (corePoint != NULL) {
          GameGimmickCorePointCtor(corePoint, kind, rec, (u16)rec->recordType, 0);
        }
      }
      break;
    }

    case 0x1779:
      heading = (float)rec->heading * 6.28318548f * 1.52590219e-05f;
      if (!(heading <= 3.14159274f)) {
        heading = heading - 6.28318548f;
      } else if (heading <= -3.14159274f) {
        heading = heading + 6.28318548f;
      }
      mgr = g_worldEffectMgr;
      kind = (effectToggle & 1) + 0xf;
      effectToggle++;
      FIELD_POS(pos, rec);
      effect = GfxEffectSpawn(mgr, kind, pos);
      effect->vec1d0[0] = heading;
      break;

    default:
      g_gameFieldSkippedObjCount++;
      break;
    }
  }
}
