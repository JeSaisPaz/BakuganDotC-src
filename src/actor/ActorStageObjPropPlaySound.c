// bdc 0x088b115c ActorStageObjPropPlaySound
#include "bdc.h"

/* Plays sound `id` at the prop's model position (`SndEmitterCreateAtPos`) in battle when the
   camera task allows sound (`camera+0x558`). */

void ActorStageObjPropPlaySound(ActorStageObjProp *self, s32 id)

{
  void *camera;
  SndListener *listener;

  if (BtlCameraTaskExists() != 0) {
    camera = BDC_OR_ZERO(BtlGetCameraTask());
    if (((BtlMain *)camera)->battleStarted != 0 && SndHasListener()) {
      listener = SndGetListener();
      SndEmitterCreateAtPos(listener, id, ((self->base).base.data)->rootMatrix + 0xc, 0, 1);
    }
  }
}
