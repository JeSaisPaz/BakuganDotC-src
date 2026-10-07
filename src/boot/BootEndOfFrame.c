// bdc 0x089ce6ac BootEndOfFrame
#include "bdc.h"

/* End-of-frame step run by `BootMainThread` (and `PadInitAndPoll`): finishes the pad setup once
   (`PadApplyLanguageMode`, result kept in `pad->flag58`), polls the controller with a blocking
   `PadRead`(pad, 1), ticks the player profile's play-time clock (`SaveProfileTickPlaytime`)
   and, if the 3D-sound listener exists (`SndHasListener`), updates the positional sound system:
   sets the listener heading from the active camera's `yaw` (`SndListenerSet`),
   updates the sound objects (`SndObjectMgrUpdate`) and then every positional emitter
   (`SndEmitterUpdateAll`). */

void BootEndOfFrame(PadState *pad)
{
  GfxCamera *cam;

  if (pad->flag58 == 0) {
    pad->flag58 = PadApplyLanguageMode(pad) != 0;
  }
  PadRead(pad, 1);
  if (SaveHasProfile()) {
    SaveProfileTickPlaytime(SaveGetProfile());
  }
  if (SndHasListener()) {
    cam = g_gfxActiveCamera;
    if (cam != (GfxCamera *)0x0) {
      SndListenerSet(cam->yaw, SndGetListener(), (float *)0x0, (float *)0x0);
    }
    if (SndHasObjectMgr()) {
      SndObjectMgrUpdate(SndGetObjectList());
    }
    SndEmitterUpdateAll(SndGetListener());
  }
}
