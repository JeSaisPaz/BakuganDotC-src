// bdc 0x088c35c0 GameFieldCtor
#include "bdc.h"

/* Constructor of the field/world-map scene task `GameFieldTask` (task id 500 = 0x1f4, 0x7a0
   bytes, vtable `g_gameFieldTaskVtbl`): builds the camera and guard-blind timer, restores the
   saved global script flags 6..30 when `g_gameFieldReturnKind` is 3, creates the 2D sprite layer
   and three 3D sprite layers (publishing them as `g_billboardSpriteLayer`, `g_puffSpriteLayer`,
   `g_worldSpriteLayer`, `g_worldSpriteLayerCopy`), clears the lists and state, sets the black
   clear colour, creates the hidden location sprites and returns `task`. See `GameFieldUpdate`
   (phase dispatcher over `phase`) and `GameFieldDraw`. */

#define GAME_FIELD_SPRITE_VISIBLE 1u

CoreTask *GameFieldCtor(CoreTask *task)
{
  GameFieldTask *field = (GameFieldTask *)task;
  GfxSpriteLayer *mem;
  GfxSpriteLayer *layer;
  GfxSprite *sprite;
  PadState *pad;
  s16 *record;
  bool fromLow;
  s32 i;
  float pos[4] __attribute__((aligned(16)));

  CoreTaskInit(task);
  field->base.vtable = g_gameFieldTaskVtbl;
  GameFieldCameraCtor((GameFieldCamera *)field->camera);
  field->setupGimmick = NULL;
  field->alarmLoopHandle = 0;
  field->triggerByContact = 0;
  GameFieldGuardBlindCtor(&field->guardBlind);
  field->caughtSlot = 0xff;

  /* back from a saved return location: restore global script flags 6..30 */
  if (g_gameFieldReturnKind == 3) {
    for (i = 0; i < 25; i++) {
      if (g_gameFieldSavedFlagBits[i] != 0) {
        CoreBitsetSet(i + 6, g_scriptGlobalBits);
      } else {
        CoreBitsetClear(i + 6, g_scriptGlobalBits);
      }
    }
  }

  g_gfxDisplay->frameSkip = 0;
  CollisionInitQueryShapes();
  field->screenFx = NULL;

  /* 2D sprite layer */
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(0x80, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  layer = NULL;
  if (mem != NULL) {
    GfxSpriteLayerCtor(mem, 0);
    layer = mem;
  }
  field->spriteLayer = layer;
  layer->sorted = 1;

  /* 3D layer 0: billboards / puffs */
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(0x80, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  layer = NULL;
  if (mem != NULL) {
    GfxSpriteLayerCtor(mem, 1);
    layer = mem;
  }
  field->layers[0] = layer;
  layer->sorted = 1;
  g_billboardSpriteLayer = field->layers[0];
  g_puffSpriteLayer = field->layers[0];

  /* 3D layer 1 */
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(0x80, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  layer = NULL;
  if (mem != NULL) {
    GfxSpriteLayerCtor(mem, 1);
    layer = mem;
  }
  field->layers[1] = layer;

  /* 3D layer 2: world sprites */
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(0x80, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  layer = NULL;
  if (mem != NULL) {
    GfxSpriteLayerCtor(mem, 1);
    layer = mem;
  }
  field->layers[2] = layer;
  g_worldSpriteLayer = layer;
  g_worldSpriteLayerCopy = field->layers[2];

  field->phase = 0;
  field->frameCount = 0;
  field->subState = 0;
  field->unk67c[0] = 0.0f;
  field->unk67c[1] = 0.0f;
  field->unk67c[2] = 0.0f;
  field->field3 = 0;
  field->unk6a1 = 0;
  field->nextRequest = 0;
  field->unk624 = 0;
  field->worldMapTask = NULL;
  field->field4 = 0;
  field->pauseMode = 0;
  pad = g_padState;
  field->pad = pad;
  pad->dpadEmulatesStick = 0;

  field->objList640.tail = NULL;
  field->objList640.head = NULL;
  field->objList640.count = 0;
  field->npcsTail = NULL;
  field->npcs = NULL;
  field->npcCount = 0;
  field->objList634.tail = NULL;
  field->objList634.head = NULL;
  field->objList634.count = 0;
  ((CoreObjectList *)field->ballList)->tail = NULL;
  ((CoreObjectList *)field->ballList)->head = NULL;
  ((CoreObjectList *)field->ballList)->count = 0;
  field->gimmicksTail = NULL;
  field->gimmicks = NULL;
  field->gimmickCount = 0;
  field->stageObjs.tail = NULL;
  field->stageObjs.head = NULL;
  field->stageObjs.count = 0;
  field->objList670.tail = NULL;
  field->objList670.head = NULL;
  field->objList670.count = 0;

  field->effectMgr = NULL;
  field->effectMgr2 = NULL;
  field->talkPartner = NULL;
  field->aux = NULL;
  field->events = NULL;
  field->eventViewTimer = 0;
  field->caughtEventId = 0;
  memset(field->entryPoints, 0, sizeof(field->entryPoints));
  field->returnedFlag = 0;
  field->unk6bc = -1;
  field->itemBoxOpened = 0;
  field->eventTable = NULL;

  /* black clear colour: g_colorBlack -> clearColor -> g_gfxDisplay->clearColor (VFPU quad copies) */
  field->clearColor = g_colorBlack;
  g_gfxDisplay->clearColor[0] = field->clearColor.x;
  g_gfxDisplay->clearColor[1] = field->clearColor.y;
  g_gfxDisplay->clearColor[2] = field->clearColor.z;
  g_gfxDisplay->clearColor[3] = field->clearColor.w;
  GfxSetMotionTimeScale(1.0f);

  if (SaveGetProfile()->data->curBakugan == 0) {
    SaveGetProfile()->data->curBakugan = 1;
  }
  /* current stage number split into story chapter / sub-stage */
  g_gameEventFlags[0] = (u8)(g_scriptGlobalVars[1] / 4);
  g_gameEventFlags[2] = (u8)(g_scriptGlobalVars[1] % 4);

  pos[2] = 0.0f;
  pos[1] = 0.0f;
  pos[0] = 0.0f;
  pos[3] = 0.0f;
  sprite = GfxSpriteLayerCreateSpriteByName(field->spriteLayer, "tuto_000", pos, false);
  field->locationLabel = sprite;
  sprite->flags &= ~GAME_FIELD_SPRITE_VISIBLE;
  record = UiLayoutGetEntry(12, 0x40);
  sprite = GameFieldCreateLayoutSprite(task, record);
  field->locationMarker = sprite;
  sprite->flags &= ~GAME_FIELD_SPRITE_VISIBLE;
  sprite = GameFieldCreateLayoutSprite(task, record);
  field->locationPulse = sprite;
  sprite->flags &= ~GAME_FIELD_SPRITE_VISIBLE;

  /* sv.q of the bank constant C720 = (0, 0, 0, 0) */
  field->playerStart[0] = 0.0f;
  field->playerStart[1] = 0.0f;
  field->playerStart[2] = 0.0f;
  field->playerStart[3] = 0.0f;
  field->pulseActive = 0;
  field->pulseTimer = 0;
  field->catchActive = 0;
  SaveProfileSetWord(SaveGetProfile(), 0x2f, 0);
  field->stageVoiceId = 0;
  field->hudOpenedOnEntry = 0;
  field->alarmTimer = 0;
  field->collidableCount = 0;
  field->alarmActive = 0;
  field->talkBlendForce = 0;
  field->talkBlendRate = 0.01f;
  return task;
}
