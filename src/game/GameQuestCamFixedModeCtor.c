// bdc 0x088f66f4 GameQuestCamFixedModeCtor
#include "bdc.h"

/* Constructor of the fixed-camera mode of the quest-field camera system (see
   `GameQuestCamCtrlCtor`) (vtable `g_gameQuestCamFixedModeVtbl`), used for type-1 camera table entries:
   `GameQuestCamTargetCtor`, path set `desc+0x40` -> `+0x80`, mode flag `+0x70 = 1`. */

typedef struct CamFixedModeDesc {
  GameQuestCamSpringDesc spring; /* +0x00 */
  ScePspFVector4 point;          /* +0x30 */
  void *entry;                   /* +0x40 */
} __attribute__((aligned(16))) CamFixedModeDesc;

GameQuestCamFixedMode *GameQuestCamFixedModeCtor(GameQuestCamFixedMode *self, void *desc)

{
  GameQuestCamTargetCtor(&self->base,desc);
  (self->base).base.vtbl = g_gameQuestCamFixedModeVtbl;
  self->entry = ((CamFixedModeDesc *)desc)->entry;
  self->kind = 1;
  return self;
}

