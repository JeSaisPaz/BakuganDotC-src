// bdc 0x088efcc8 GameEvent470LoadScript
#include "bdc.h"

/* Loads the command script of a field room into the task-470 event: stores `+0x288`, allocates the
   actor records (`GameEvent470AllocActorRecords`), applies the room setting and initialises the
   event with `cmd_f<area>_<room>.cut` (`GameEventInit`). */

void GameEvent470LoadScript(GameEvent470 *self, s32 a1, s32 a2, s32 value, s32 a4, s32 mode, u8 area, u8 room)

{
  char cmdName [128];
  
  self->loadValue = value;
  GameEvent470AllocActorRecords(self);
  GameEvent470ApplyRoomSetting();
  sprintf(cmdName,"cmd_f%d_%02d.cut",area,room);
  GameEventInit(&self->base,mode,'\0',cmdName);
  return;
}

