// bdc 0x088d7d30 GameGimmickJetDoorUpdateLights
#include "bdc.h"

/* Light animation of the Marucho-jet cabin door gimmick (`GameGimmickJetDoorCtor`, vtables
   `0x08af31ac`/`0x08af324c`): steps through the blink table `g_gameGimmickJetDoorBlinkTable`
   (`{s16 from, to, start, end}`, −1 ends) while active and scales the vertex colour of node
   `"menu_cabin_door03_02__BA"` between the two percentages (`GfxModelScaleAmbientColorByName`);
   inactive doors stay at 100 %. */

void GameGimmickJetDoorUpdateLights(GameGimmickJetDoor *obj)
{
  s32 frame = obj->lightFrame;
  u32 end = obj->lightEnd;
  u32 start;
  s16 range;
  float scale;

  if (obj->base.active == 0) {
    obj->lightFrom = 100;
    obj->lightTo = 100;
    start = obj->lightStart;
  }
  else {
    obj->lightFrame = obj->lightFrame + 1;
    frame = obj->lightFrame;
    if (frame != (s32)end) {
      start = obj->lightStart;
    }
    else {
      const GameGimmickJetDoorBlinkStep *step;

      obj->lightStep = obj->lightStep + 1;
      if (g_gameGimmickJetDoorBlinkTable[obj->lightStep].from == -1) {
        obj->lightFrame = 0;
        obj->lightStep = 0;
        frame = obj->lightFrame;
      }
      step = &g_gameGimmickJetDoorBlinkTable[obj->lightStep];
      obj->lightFrom = (u8)step->from;
      obj->lightTo = (u8)step->to;
      obj->lightStart = (u8)step->start;
      obj->lightEnd = (u8)step->end;
      end = obj->lightEnd;
      start = obj->lightStart;
    }
  }
  range = (s16)(obj->lightFrom - obj->lightTo);
  if (range != 0) {
    scale = (((float)(s32)(end - frame) * (float)range) / (float)(s32)(end - start)) * 0.01f;
  }
  else if (obj->lightFrom == 100) {
    scale = 1.0f;
  }
  else {
    scale = 0.0f;
  }
  obj->lightScale = scale;
  GfxModelScaleAmbientColorByName(scale, (GfxModel *)obj, "menu_cabin_door03_02__BA");
}
