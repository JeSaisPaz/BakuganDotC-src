// bdc 0x089d10b4 NetCharaFindOwnRecvEntry
#include "bdc.h"

/* Searches the character's receive buffer (`recvBuf`, `NetCharaRecvEntry` records of 0x34 bytes,
   count `recvLen / 0x2c`) for the entry carrying the local character's MAC and returns its fields
   through the optional outputs: the four status bytes `+0xc..+0xf`, word `+8` and word `+0x30`.
   Returns 1 when found; 0 when there is no buffer, no local character or no matching entry. */

int NetCharaFindOwnRecvEntry(NetChara *self, u32 *b0, u32 *b1, u32 *b2, u32 *b3, u32 *w8, u32 *w30)
{
  NetCharaRecvEntry *entry;
  NetChara *local;
  int count;
  int i;
  int k;
  int match;

  entry = (NetCharaRecvEntry *)self->recvBuf;
  if (entry == NULL) {
    return 0;
  }
  count = self->recvLen / 0x2c; /* stride is 0x34, as compiled */
  local = NetCharaGetByIndex(0);
  if (local == NULL) {
    return 0;
  }
  for (i = 0; i < count; i++, entry++) {
    match = 1;
    for (k = 0; k < 6; k++) {
      if (entry->mac[k] != local->outHdr.mac[k]) {
        match = 0;
        break;
      }
    }
    if (match) {
      if (b0 != NULL) {
        *b0 = entry->status[0];
      }
      if (b1 != NULL) {
        *b1 = entry->status[1];
      }
      if (b2 != NULL) {
        *b2 = entry->status[2];
      }
      if (b3 != NULL) {
        *b3 = entry->status[3];
      }
      if (w8 != NULL) {
        *w8 = entry->word08;
      }
      if (w30 != NULL) {
        *w30 = entry->word30;
      }
      return 1;
    }
  }
  return 0;
}
