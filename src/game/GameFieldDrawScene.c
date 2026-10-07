// bdc 0x088c3030 GameFieldDrawScene
#include "bdc.h"

/* Draw handler of the field (world map) scene task, task id 500 (`GameFieldCtor`, 0x7a0 bytes,
   vtable `0x08af2cfc`) for phases 1..4, 6 and 7 (several entries of the table `0x08a91b90`).
   Scene packet (sort key 0): a chunk with the fog commands of `g_gfxFogParams`, the light state
   and the opaque pass of `objList640` (two-pass) and the stage objects; mesh list 1 and sprite
   layer `layers[2]` with sprite fog on and a camera fog base of 4000 / |target - eye| clamped to
   10..100; a second chunk with the NPCs (particles), `ballList`, `objList634`, the gimmicks and
   `objList670`, then the translucent pass of every list, the camera projection with fog base 50;
   collision debug prims; sprite layers `layers[0]` and `layers[1]` (the latter without depth
   test); the effect managers `effectMgr`/`effectMgr2`; mesh list 0. Packet 1.9: the top/bottom
   vignette bands (`g_gameFieldVignetteYs`). Packet 2.0: the white flash `unk67c` and the
   framebuffer copy, mesh list 2 and the screen effects `screenFx`. Packet 3.0: the 2D sprite
   layer `spriteLayer`. */

void GameFieldDrawScene(CoreTask *task)
{
  GameFieldTask *field = (GameFieldTask *)task;
  BtlArenaFog *fog;
  GfxCamera *cam;
  void *packet;
  u32 *dl;
  void *stageObjs;
  float diff[3];
  float dist;
  float fogBase;
  float step;
  float rgba[4] __attribute__((aligned(16)));
  union { float f; u32 u; } bits;

  packet = GfxNewRenderPacket(0.0f);
  dl = GfxPacketBeginChunk(packet);
  fog = g_gfxFogParams;
  dl[0] = (fog->color & 0xffffff) | 0xcf000000;
  bits.f = fog->range;
  dl[1] = (bits.u >> 8) | 0xcd000000;
  bits.f = fog->scale;
  dl[2] = (bits.u >> 8) | 0xce000000;
  dl = dl + 3;
  dl = GfxDlWriteLightState(dl, g_gfxActiveCamera, 1);
  *dl = 0x19000001;
  dl = dl + 1;
  dl = GameFieldDrawModelListTwoPass(task, dl, (void **)&field->objList640);
  dl = GameFieldDrawModelListRelative(dl, (void **)g_actorStageObjList, 0);
  GfxPacketEndChunk(packet, dl);

  g_gfxSpriteFogEnable = 1;
  /* distance from the eye to the target */
  cam = g_gfxActiveCamera;
  diff[0] = cam->target[0] - cam->eye[0];
  diff[1] = cam->target[1] - cam->eye[1];
  diff[2] = cam->target[2] - cam->eye[2];
  dist = __builtin_sqrtf(diff[0] * diff[0] + diff[1] * diff[1] + diff[2] * diff[2]);
  fogBase = 4000.0f / dist;
  if (!(fogBase <= 100.0f)) {
    fogBase = 100.0f;
  }
  if (fogBase < 10.0f) {
    fogBase = 10.0f;
  }
  g_gfxActiveCamera->fogBase = fogBase;
  GfxMeshObjDrawList1(packet);
  GfxSpriteLayerDrawWorld(field->layers[2], packet, g_gfxActiveCamera, NULL);
  g_gfxActiveCamera->fogBase = 0.0f;
  g_gfxSpriteFogEnable = 0;

  dl = GfxPacketBeginChunk(packet);
  dl = GfxDlWriteLightState(dl, g_gfxActiveCamera, 1);
  *dl = 0x19000001;
  dl = dl + 1;
  dl = GameFieldDrawParticles(task, dl, (void **)&field->npcs);
  dl = GameFieldDrawModelListRelative(dl, (void **)field->ballList, 0);
  dl = GameFieldDrawModelListRelative(dl, (void **)&field->objList634, 0);
  dl = GameFieldDrawModelListRelative(dl, (void **)&field->gimmicks, 0);
  dl = GameFieldDrawModelListRelative(dl, (void **)&field->objList670, 0);
  GfxModelListDrawTranslucent(&dl, field->objList640.head, true);
  stageObjs = NULL;
  if (g_actorStageObjList != NULL) {
    stageObjs = g_actorStageObjList->head;
  }
  GfxModelListDrawTranslucent(&dl, stageObjs, true);
  GfxModelListDrawTranslucent(&dl, field->npcs, true);
  GfxModelListDrawTranslucent(&dl, ((CoreObjectList *)field->ballList)->head, true);
  GfxModelListDrawTranslucent(&dl, field->objList634.head, true);
  GfxModelListDrawTranslucent(&dl, field->gimmicks, true);
  GfxModelListDrawTranslucent(&dl, field->objList670.head, true);
  g_gfxActiveCamera->fogBase = 50.0f;
  dl = GfxCameraDlWrite(g_gfxActiveCamera, dl, 4);
  GfxPacketEndChunk(packet, dl);
  CollisionDebugPrimsDraw(packet);
  g_gfxActiveCamera->fogBase = 0.0f;

  GameFieldDrawSpriteLayerRelative(task, packet, field->layers[0]);
  g_gfxSpriteDepthTest = 0;
  GameFieldDrawSpriteLayerRelative(task, packet, field->layers[1]);
  g_gfxSpriteDepthTest = 1;
  GfxEffectMgrDrawModels(field->effectMgr, packet, g_gfxActiveCamera);
  GfxSpriteLayerDrawWorld(&field->effectMgr->base, packet, g_gfxActiveCamera, NULL);
  if (field->effectMgr2 != NULL) {
    GfxEffectMgrDrawModels(field->effectMgr2, packet, g_gfxActiveCamera);
    GfxSpriteLayerDrawWorld(&field->effectMgr2->base, packet, g_gfxActiveCamera, NULL);
  }
  GfxMeshObjDrawList0(packet);

  packet = GfxNewRenderPacket(1.9f);
  GfxPacketCall2DState(packet);
  GfxPacketDrawGradientBands(packet, &g_gameFieldVignetteYs[0], &g_gameFieldVignetteColours[0], 1, 2);
  GfxPacketDrawGradientBands(packet, &g_gameFieldVignetteYs[2], &g_gameFieldVignetteColours[2], 1, 3);

  /* white flash: unk67c[0] target level, [1] current level, [2] fall speed */
  packet = GfxNewRenderPacket(2.0f);
  if (field->unk67c[1] != 0.0f || field->unk67c[0] != 0.0f) {
    step = (field->unk67c[0] - field->unk67c[1]) * 0.2f;
    if (!(step <= 0.1f)) {
      step = 0.1f;
    } else if (step < -0.1f) {
      step = -0.1f;
    }
    if (field->unk67c[1] <= field->unk67c[0]) {
      field->unk67c[2] = 0.0f;
    } else {
      field->unk67c[2] = field->unk67c[2] - 0.002f;
      step = field->unk67c[2];
    }
    field->unk67c[1] = field->unk67c[1] + step;
    if (field->unk67c[1] < 0.0f) {
      field->unk67c[1] = 0.0f;
    }
    if (!(field->unk67c[1] <= 0.0f)) {
      rgba[0] = 1.0f;
      rgba[1] = 1.0f;
      rgba[2] = 1.0f;
      rgba[3] = field->unk67c[1] * 0.8f;
      GfxPacketDrawScreenTint(packet, rgba, 1, NULL);
    }
  }
  GfxPacketCopyFramebuffer(packet, NULL);
  GfxMeshObjDrawList2(packet);
  GameFieldScreenFxDraw(field->screenFx, packet);

  packet = GfxNewRenderPacket(3.0f);
  GfxSpriteLayerDraw(field->spriteLayer, packet);
}
