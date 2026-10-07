// bdc 0x08998090 UiWorldMapCreateMapModel
#include "bdc.h"

/* Sets up the 3D globe of `UiWorldMap`: initialises `camera` (eye (0, 0, 150),
   target 0, screen offset (-110, 0), `GfxCameraUpdate` all), loads `"menu_worldmap.gmo"`
   (`GfxModelCtor` → `mapModel`, allocated from the low end of the heap) with lighting on,
   specular 0.4 grey (power 8), position Z -30 copied into the root matrix translation, hooks the
   line and sea UV scrolls (`UiWorldMapHookLineMaterial`, `UiWorldMapHookSeaMaterial`), finds
   the `"_01_worldmap"`/`"_02_worldmap"` nodes (`globeNode`, their local matrices in
   `globeNodeMatrix[0/1]`), and sets ambient alpha 0 and root matrix scale 1.5. */

void UiWorldMapCreateMapModel(UiScreen *screen)
{
  UiWorldMap *map = (UiWorldMap *)screen;
  GfxCamera *cam = &map->camera;
  GfxModel *model;
  GfxModel *alloc;
  bool fromLow;
  float specular[4] __attribute__((aligned(16)));

  GfxCameraInit(cam);
  cam->target[0] = 0.0f;
  cam->target[1] = 0.0f;
  cam->target[2] = 0.0f;
  cam->target[3] = 0.0f;
  cam->eye[0] = 0.0f;
  cam->eye[1] = 0.0f;
  cam->eye[2] = 150.0f;
  cam->eye[3] = 0.0f;
  GfxCameraSetScreenOffset(-110.0f, 0.0f, cam);
  GfxCameraUpdate(cam, 0xffffffff);

  model = NULL;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  alloc = (GfxModel *)MemAlloc(sizeof(GfxModel), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (alloc != NULL) {
    GfxModelCtor(alloc, "menu_worldmap.gmo", 0);
    model = alloc;
  }
  /* no NULL check: a failed allocation faults here */
  map->mapModel = model;
  model->lighting = 1;
  specular[0] = 0.4f;
  specular[1] = 0.4f;
  specular[2] = 0.4f;
  specular[3] = 1.0f;
  GfxModelSetSpecular(8.0f, map->mapModel, specular, NULL);
  map->mapModel->pos[2] = -30.0f;
  model = map->mapModel;
  /* root matrix translation row = pos (one quad copy) */
  model->data->rootMatrix[12] = model->pos[0];
  model->data->rootMatrix[13] = model->pos[1];
  model->data->rootMatrix[14] = model->pos[2];
  model->data->rootMatrix[15] = model->pos[3];
  map->mapModel->data->rootMatrix[15] = 1.0f;
  UiWorldMapHookLineMaterial(screen);
  UiWorldMapHookSeaMaterial(screen);
  map->globeNode = GfxModelFindNode(map->mapModel, "_01_worldmap");
  map->globeNodeMatrix[0] = ((GmoNode *)map->globeNode)->localMatrix;
  map->globeNode = GfxModelFindNode(map->mapModel, "_02_worldmap");
  map->globeNodeMatrix[1] = ((GmoNode *)map->globeNode)->localMatrix;
  map->mapModel->ambient[3] = 0.0f;
  map->mapModel->data->rootMatrix[10] = 1.5f;
  map->mapModel->data->rootMatrix[5] = 1.5f;
  map->mapModel->data->rootMatrix[0] = 1.5f;
}
