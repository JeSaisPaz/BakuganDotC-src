// bdc 0x0881b634 NetPlayStartExchange
#include "bdc.h"

/* Starts a per-player entry exchange (`NetPlayExchangeEntries`): clears flag `0x8000000`
   (`NetPlayClearFlags`) and, when the local slot is valid and no exchange is running (state
   `+0xc0` 0 or finished 6), zeroes the 4×5-byte entry table `+0xc4` and the result `+0xd8`, stores
   the local 5-byte `entry` in its slot, sets the exchange state to 1 and returns 1; otherwise
   returns 0. Used by `NetBattleSyncTaskUpdate` to trade battle settings. */

bool NetPlayStartExchange(NetPlay *self, const u8 *entry)
{
  int slot;
  int st;

  NetPlayClearFlags(self, 0x8000000);
  if (-1 < self->localSlot) {
    st = self->exchangeState;
    if (st < 1) {
      if (st < 0) {
        return 0;
      }
    } else if (st != 6) {
      return 0;
    }
    memset(self->exchangeEntries, 0, 0x14);
    memset(self->exchangeResult, 0, 5);
    slot = self->localSlot;
    self->exchangeEntries[slot][0] = entry[0];
    self->exchangeEntries[slot][1] = entry[1];
    self->exchangeEntries[slot][2] = entry[2];
    self->exchangeEntries[slot][3] = entry[3];
    self->exchangeEntries[slot][4] = entry[4];
    self->exchangeState = 1;
    return 1;
  }
  return 0;
}
