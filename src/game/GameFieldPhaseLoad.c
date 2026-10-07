// bdc 0x088c4fb4 GameFieldPhaseLoad
#include "bdc.h"

/* Phase 0 (loading) of the field (world map) scene task, task id 500 (`GameFieldCtor`, 0x7a0
   bytes), stepped by `subState` (+0x61c):
   0  opens the now-loading screen if needed and fades to black (20 frames), then step 3;
   3  allocates the stage package (`aux`, an `IoLzsPackage`) and loads
      `GameStageGetPackagePath(g_scriptGlobalVars[1])`; once it is loaded (VRAM textures enabled meanwhile) step 4;
   4  when the loading screen (task 0x2774) reports field 3 == 3 and the fade is done, fades back in;
   5  after the fade: builds the stage (`GameStageBuild(stage, &objList640)`), the Bakugan list
      `+0x634`, effect managers (`particle_02.ptb`, plus `particle_01.ptb` on stage 1), the screen
      effects `+0x610`; on stage index 0x20..0x23 the world-map blur task (`menu_worldmap.gmo`,
      own camera, a billboard on layer 0) and the TV material scrolls of the extra stage model;
      then the event table `f%d.eset`, the object layout, actor/ball lists and the character set;
   6  hooks the field camera to the player and makes it the active camera, snaps it;
   7/8 closes the loading screen and waits until it is gone;
   9  waits for the sound groups to load; 10 creates the event task 0x1d6 and loads the room script;
   11 loads the stage voice package (`g_gameFieldStageVoices`); 12 waits for it;
   1, 2 and anything above 12 end the phase: frame skip 1, `phase` = 1, `subState` = 0. */

void GameFieldPhaseLoad(CoreTask *task)
{
  GameFieldTask *field = (GameFieldTask *)task;
  char name[64];
  float specular[4];
  float projScale[4];
  float rot[4][4];
  float result[16];
  float angle;
  float c;
  float s;
  float sum;
  s32 j;
  s32 r;
  s32 k;
  bool fromLow;
  GfxFader *fader;
  IoLzsPackage *pkg;
  IoLzsPackage *pkgResult;
  void **fx;
  void **fxResult;
  GfxPlayerBlurTask *blur;
  GfxPlayerBlurTask *blurResult;
  GfxModel *model;
  GfxModel *modelResult;
  GfxCamera *cam;
  GfxCamera *camResult;
  GfxModel *extra;
  GfxMaterialState *state;
  GfxSprite *sprite;
  void *charSet;
  void *charSetResult;
  void *eset;
  UiLoading *ui;
  Actor *player;
  GameFieldCamera *fieldCam;
  GameEvent470 *events;
  const VtblEntry *slot;
  u32 uiField;
  s32 voicePac;
  s16 i;

  switch (field->subState) {
  case 0:
    if (!UiLoadingIsOpen()) {
      UiLoadingOpen();
      GfxGetActiveFader()->sortKey = 20000.0f;
      fader = GfxGetActiveFader();
      fader->start[0] = 0.0f;
      fader->start[1] = 0.0f;
      fader->start[2] = 0.0f;
      fader->start[3] = 1.0f;
      fader = GfxGetActiveFader();
      fader->end[0] = 0.0f;
      fader->end[1] = 0.0f;
      fader->end[2] = 0.0f;
      fader->end[3] = 0.0f;
      GfxFaderStart(GfxGetActiveFader(), 20);
    }
    field->subState = 3;
    break;

  case 3:
    if (field->aux == NULL) {
      MemLock();
      fromLow = MemIsAllocFromLow();
      MemSetAllocFromLow(true);
      pkg = MemAlloc(sizeof(IoLzsPackage), NULL, 0);
      MemSetAllocFromLow(fromLow);
      MemUnlock();
      pkgResult = NULL;
      if (pkg != NULL) {
        IoLzsPackageCtor(pkg);
        pkgResult = pkg;
      }
      field->aux = (GameFieldAux *)pkgResult;
    }
    pkg = (IoLzsPackage *)field->aux;
    if (IoLzsPackageStartLoad(pkg, GameStageGetPackagePath(g_scriptGlobalVars[1]), 10, 1, 0) != 0) {
      g_gfxTexVramEnabled = 1;
      if (IoLzsPackagePoll((IoLzsPackage *)field->aux, 1) != 0) {
        field->subState = 4;
      }
    }
    break;

  case 4:
    ui = CoreTaskFind(0x2774);
    slot = &((const VtblEntry *)ui->base.vtable)[6];
    uiField = ((u32 (*)(void *, u32))slot->fn)((u8 *)ui + slot->delta, 3);
    if (GfxFaderIsFinished(GfxGetActiveFader()) && uiField == 3) {
      GfxGetActiveFader()->sortKey = 20000.0f;
      fader = GfxGetActiveFader();
      fader->start[0] = 0.0f;
      fader->start[1] = 0.0f;
      fader->start[2] = 0.0f;
      fader->start[3] = 0.0f;
      fader = GfxGetActiveFader();
      fader->end[0] = 0.0f;
      fader->end[1] = 0.0f;
      fader->end[2] = 0.0f;
      fader->end[3] = 1.0f;
      GfxFaderStart(GfxGetActiveFader(), 20);
      field->subState = field->subState + 1;
    }
    break;

  case 5:
    if (!GfxFaderIsFinished(GfxGetActiveFader())) {
      break;
    }
    field->field3 = 1;
    g_gfxTexVramEnabled = 0;
    GameStageBuild(g_scriptGlobalVars[1], &field->objList640);
    GfxPuffLoadTexture();
    GfxMeshObjClearLists();
    BtlInitBakuganList(&field->objList634);
    GameFieldCreateEffectManager(task, CorePackChainFind(g_ioLzsPackages, "particle_02.ptb"));
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    fx = MemAlloc(sizeof(void *), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    fxResult = NULL;
    if (fx != NULL) {
      GameFieldScreenFxCtor(fx);
      fxResult = fx;
    }
    field->screenFx = fxResult;
    if (g_scriptGlobalVars[1] == 1) {
      GameFieldCreateEffectManager2(task, CorePackChainFind(g_ioLzsPackages, "particle_01.ptb"));
    }
    if (g_gameStageIndex >= 0x20 && g_gameStageIndex < 0x24) {
      MemLock();
      fromLow = MemIsAllocFromLow();
      MemSetAllocFromLow(true);
      blur = MemAlloc(sizeof(GfxPlayerBlurTask), NULL, 0);
      MemSetAllocFromLow(fromLow);
      MemUnlock();
      blurResult = NULL;
      if (blur != NULL) {
        MemLock();
        fromLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        model = MemAlloc(sizeof(GfxModel), NULL, 0);
        MemSetAllocFromLow(fromLow);
        MemUnlock();
        modelResult = NULL;
        if (model != NULL) {
          GfxModelCtor(model, "menu_worldmap.gmo", 0);
          modelResult = model;
        }
        MemLock();
        fromLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        cam = MemAlloc(sizeof(GfxCamera), NULL, 0);
        MemSetAllocFromLow(fromLow);
        MemUnlock();
        camResult = NULL;
        if (cam != NULL) {
          GfxCameraCtor(&cam->base);
          camResult = cam;
        }
        GfxPlayerBlurTaskCtor(blur, modelResult, camResult, -1);
        blurResult = blur;
      }
      field->worldMapTask = &blurResult->base;
      /* the binary dereferences the task even when the allocation failed */
      model = blurResult->model;
      model->lighting = 1;
      specular[0] = 0.4f;
      specular[1] = 0.4f;
      specular[2] = 0.4f;
      specular[3] = 1.0f;
      GfxModelSetSpecular(7.0f, model, specular, NULL);
      model->ambient[0] = 0.0f;
      model->ambient[1] = 0.0f;
      model->ambient[2] = 0.0f;
      model->ambient[3] = 1.0f;
      GfxModelSetMaterialAnimCallback(model, "psp_line__BA", GameFieldLineDlWriteTexOffsetV,
                                      model->velocity);
      state = GfxModelFindMaterialState(model, "psp_line__BA");
      /* fog off (bit 2), lighting off (bit 3) */
      state->renderFlags = (u8)((state->renderFlags & ~0xc) | 8);
      ((GfxPlayerBlurTask *)field->worldMapTask)->camera->nearZ = 0.5f;
      cam = ((GfxPlayerBlurTask *)field->worldMapTask)->camera;
      cam->eye[0] = 0.0f;
      cam->eye[1] = 50.0f;
      cam->eye[2] = -80.0f;
      cam->eye[3] = 0.0f;
      GfxCameraUpdate(((GfxPlayerBlurTask *)field->worldMapTask)->camera, 0xffffffffu);
      cam = ((GfxPlayerBlurTask *)field->worldMapTask)->camera;
      projScale[0] = 1.0f;
      projScale[1] = 0.58f;
      projScale[2] = 1.0f;
      projScale[3] = 0.0f;
      /* scale the first three rows of the projection (`vscl.q`): y by 0.58 */
      cam->proj.x.x = cam->proj.x.x * projScale[0];
      cam->proj.x.y = cam->proj.x.y * projScale[0];
      cam->proj.x.z = cam->proj.x.z * projScale[0];
      cam->proj.x.w = cam->proj.x.w * projScale[0];
      cam->proj.y.x = cam->proj.y.x * projScale[1];
      cam->proj.y.y = cam->proj.y.y * projScale[1];
      cam->proj.y.z = cam->proj.y.z * projScale[1];
      cam->proj.y.w = cam->proj.y.w * projScale[1];
      cam->proj.z.x = cam->proj.z.x * projScale[2];
      cam->proj.z.y = cam->proj.z.y * projScale[2];
      cam->proj.z.z = cam->proj.z.z * projScale[2];
      cam->proj.z.w = cam->proj.z.w * projScale[2];
      sprite = GfxSpriteLayerCreateBillboard(field->layers[0],
                                             ((GfxPlayerBlurTask *)field->worldMapTask)->texture);
      /* `vmidt.q`: identity matrix */
      for (j = 0; j < 16; j++) {
        sprite->matrix[j] = (j % 5 == 0) ? 1.0f : 0.0f;
      }
      sprite->billboardMode = 1;
      sprite->blendMode = 2;
      sprite->matrix[0] = -12.0f;
      sprite->matrix[5] = -12.0f;
      sprite->matrix[10] = 1.0f;
      /* Y rotation of pi/2 (0x3fc90fdb times the bank's S703 = 2/pi gives quarter turns for `vrot`,
         so cos/sin of the angle itself); rows of the matrix whose columns `vrot`/`vidt` build. */
      angle = 1.57079637f;
      c = __builtin_cosf(angle);
      s = __builtin_sinf(angle);
      rot[0][0] = c;    rot[0][1] = 0.0f; rot[0][2] = s;    rot[0][3] = 0.0f;
      rot[1][0] = 0.0f; rot[1][1] = 1.0f; rot[1][2] = 0.0f; rot[1][3] = 0.0f;
      rot[2][0] = -s;   rot[2][1] = 0.0f; rot[2][2] = c;    rot[2][3] = 0.0f;
      rot[3][0] = 0.0f; rot[3][1] = 0.0f; rot[3][2] = 0.0f; rot[3][3] = 1.0f;
      /* `vmmul.q E200, E100, E000`: M200 = M000 * M100 = rot * matrix (matrix stored column per
         16 bytes): new matrix[j*4+r] = sum_k rot[r][k] * matrix[j*4+k]. */
      for (j = 0; j < 4; j++) {
        for (r = 0; r < 4; r++) {
          sum = rot[r][0] * sprite->matrix[j * 4 + 0];
          for (k = 1; k < 4; k++) {
            sum = sum + rot[r][k] * sprite->matrix[j * 4 + k];
          }
          result[j * 4 + r] = sum;
        }
      }
      for (j = 0; j < 16; j++) {
        sprite->matrix[j] = result[j];
      }
      sprite->posX = 79.5f;
      sprite->posY = 24.5f;
      sprite->posZ = 5.4f;
      sprite->posW = 0.0f;
      sprite->alpha = 0.7f;
      extra = (GfxModel *)g_gameStageExtraModels[0];
      GfxModelSetMaterialAnimCallback(extra, "tv_img_02", GameFieldTvDlWriteTexOffsetU,
                                      &extra->velocity[0]);
      GfxModelSetMaterialAnimCallback(extra, "tv_img_03", GameFieldTvDlWriteTexOffsetU,
                                      &extra->velocity[1]);
    }
    BtlSetAnimPhaseCounter(0x1b);
    sprintf(name, "f%d.eset", g_gameEventFlags[0]);
    eset = CorePackChainFind(g_ioLzsPackages, name);
    field->eventTable = eset;
    GameFieldLoadObjects(task, g_gameEventFlags[0], g_gameEventFlags[2]);
    ActorSetList(&field->npcs);
    ActorStageObjSystemInit();
    ActorBallSetList(field->ballList);
    field->subState = field->subState + 1;
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    charSet = MemAlloc(0xd8 /* PSP: char-set manager size; GameFieldCharSet definition is only 0xd0 */, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    charSetResult = NULL;
    if (charSet != NULL) {
      GameFieldCharSetCtor(charSet);
      charSetResult = charSet;
    }
    field->charSet = charSetResult;
    GameFieldCharSetLoadFresh(charSetResult, g_gameEventFlags[0], g_gameEventFlags[2], 0);
    ActorFindPlayer();
    break;

  case 6:
    fieldCam = (GameFieldCamera *)field->camera;
    GameFieldCameraSetTarget(fieldCam, ActorFindPlayer());
    g_gfxActiveCamera = &fieldCam->base;
    GameFieldCameraLoadQuestCam(fieldCam);
    player = ActorFindPlayer();
    if (player != NULL) {
      player->camera = fieldCam;
      /* the binary dispatches on g_scriptGlobalVars[1] (0..11) here, every case falls through */
    }
    ActorStageObjUpdateAll();
    player = ActorFindPlayer();
    slot = &((const VtblEntry *)player->base.base.vtable)[7];
    ((void (*)(void *))slot->fn)((u8 *)player + slot->delta);
    GameFieldCameraReset(fieldCam, 1, 0);
    GameFieldCameraUpdate(fieldCam);
    field->subState = field->subState + 1;
    break;

  case 7:
    ui = CoreTaskFind(0x2774);
    if (ui != NULL) {
      slot = &((const VtblEntry *)ui->base.vtable)[5];
      ((void (*)(void *, u32, u32))slot->fn)((u8 *)ui + slot->delta, 3, 4);
    }
    field->subState = field->subState + 1;
    /* fall through */
  case 8:
    if (!UiLoadingIsOpen()) {
      field->subState = field->subState + 1;
    }
    break;

  case 9:
    if (!SndHasManager()) {
      field->subState = field->subState + 1;
    } else if (SndManagerQueryGroupLoad(SndGetManager(), -1)) {
      field->subState = field->subState + 1;
    }
    break;

  case 10:
    events = (GameEvent470 *)CoreTaskCreate(0x1d6, 100);
    field->events = events;
    GameEvent470LoadScript(events, 0, 0, 0, (s32)(uintptr_t)&g_gameEventState, 0,
                           g_gameEventFlags[0], g_gameEventFlags[2]);
    field->subState = field->subState + 1;
    break;

  case 11:
    voicePac = 0x6d;
    i = 0;
    do {
      if (g_scriptGlobalVars[1] == g_gameFieldStageVoices[i][0]) {
        voicePac = g_gameFieldStageVoices[i][1];
        field->stageVoiceId = g_gameFieldStageVoices[i][2];
        break;
      }
      i++;
    } while (i < 11);
    if (SndVoicePacLoad(voicePac)) {
      field->subState = field->subState + 1;
    } else if (!SndVoicePacIsIdle()) {
      SndVoicePacRelease();
    }
    break;

  case 12:
    if (SndVoicePacIsLoaded()) {
      field->subState = field->subState + 1;
    }
    break;

  default:
    g_gfxDisplay->frameSkip = 1;
    field->phase = 1;
    field->subState = 0;
    break;
  }
}
