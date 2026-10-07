// bdc 0x088548b0 BtlCutInTaskDraw
#include "bdc.h"

/* Draw slot (4) of the cut-in task (task id 0x1e1): opens a display-list chunk on a new render
   packet with sort key 0.5, writes the fog commands from `g_gfxFogParams` (0xcf colour low 24
   bits; 0xcd/0xce the raw bits of range and scale shifted right by 8) and the light state
   (`GfxDlWriteLightState` with the active camera, shadowed), closes the chunk, then draws the
   task's effect manager layer on that packet (`GfxSpriteLayerDrawWorld`) when it holds sprites. */
void BtlCutInTaskDraw(BtlCutInTask *task)
{
    union { float f; u32 u; } range;
    union { float f; u32 u; } scale;
    BtlArenaFog *fog;
    void *packet;
    u32 *list;

    packet = GfxNewRenderPacket(0.5f);
    list = GfxPacketBeginChunk(packet);
    fog = g_gfxFogParams;
    list[0] = (fog->color & 0xffffff) | 0xcf000000;
    range.f = fog->range;
    list[1] = (range.u >> 8) | 0xcd000000;
    scale.f = fog->scale;
    list[2] = (scale.u >> 8) | 0xce000000;
    list = GfxDlWriteLightState(list + 3, g_gfxActiveCamera, 1);
    GfxPacketEndChunk(packet, list);
    if (task->effects->base.count != 0) {
        GfxSpriteLayerDrawWorld(&task->effects->base, packet, g_gfxActiveCamera, NULL);
    }
}
