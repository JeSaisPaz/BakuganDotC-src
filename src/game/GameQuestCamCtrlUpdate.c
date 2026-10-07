// bdc 0x088fa67c GameQuestCamCtrlUpdate
#include "bdc.h"

/* Update (vtable `0x08af453c` slot `+0x14`) of the quest-field camera controller (field camera
   `+0x5c4`, see `GameFieldCameraReset`; tables from
   `GameQuestParsePathTable`/`GameQuestParseCamTable`): runs `GameQuestCamCtrlStep` (forwarding `dt`); when the
   nearest node or the current camera set (table `0x08abfc50`, `+0xc`) changed since the last frame,
   reselects the camera set (`GameQuestCamCtrlSelectCamSet`) and the mode
   (`GameQuestCamCtrlSwitchMode`) and records the new node/set (`+0x48`, `+0x40`). */

void GameQuestCamCtrlUpdate(float dt,GameQuestCamCtrl *self)

{
  short node;
  GameQuestCamPtrVec *camSet;
  
  GameQuestCamCtrlStep(dt,self);
  node = *self->nodeCursor;
  camSet = g_questCamTable->cur;
  if ((self->node != node) || (self->camSet != camSet)) {
    GameQuestCamCtrlSelectCamSet(self,node);
    GameQuestCamCtrlSwitchMode(self,node);
    self->camSet = camSet;
    self->node = *self->nodeCursor;
  }
  return;
}

