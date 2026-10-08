// bdc 0x088c3ac8 GameFieldDtor
#include "bdc.h"

/* Destructor (vtable slot 1) of the field (world map) scene task `GameFieldTask`, task id 500
   (`GameFieldCtor`, 0x7a0 bytes, vtable `g_gameFieldTaskVtbl`): waits for the GE, closes the
   HUD, clears `g_btlBakuganList` (`BtlSetBakuganList``(NULL)`) and the actor list
   (`ActorClearList`), removes task 0x1e2, deletes the sprite layers, effect managers, screen
   effects, model chains, mesh-object lists (`g_gfxMeshObjList0`..5), colliders and motions,
   clears `g_worldEffectMgr`, deletes the owned object `+0x10`, the point list, the world-map
   task 0x189, the event manager and the character set, stores the task id into profile word 0x1d,
   clears `g_gfxSpriteFogEnable`, syncs story progress (`GameFieldSyncProgressFlags`), saves
   script flag bits 6..30 into `g_gameFieldSavedFlagBits` when `g_gameFieldReturnKind` is 3,
   destroys the guard-blind timer and the embedded camera, runs the base destructor and frees the
   task when `flags & 1`. Does nothing for a NULL task. */

/* GCC 2.x virtual destructor call: slot 1 of the vtable, `this` adjusted by the entry delta. */
#define GF_VDELETE(obj, vtbl)                                                        \
  do {                                                                               \
    const VtblEntry *dtor_ = &((const VtblEntry *)(vtbl))[1];                        \
    ((void (*)(void *, u32))dtor_->fn)((u8 *)(obj) + dtor_->delta, 3);               \
  } while (0)

void GameFieldDtor(CoreTask *task, u32 flags)
{
  GameFieldTask *field = (GameFieldTask *)task;
  GfxSpriteLayer *layer;
  GfxEffectMgr *effects;
  GameFieldAux *aux;
  s32 i;

  if (task == NULL) {
    return;
  }
  task->vtable = g_gameFieldTaskVtbl;
  GfxWaitGeIdle();
  GameFieldCloseHud();
  BtlSetBakuganList(NULL);
  ActorClearList();
  CoreTaskRemoveById(0x1e2);

  layer = field->layers[1];
  if (layer != NULL) {
    GF_VDELETE(layer, layer->vtbl);
    field->layers[1] = NULL;
  }
  layer = field->layers[0];
  if (layer != NULL) {
    GF_VDELETE(layer, layer->vtbl);
    field->layers[0] = NULL;
  }
  layer = field->spriteLayer;
  if (layer != NULL) {
    GF_VDELETE(layer, layer->vtbl);
    field->spriteLayer = NULL;
  }
  if (field->screenFx != NULL) {
    GameFieldScreenFxDtor(field->screenFx, 3);
    field->screenFx = NULL;
  }

  GfxModelChainDeleteAll((CoreObject *)field->npcs);
  GfxModelChainDeleteAll(field->objList634.head);
  GfxModelChainDeleteAll(field->objList640.head);
  GfxModelChainDeleteAll(((CoreObjectList *)field->ballList)->head);
  GfxModelChainDeleteAll((CoreObject *)field->gimmicks);
  GfxModelChainDeleteAll(field->objList670.head);
  ActorStageObjSystemShutdown();
  GfxEffectModelsFree();

  effects = field->effectMgr;
  if (effects != NULL) {
    GF_VDELETE(effects, effects->base.vtbl);
    field->effectMgr = NULL;
  }
  effects = field->effectMgr2;
  if (effects != NULL) {
    GF_VDELETE(effects, effects->base.vtbl);
    field->effectMgr2 = NULL;
  }
  layer = field->layers[2];
  if (layer != NULL) {
    GF_VDELETE(layer, layer->vtbl);
    field->layers[2] = NULL;
  }

  g_worldEffectMgr = NULL;
  CoreObjectChainDeleteAll(g_gfxMeshObjList0.head);
  CoreObjectChainDeleteAll(g_gfxMeshObjList1.head);
  CoreObjectChainDeleteAll(g_gfxMeshObjList2.head);
  CoreObjectChainDeleteAll(g_gfxMeshObjList3.head);
  CoreObjectChainDeleteAll(g_gfxMeshObjList4.head);
  CoreObjectChainDeleteAll(g_gfxMeshObjList5.head);
  CollisionDeleteAllColliders();
  GmoMotionFreeAll(GmoMotionMgrGet(), 0);

  aux = field->aux;
  if (aux != NULL) {
    GF_VDELETE(aux, aux->base.vtable);
    field->aux = NULL;
  }
  GameFieldPointListDestroy();

  if (field->worldMapTask != NULL) {
    CoreTaskRemoveById(0x189);
  }
  if (field->events != NULL) {
    GameEvent470Release(field->events);
    GameEventRequestRemove(&field->events->base);
    field->events = NULL;
  }
  if (field->charSet != NULL) {
    GameFieldCharSetClearForDtor(field->charSet);
    if (field->charSet != NULL) {
      GameFieldCharSetDtor(field->charSet, 3);
      field->charSet = NULL;
    }
  }

  SaveProfileSetWord(SaveGetProfile(), 0x1d, (u32)task->id);
  g_gfxSpriteFogEnable = 0;
  GameFieldSyncProgressFlags();
  if (g_gameFieldReturnKind == 3) {
    for (i = 0; i < 25; i++) {
      g_gameFieldSavedFlagBits[i] = CoreBitsetTest((u32)(i + 6), g_scriptGlobalBits);
    }
  }

  GameFieldGuardBlindDtor(&field->guardBlind, 2);
  GameFieldCameraDtor((GameFieldCamera *)field->camera, 2);
  CoreTaskDestroy(task, 0);
  if ((flags & 1) != 0) {
    MemLock();
    MemFree(task, NULL, 0);
    MemUnlock();
  }
}
