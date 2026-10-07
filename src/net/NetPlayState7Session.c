// bdc 0x0881cc34 NetPlayState7Session
#include "bdc.h"

/* Handler of NetPlay state 7 (`state`), entry 7 of the state table at `0x08a50970` run by
   `NetPlayUpdate` (in session): finds the local player's slot by comparing the own MAC
   (`NetAdhocGetOwnMac`, only when `NetAdhocHasManager`) with each peer's `mac` into
   `localSlot` (-1 when absent; the last match wins), stores peerCount and localSlot in profile
   words 0x12/0x13 (`SaveProfileSetWord`) when a profile exists, and switches to the abort
   state 8 (substate 0) once the ad-hoc link is gone (`NetAdhocIsDisconnected`), also setting
   profile flag 0x80 unless `abortRequested` is set. */

void NetPlayState7Session(NetPlay *self)
{
  u8 *mac;
  s32 i;
  s32 j;

  mac = NULL;
  if (NetAdhocHasManager()) {
    NetAdhocGetManager();
    mac = NetAdhocGetOwnMac();
  }
  self->localSlot = -1;
  if (mac != NULL) {
    for (i = 0; i < self->peerCount; i++) {
      NetPlayPeer *peer = &self->peers[i];
      /* 6-byte MAC compare (unrolled in the binary) */
      for (j = 0; j < 6 && peer->mac[j] == mac[j]; j++) {
      }
      if (j == 6) {
        self->localSlot = i;
      }
    }
  }
  if (SaveHasProfile()) {
    SaveProfileSetWord(SaveGetProfile(), 0x12, self->peerCount);
    SaveProfileSetWord(SaveGetProfile(), 0x13, self->localSlot);
  }
  if (NetAdhocIsDisconnected(NetAdhocGetManager())) {
    self->state = 8;
    self->substate = 0;
    if (self->abortRequested == 0 && SaveHasProfile()) {
      SaveProfileSetFlags(SaveGetProfile(), 0x80);
    }
  }
}
