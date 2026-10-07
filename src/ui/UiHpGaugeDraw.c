// bdc 0x0888b91c UiHpGaugeDraw
#include "bdc.h"

/* Draws every HUD HP gauge of `g_uiHpGaugeGroup` into the render packet `packet`, one chunk
   (`GfxPacketBeginChunk`/`GfxPacketEndChunk`) per gauge. The first call sets the world-matrix
   scale `g_uiHpGaugeDrawScale` to 32768 (guard `g_uiHpGaugeDrawScaleInit`). Each chunk gets
   the light state for `g_gfxActiveCamera`, the full state of `g_gfxScreenCamera`
   (`GfxCameraDlWrite`, flags -1) and fixed 2D render-state words. A gauge is drawn only when
   `updated` is set and its unit (if any) does not answer true to both virtual slots 11 (`+0x58`)
   and 17 (`+0x88`). Then:
   - the local player's gauge (`UiHpGaugeIsLocalPlayer`) is forced opaque and drawn at a fixed
     spot (x = bar width / 2 + 62, y = 232) with scale -1;
   - any other gauge: unless it is not the player's target (`UiHpGaugeIsNotPlayersTarget`), its
     anchor is projected through `g_gfxActiveCamera` and, unless the depth from
     `MathVfpuStoreC100` is <= -3, drawn at the rounded screen position (y - 12) with scale -1;
     then, when `UiHpGaugeObjectSlot6C` holds, it is drawn again in the left-hand list
     (x = 42, y = (object `drawY` - standing HP objects among the first 3) * 16 + 208) with scale
     40 and alpha forced to 1 unless `UiHpGaugeIsSourceDown` (alpha is restored after).
   Each draw uploads a world matrix (scale rows + translation) and stores the translation in
   `screenPos` (w = y) before `UiHpGaugeEmitBars`. The fixed positions are staged on the stack
   and copied as one quad (`lv.q`/`sv.q` through C000). */

/* GE world matrix upload: command `cmd`, WORLDMATRIXNUMBER 0, then 12 WORLDMATRIXDATA words (x/y/z
   of the three scale rows, then of `pos`). 0x3b000000 is the .rodata word 0x08a69048 the asm merges
   each float's top 24 bits into with byte-offset-1 `lwr`s. */
static u32 *UiHpGaugeDrawWorldMatrix(u32 *dl, u32 cmd, const float world[4][4], const float *pos)
{
  const u32 *w;
  int row, col;

  dl[0] = cmd;
  dl[1] = 0x3a000000;
  for (row = 0; row < 3; row++) {
    w = (const u32 *)world[row];
    for (col = 0; col < 3; col++) {
      dl[2 + row * 3 + col] = 0x3b000000 | (w[col] >> 8);
    }
  }
  w = (const u32 *)pos;
  for (col = 0; col < 3; col++) {
    dl[11 + col] = 0x3b000000 | (w[col] >> 8);
  }
  return dl + 14;
}

void UiHpGaugeDraw(void *packet)
{
  float world[4][4] __attribute__((aligned(16)));
  float spot[4] __attribute__((aligned(16)));
  float pos[4] __attribute__((aligned(16)));
  float projected[4] __attribute__((aligned(16)));
  UiHpGauge *gauge;
  const VtblEntry *vtbl;
  BtlBakugan *unit;
  u32 *dl;
  float savedAlpha;
  s32 standing;
  s32 visible;
  s32 i;

  if (g_uiHpGaugeDrawScaleInit == 0) {
    g_uiHpGaugeDrawScaleInit = 1;
    g_uiHpGaugeDrawScale[3] = 0.0f;
    g_uiHpGaugeDrawScale[2] = 32768.0f;
    g_uiHpGaugeDrawScale[1] = 32768.0f;
    g_uiHpGaugeDrawScale[0] = 32768.0f;
  }
  /* vmidt + vscl: rows 0..2 of the identity scaled by x/y/z of the scale vector, row 3 is
     (0, 0, 0, 1). */
  {
    s32 row, col;

    for (row = 0; row < 4; row++) {
      for (col = 0; col < 4; col++) {
        world[row][col] = (row == col) ? 1.0f : 0.0f;
      }
    }
    for (row = 0; row < 3; row++) {
      for (col = 0; col < 4; col++) {
        world[row][col] = world[row][col] * g_uiHpGaugeDrawScale[row];
      }
    }
  }

  gauge = (UiHpGauge *)g_uiHpGaugeGroup.head;
  if (gauge == NULL) {
    return;
  }
  do {
    dl = GfxPacketBeginChunk((RenderPacket *)packet);
    dl = GfxDlWriteLightState(dl, g_gfxActiveCamera, 0);
    dl = GfxCameraDlWrite(g_gfxScreenCamera, dl, 0xffffffff);
    dl[0] = 0xdf000032;
    dl[1] = 0xe0000000;
    dl[2] = 0xe1000000;
    dl[3] = 0xdcff0001;
    dl[4] = 0x17000000;
    dl[5] = 0x22000000;
    dl[6] = 0x1e000000;
    dl[7] = 0x21000001;
    dl[8] = 0xe9000000;
    dl[9] = 0xe7000000;
    dl += 10;

    standing = 0;
    for (i = 0; i < 3; i++) {
      if (ActorStageObjGetStandingTarget(i) != NULL) {
        standing++;
      }
    }

    visible = 1;
    if (gauge->unit != NULL) {
      unit = gauge->unit;
      vtbl = (const VtblEntry *)unit->base.base.vtable;
      if (((s32 (*)(void *))vtbl[11].fn)((u8 *)unit + vtbl[11].delta) != 0) {
        unit = gauge->unit;
        vtbl = (const VtblEntry *)unit->base.base.vtable;
        if (((s32 (*)(void *))vtbl[17].fn)((u8 *)unit + vtbl[17].delta) != 0) {
          visible = 0;
        }
      }
    }

    if (gauge->updated != 0 && visible == 1) {
      if (UiHpGaugeIsLocalPlayer(gauge) != 0) {
        gauge->forceOpaque = 1;
        spot[0] = UiHpGaugeGetBarWidth(gauge) * 0.5f + 62.0f;
        spot[1] = 232.0f;
        spot[2] = -1000.0f;
        spot[3] = 0.0f;
        pos[0] = spot[0];
        pos[1] = spot[1];
        pos[2] = spot[2];
        pos[3] = spot[3];
        dl = UiHpGaugeDrawWorldMatrix(dl, 0x23000000, world, pos);
        gauge->screenPos[0] = pos[0];
        gauge->screenPos[1] = pos[1];
        gauge->screenPos[2] = pos[2];
        gauge->screenPos[3] = pos[1];
        dl = UiHpGaugeEmitBars(-1.0f, gauge, dl);
      } else {
        if (UiHpGaugeIsNotPlayersTarget(gauge) == 0) {
          MathVfpuStoreC100(projected,
                            GfxCameraProjectPoint(g_gfxActiveCamera, pos, gauge->anchor));
          if (!(projected[2] <= -3.0f)) {
            pos[2] = -projected[2];
            pos[0] = (float)(s32)pos[0];
            pos[1] = (float)(s32)pos[1] - 12.0f;
            dl = UiHpGaugeDrawWorldMatrix(dl, 0x23000001, world, pos);
            gauge->screenPos[0] = pos[0];
            gauge->screenPos[1] = pos[1];
            gauge->screenPos[2] = pos[2];
            gauge->screenPos[3] = pos[1];
            dl = UiHpGaugeEmitBars(-1.0f, gauge, dl);
          }
        }
        if (UiHpGaugeObjectSlot6C(gauge) != 0) {
          spot[0] = 42.0f;
          spot[1] = (float)((gauge->object->drawY - standing) * 16 + 0xd0);
          spot[2] = -1000.0f;
          spot[3] = 0.0f;
          pos[0] = spot[0];
          pos[1] = spot[1];
          pos[2] = spot[2];
          pos[3] = spot[3];
          dl = UiHpGaugeDrawWorldMatrix(dl, 0x23000000, world, pos);
          savedAlpha = gauge->alpha;
          if (UiHpGaugeIsSourceDown(gauge) == 0) {
            gauge->alpha = 1.0f;
          }
          gauge->screenPos[0] = pos[0];
          gauge->screenPos[1] = pos[1];
          gauge->screenPos[2] = pos[2];
          gauge->screenPos[3] = pos[1];
          dl = UiHpGaugeEmitBars(40.0f, gauge, dl);
          gauge->alpha = savedAlpha;
        }
      }
    }
    gauge = (UiHpGauge *)gauge->base.next;
    GfxPacketEndChunk((RenderPacket *)packet, dl);
  } while (gauge != NULL);
}
