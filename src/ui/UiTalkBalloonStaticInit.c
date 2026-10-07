// bdc 0x088cc6d4 UiTalkBalloonStaticInit
#include "bdc.h"

/* Static initialiser (entry 17 of the static-constructor table `0x08af5bbc`) of the talk-balloon
   module: sets the three vec4 layout constants `0x08b00ef0 = {300, 64, 0, 0}`, `0x08b00f00 = {240,
   240, 0, 0}` and `0x08b00f10 = {240, 0, 0, 0}` used by `UiTalkBalloonLayout`,
   `UiTalkBalloonOpen`, `UiTalkBalloonStep` and `UiTalkBalloonCreate`. */

void UiTalkBalloonStaticInit(void)

{
  g_uiTalkBalloonVecA.x = 300.0;
  g_uiTalkBalloonVecA.z = 0.0;
  g_uiTalkBalloonVecA.y = 64.0;
  g_uiTalkBalloonVecA.w = 0.0;
  g_uiTalkBalloonVecB.x = 240.0;
  g_uiTalkBalloonVecB.y = 240.0;
  g_uiTalkBalloonVecB.z = 0.0;
  g_uiTalkBalloonVecB.w = 0.0;
  g_uiTalkBalloonVecC.x = 240.0;
  g_uiTalkBalloonVecC.y = 0.0;
  g_uiTalkBalloonVecC.z = 0.0;
  g_uiTalkBalloonVecC.w = 0.0;
  return;
}

