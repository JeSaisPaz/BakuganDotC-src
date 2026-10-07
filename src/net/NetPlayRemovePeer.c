// bdc 0x0881b3bc NetPlayRemovePeer
#include "bdc.h"

/* Removes from the `NetPlay` peer table the first record whose 6-byte MAC (+0x19)
   matches that of `rec`, shifting the following records down. Used by `NetCharaHandshakeStep`. */

void NetPlayRemovePeer(NetPlay *self, const u8 *rec)
{
  const NetPlayPeer *peer = (const NetPlayPeer *)rec;
  int count;
  int i;
  int j;
  bool match;

  if (rec == (const u8 *)0x0) {
    return;
  }
  count = self->peerCount;
  for (i = 0; i < count; i++) {
    match = true;
    for (j = 0; j < 6; j++) {
      if (self->peers[i].mac[j] != peer->mac[j]) {
        match = false;
        break;
      }
    }
    if (match) {
      count--;
      self->peerCount = count;
      if (count < 1) {
        return;
      }
      for (; i < self->peerCount; i++) {
        self->peers[i] = self->peers[i + 1];
      }
      return;
    }
  }
}
