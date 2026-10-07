// bdc 0x089ddf4c GmoMotionUpdateAll
#include "bdc.h"

/* Runs `GmoMotionUpdate` on a motion player with all channels enabled (mask 0xffff); called by
   `GfxModelBindGmo`. */

void GmoMotionUpdateAll(float dt, void *player)

{
  GmoMotionUpdate(dt,player,0xffff);
  return;
}

