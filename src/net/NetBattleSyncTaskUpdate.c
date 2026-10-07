// bdc 0x0881cf50 NetBattleSyncTaskUpdate
#include "bdc.h"

/* Update of the netplay battle-settings sync task (id 2001): drives the `NetPlayGetManager`
   session through its handshake steps (`NetPlaySetFlags` with 0x8000000, 0x9000000, 0x4000000), then
   exchanges profile words 3-6 (battle settings) and the stage number (`g_scriptGlobalVars` entry 1)
   with the peer (`NetPlayStartExchange` send, `NetPlayGetExchangeResult` receive) and stores the
   received values. In netplay it counts frames: after 150 it shows the 'communicating' overlay
   (`NetStatusSetMessage`), after 600 it aborts the ad-hoc session (`NetAdhocBeginStop`), sets
   profile flag 0x80 and ends. Removes itself (hiding the overlay) when the profile or NetPlay
   manager disappears or when step 5 is reached. */

void NetBattleSyncTaskUpdate(NetBattleSyncTask *self)
{
  u32 slotRec0[10];    /* sp+0x00: read and discarded */
  u32 slotRec1[10];    /* sp+0x28: read and discarded */
  u32 slotRec2[10];    /* sp+0x50: read and discarded */
  u8 entry[5];         /* sp+0x78: local entry sent to the peer */
  u8 result[5];        /* sp+0x7d: exchange result */
  bool ok;
  s32 slot;
  s32 i;
  NetChara *chara;

  if (SaveGetProfileFlag0()) {
    self->timeout++;
    if (self->timeout > 150) {
      NetStatusSetMessage(1, 0);
    }
    if (self->timeout > 600) {
      ok = 1;
      if (NetAdhocHasManager()) {
        if (!NetAdhocBeginStop((NetAdhocConn *)NetAdhocGetManager())) {
          ok = 0;
        }
      }
      if (ok) {
        SaveProfileSetFlags((SaveProfile *)SaveGetProfile(), 0x80);
        NetCharaResetAllSync();
        if (NetPlayHasManager()) {
          NetPlaySetFlags((NetPlay *)NetPlayGetManager(), 0);
        }
        self->step = 999;
      }
    }
  }
  if (!SaveHasProfile()) {
    self->step = 999;
  }
  if (!NetPlayHasManager()) {
    self->step = 999;
  }

  switch ((u32)self->step) {
  case 0:
    NetCharaResetAllSync();
    NetPlaySetFlags((NetPlay *)NetPlayGetManager(), 0x8000000);
    NetModeFlagSet();
    self->step++;
    break;

  case 1:
    if (NetSyncStateIs2() && NetPlayIsSynced((NetPlay *)NetPlayGetManager())) {
      NetPlaySetFlags((NetPlay *)NetPlayGetManager(), 0x9000000);
      self->step++;
      chara = NetCharaGetByIndex(0);
      if (chara != NULL && NetCharaGetReadyFrames(chara) > 5) {
        NetCharaReadSlot(chara, 0, slotRec0);
      }
    }
    break;

  case 2:
    if (NetSyncStateIs2() && NetPlayIsSynced((NetPlay *)NetPlayGetManager())) {
      chara = NetCharaGetByIndex(0);
      if (chara != NULL) {
        NetCharaReadSlot(chara, 0, slotRec1);
      }
      if (NetCharaIsSyncHandshakeDone()) {
        NetCharaResetAllSync();
        NetPlaySetFlags((NetPlay *)NetPlayGetManager(), 0x4000000);
        self->step++;
      }
      if (self->step != 2) {
        chara = NetCharaGetByIndex(0);
        if (chara != NULL && NetCharaGetReadyFrames(chara) > 5) {
          NetCharaReadSlot(chara, 0, slotRec2);
        }
      }
    }
    break;

  case 3:
    memset(entry, 0, 5);
    slot = NetPlayGetLocalSlot((NetPlay *)NetPlayGetManager());
    if (slot >= 0) {
      if (slot == 0) {
        entry[0] = (u8)SaveProfileGetWord((SaveProfile *)SaveGetProfile(), 3);
        entry[1] = (u8)SaveProfileGetWord((SaveProfile *)SaveGetProfile(), 4);
        entry[2] = (u8)SaveProfileGetWord((SaveProfile *)SaveGetProfile(), 5);
        entry[3] = (u8)SaveProfileGetWord((SaveProfile *)SaveGetProfile(), 6);
      }
      entry[slot] = (u8)SaveProfileGetWord((SaveProfile *)SaveGetProfile(), slot + 3);
      entry[4] = (u8)g_scriptGlobalVars[1];
      if (NetPlayStartExchange((NetPlay *)NetPlayGetManager(), entry)) {
        self->step++;
      }
    }
    break;

  case 4:
    if (NetPlayGetExchangeResult((NetPlay *)NetPlayGetManager(), result)) {
      for (i = 0; i < 4; i++) {
        SaveProfileSetWord((SaveProfile *)SaveGetProfile(), i + 3, (s32)(s8)result[i]);
      }
      g_scriptGlobalVars[1] = result[4];
      self->step++;
    }
    break;

  default:
    NetStatusSetMessage(0, 0);
    CoreTaskRemove(&self->base, true);
    break;
  }
}
