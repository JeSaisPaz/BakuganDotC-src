// bdc 0x089d48e0 NetAdhocBuildGroupName
#include "bdc.h"

/* Builds the adhocctl group name at `+0x34`: `"MATCH"` for the lobby mode, or in game mode (`+0x24`
   == 2) the 8-character name derived from the host MAC `+0x3c` (`NetAdhocMacToGroupName`; the
   `"GAME"` written first is overwritten). */

void NetAdhocBuildGroupName(NetAdhocConn *self)

{
  char *dst;
  
  dst = self->groupName;
  if (self->mode == 2) {
    strcpy(dst,"GAME");
    NetAdhocMacToGroupName(self,dst,self->hostMac);
  }
  else {
    strcpy(dst,"MATCH");
  }
  return;
}

