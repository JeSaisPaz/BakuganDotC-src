// bdc 0x088fa2ec GameQuestCamCtrlCtor
#include "bdc.h"

/* Constructor of the quest-field camera controller (field camera `+0x5c4`, see
   `GameFieldCameraReset`; tables from `GameQuestParsePathTable`/`GameQuestParseCamTable`)
   (vtable `g_questCamCtrlVtbl` at `+0x2c`, base `GameQuestCamCtrlBaseCtor`): clears the two mode slots
   `+0x38`/`+0x3c`, copies the node cursor `+0x30`, `+0x34` and `+0x44` from `desc`, sets the
   current node `+0x48 = -1`, finds the nearest path node (`GameQuestPathFindNearestNode`),
   switches to that node's camera (`GameQuestCamCtrlSwitchMode`) and runs one snapped step
   (`GameQuestCamCtrlStep` with dt 1/60 and the snap flag `+0x4a` set). Returns `ctrl`. */

GameQuestCamCtrl *GameQuestCamCtrlCtor(GameQuestCamCtrl *self, void *desc)

{
  GameQuestCamCtrlBaseCtor(self,desc);
  self->vtbl = g_questCamCtrlVtbl;
  self->collision = (void *)0x0;
  self->mode = (GameQuestCamSpring *)0x0;
  self->look = (GameQuestCamSpring *)0x0;
  self->camSet = 0;
  self->node = -1;
  self->snap = '\0';
  self->nodeCursor = ((GameQuestCamCtrl *)desc)->nodeCursor;
  self->radius = ((GameQuestCamCtrl *)desc)->radius;
  self->collision = ((GameQuestCamCtrl *)desc)->collision;
  GameQuestPathFindNearestNode(self->nodeCursor);
  GameQuestCamCtrlSwitchMode(self,*self->nodeCursor);
  self->snap = '\x01';
  GameQuestCamCtrlStep(0.016666668f,self);
  return self;
}

