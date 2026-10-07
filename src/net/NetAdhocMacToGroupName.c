// bdc 0x089d47d8 NetAdhocMacToGroupName
#include "bdc.h"

/* Encodes bytes 1..5 of a MAC address (40 bits) as 8 base-32 characters (`A`-`Z`, `0`-`5`) into
   `out`, giving an 8-character adhocctl group name unique to the host. `self` is unused; `out`
   is not NUL-terminated. */

void NetAdhocMacToGroupName(NetAdhocConn *self, char *out, const u8 *mac)
{
  u8 *dst = (u8 *)out;
  s32 i;

  dst[0] = (mac[1] & 0xf8) >> 3;
  dst[1] = (mac[1] & 0x07) << 2 | (mac[2] & 0xc0) >> 6;
  dst[2] = (mac[2] & 0x3e) >> 1;
  dst[3] = (mac[2] & 0x01) << 4 | (mac[3] & 0xf0) >> 4;
  dst[4] = (mac[3] & 0x0f) << 1 | (mac[4] & 0x80) >> 7;
  dst[5] = (mac[4] & 0x7c) >> 3;
  dst[6] = (mac[4] & 0x03) << 3 | (mac[5] & 0xe0) >> 5;
  dst[7] = mac[5] & 0x1f;

  for (i = 0; i < 8; i++) {
    if (dst[i] < 26) {
      dst[i] = dst[i] + 'A';
    }
    else {
      dst[i] = dst[i] - 26;
      dst[i] = dst[i] + '0';
    }
  }
}
