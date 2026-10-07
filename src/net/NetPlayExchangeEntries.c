// bdc 0x0881b760 NetPlayExchangeEntries
#include "bdc.h"

/* Lobby sub-state machine (`exchangeState`, `+0xc0` of the NetPlay object) that exchanges 5-byte
   per-player entries (`exchangeEntries[i]`, `+0xc4 + 5*i`) between the host and the guests
   through the net-character message queue. Run from `NetPlayLateUpdate` when bit `0x8000000` of
   `NetPlayGetFlags` is clear; does nothing unless the adhoc manager (`NetAdhocHasManager`) and
   the local net character (`NetCharaGetByIndex`(0)) exist. Each step builds a zeroed 0x28-byte
   `NetCharaMsg` and sends it with `NetCharaPushMessage` or receives one with
   `NetCharaReadSlot`:
   1 = send own entry (flag `0x4000000`, `sender = localSlot + 1`), then 3 on the host
       (`NetAdhocIsHost`) or 2 on a guest; stays in 1 while the queue is full;
   2 = guest: wait for a flagged record in slot 0, copy it into `exchangeEntries[0]`, build the
       result (see below), then 4;
   3 = host: read slots 1..peerCount-1 into `exchangeEntries[i]`; once at least one flagged record
       arrived build the result, then 4;
   4 = send `exchangeResult` (`sender = 0`, flags 0), then 5;
   5 = read slot 0 back into `exchangeResult`, then 6 (done);
   0, 6 and anything else = idle.
   The result is `exchangeEntries[0]` with byte i replaced by `exchangeEntries[i][i]` (each player
   contributes its own byte; the guest only does this for its own slot). */

void NetPlayExchangeEntries(NetPlay *self)
{
    NetChara *chara;
    NetCharaMsg msg;
    bool isHost;
    bool gotAny;
    s32 i;
    s32 j;

    isHost = false;
    chara = NetCharaGetByIndex(0);
    if (!NetAdhocHasManager()) {
        return;
    }
    if (NetAdhocIsHost((NetAdhocConn *)NetAdhocGetManager())) {
        isHost = true;
    }
    if (chara == NULL) {
        return;
    }
    memset(&msg, 0, sizeof(msg));

    switch (self->exchangeState) {
    case 0:
        break;

    case 1:
        msg.body.exchange.sender = (u8)(self->localSlot + 1);
        for (j = 0; j < 5; j++) {
            msg.body.exchange.entry[j] = self->exchangeEntries[self->localSlot][j];
        }
        msg.flags = 0x4000000;
        if (NetCharaPushMessage(chara, (u32 *)&msg)) {
            self->exchangeState = isHost ? 3 : 2;
        }
        break;

    case 2:
        if (!NetCharaReadSlot(chara, 0, (u32 *)&msg)) {
            break;
        }
        if ((msg.flags & 0x4000000) == 0) {
            break;
        }
        for (j = 0; j < 5; j++) {
            self->exchangeEntries[0][j] = msg.body.exchange.entry[j];
        }
        for (j = 0; j < 5; j++) {
            self->exchangeResult[j] = self->exchangeEntries[0][j];
        }
        self->exchangeResult[self->localSlot] =
            self->exchangeEntries[self->localSlot][self->localSlot];
        self->exchangeState = 4;
        break;

    case 3:
        gotAny = false;
        for (i = 1; i < self->peerCount; i++) {
            if (NetCharaReadSlot(chara, i, (u32 *)&msg) && (msg.flags & 0x4000000) != 0) {
                for (j = 0; j < 5; j++) {
                    self->exchangeEntries[i][j] = msg.body.exchange.entry[j];
                }
                gotAny = true;
            }
        }
        if (!gotAny) {
            break;
        }
        for (j = 0; j < 5; j++) {
            self->exchangeResult[j] = self->exchangeEntries[0][j];
        }
        for (i = 1; i < self->peerCount; i++) {
            self->exchangeResult[i] = self->exchangeEntries[i][i];
        }
        self->exchangeState = 4;
        break;

    case 4:
        msg.body.exchange.sender = 0;
        for (j = 0; j < 5; j++) {
            msg.body.exchange.entry[j] = self->exchangeResult[j];
        }
        if (NetCharaPushMessage(chara, (u32 *)&msg)) {
            self->exchangeState = 5;
        }
        break;

    case 5:
        if (NetCharaReadSlot(chara, 0, (u32 *)&msg)) {
            for (j = 0; j < 5; j++) {
                self->exchangeResult[j] = msg.body.exchange.entry[j];
            }
            self->exchangeState = 6;
        }
        break;

    default:
        break;
    }
}
