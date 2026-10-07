// bdc 0x088d6048 GameGimmickCollectionBoxCtor
#include "bdc.h"

/* Constructor of the collection-box gimmick (kind 0x10 `EW_GMKOBJ_COLLECTION_BOX`,
   menu_itembox.gmo; 0x220 bytes, spawned by `GameFieldLoadObjects`): `GameGimmickCtor`, vtables
   `g_gameGimmickCollectionBoxVtbl` / `g_gameGimmickCollectionBoxVtbl2`. (The original also scales
   the record vector `+0x28` (20.12) by 20 into a stack temporary nothing reads; dropped.) Clears
   `motionName` and fills both slots (`GameGimmickCollectionBoxMotionName`), loading each
   `"<name>.gmo"` motion (`GmoMotionMgrGet`, `GmoMotionLoadFile`). Selects the open motion without
   looping at frame 0.2f (`GfxModelPlayMotionByName`), sets frame 0, calls vtable slot 6
   (`GfxModelSetMotionSpeed`) with 0.0f, sets `lidState = -1`, moves the model position into
   `triggerPos` and replaces it with `g_gameGimmickCollectionBoxTriggerPos`. Rebuilds the root matrix
   as the Y rotation by `rot[1] + 0.35f` scaled by 0.15 with translation `pos` and w = 1.
   Returns `obj`. */

CoreObject *GameGimmickCollectionBoxCtor(GameGimmickCollectionBox *obj, s32 kind, void *record, u16 typeId, u8 flag)
{
  const VtblEntry *entry;
  float *m;
  char file[80];
  float angle;
  float c;
  float s;
  int i;

  GameGimmickCtor(&obj->base, kind, record, typeId, flag);
  obj->base.base.base.vtable = g_gameGimmickCollectionBoxVtbl;
  obj->base.vtbl2 = g_gameGimmickCollectionBoxVtbl2;

  memset(obj->motionName, 0, sizeof(obj->motionName));
  for (i = 0; i < 2; i++) {
    GameGimmickCollectionBoxMotionName(obj, (u8)i, obj->motionName[i]);
    sprintf(file, "%s.gmo", obj->motionName[i]);
    GmoMotionLoadFile(GmoMotionMgrGet(), file);
  }

  GfxModelPlayMotionByName(0.2f, &obj->base.base, obj->motionName[0], false);
  GfxModelSwapMotionFrame(&obj->base.base, 0.0f);
  entry = &((const VtblEntry *)obj->base.base.base.vtable)[6];
  ((float (*)(void *, float))entry->fn)((u8 *)obj + entry->delta, 0.0f);

  obj->lidState = -1;
  obj->triggerPos.x = obj->base.base.pos[0];
  obj->triggerPos.y = obj->base.base.pos[1];
  obj->triggerPos.z = obj->base.base.pos[2];
  obj->triggerPos.w = obj->base.base.pos[3];
  obj->base.base.pos[0] = g_gameGimmickCollectionBoxTriggerPos.x;
  obj->base.base.pos[1] = g_gameGimmickCollectionBoxTriggerPos.y;
  obj->base.base.pos[2] = g_gameGimmickCollectionBoxTriggerPos.z;
  obj->base.base.pos[3] = g_gameGimmickCollectionBoxTriggerPos.w;

  /* Y rotation (vrot of the angle times 2/pi), its upper 3x4 scaled by 0.15. */
  angle = obj->base.base.rot[1] + 0.35f;
  c = __builtin_cosf(angle);
  s = __builtin_sinf(angle);
  m = obj->base.base.data->rootMatrix;
  m[0] = c * 0.15f;
  m[1] = 0.0f;
  m[2] = -s * 0.15f;
  m[3] = 0.0f;
  m[4] = 0.0f;
  m[5] = 0.15f;
  m[6] = 0.0f;
  m[7] = 0.0f;
  m[8] = s * 0.15f;
  m[9] = 0.0f;
  m[10] = c * 0.15f;
  m[11] = 0.0f;
  m[12] = obj->base.base.pos[0];
  m[13] = obj->base.base.pos[1];
  m[14] = obj->base.base.pos[2];
  m[15] = 1.0f;
  return &obj->base.base.base;
}
