// bdc 0x0880d1e8 SaveProfileInit
#include "bdc.h"

/* Creates the player profile: allocates and constructs the holder (`SaveProfileHolderCtor`) if
   `g_playerProfile` is NULL, attaches the current `g_saveDataBlock`
   (`SaveProfileAttachBlock`), then resets it to defaults (`SaveProfileReset`,
   `SaveProfileResetWordTable` with `keepFlag = 0`). Called by `ScriptVarsInit`. */

void SaveProfileInit(void)

{
  SaveProfile *holder;
  bool fromLow;
  SaveProfile *self;
  
  holder = g_playerProfile;
  if (g_playerProfile == (SaveProfile *)0x0) {
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    self = MemAlloc(sizeof(SaveProfile),(char *)0x0,0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    holder = (SaveProfile *)0x0;
    if (self != (SaveProfile *)0x0) {
      SaveProfileHolderCtor(self);
      holder = self;
    }
  }
  g_playerProfile = holder;
  SaveProfileAttachBlock(g_saveDataBlock);
  SaveProfileReset(g_playerProfile);
  SaveProfileResetWordTable(g_playerProfile,false);
  return;
}

