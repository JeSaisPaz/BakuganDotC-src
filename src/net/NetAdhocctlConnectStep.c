// bdc 0x089d4ad8 NetAdhocctlConnectStep
#include "bdc.h"

/* If the adhocctl state is 0 (disconnected), calls `sceNetAdhocctlConnect(group)`; returns 1 when
   started, 0 to retry (also on the busy error `0x80410b10`), or the error after posting it
   (`NetAdhocPostError`). On success refreshes the net heap stats (`NetRefreshMallocStat`). */

s32 NetAdhocctlConnectStep(NetAdhocConn *self, const char *group)
{
  int err;
  int code;
  int state;
  char name[12];

  code = 0;
  state = 0;
  err = sceNetAdhocctlGetState(&state);
  if ((err == 0) && (state == 0)) {
    code = sceNetAdhocctlConnect(group);
    if (code == 0) {
      code = 1;
    }
    else if (code == -0x7fbef4f0) {
      code = 0;
    }
    else {
      NetAdhocPostError(self,code);
    }
  }
  if (0 < code) {
    memset(name,0,9);
    memcpy(name,group,8);
    NetRefreshMallocStat();
  }
  return code;
}
