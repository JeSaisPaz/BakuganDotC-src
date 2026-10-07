// bdc 0x088d819c GameGimmickCameraDtor
#include "bdc.h"

/* Destructor (vtable slot 1) of the surveillance camera gimmick (`GameGimmickCameraCtor`, vtables
   `0x08af325c`/`0x08af3304`): releases the view-cone sprites `+0x24c`/`+0x250` from their layers
   (`UiSpriteLayerRelease`), deletes the collider `+0x174`, the sound object (`GfxModelReleaseSound`),
   runs `GameGimmickDtor` and frees when `flags & 1`. */

typedef struct {
  u8 pad[0x214];
  void *layer;
} GameGimmickCameraSprite;

void GameGimmickCameraDtor(GameGimmickCamera *obj, u32 flags)
{
  if (obj != (GameGimmickCamera *)0x0) {
    obj->base.base.base.vtable = g_gameGimmickCameraVtbl;
    obj->base.vtbl2 = g_gameGimmickCameraVtbl2;
    if (obj->coneEffect != (void *)0x0) {
      UiSpriteLayerRelease(((GameGimmickCameraSprite *)obj->coneEffect)->layer, obj->coneEffect);
    }
    if (obj->spotEffect != (void *)0x0) {
      UiSpriteLayerRelease(((GameGimmickCameraSprite *)obj->spotEffect)->layer, obj->spotEffect);
    }
    if (obj->base.attached != (void *)0x0) {
      CoreNode *node = (CoreNode *)obj->base.attached;
      const VtblEntry *dtor = &((const VtblEntry *)node->vtable)[1];

      ((void (*)(void *, s32))dtor->fn)((u8 *)node + dtor->delta, 3);
      obj->base.attached = (void *)0x0;
    }
    GfxModelReleaseSound(&obj->base.base);
    GameGimmickDtor(&obj->base, 0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(obj, (char *)0x0, 0);
      MemUnlock();
    }
  }
}
