// bdc 0x089c1f04 SndObjectDetachEmitters
#include "bdc.h"

/* Starts shutting a `SndObject` down: clears `alive` (+0x24) and sets the position source (`src`,
   emitter +8) of every emitter in the object's array to NULL, so none of them keeps a pointer into
   the object or the entity it followed. The emitters continue to exist (frozen at their last
   position); the next `SndObjectUpdate` sees `alive == 0`, marks them `released` + `autoFree`,
   clears the array and reports the object deletable, after which `SndObjectMgrUpdate` runs
   `SndObjectDestroy`. */

void SndObjectDetachEmitters(SndObject *obj)
{
  int i;

  obj->alive = 0;
  for (i = 0; i < obj->emitterCount; i++) {
    SndEmitter *e = obj->emitters[i];
    if (e != NULL) {
      e->src = NULL;
    }
  }
}
