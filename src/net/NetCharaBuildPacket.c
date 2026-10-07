// bdc 0x089cfbc4 NetCharaBuildPacket
#include "bdc.h"

/* Serialises the first net character of `g_netCharaMgr` into the PDP send buffer `buf` and returns
   the byte count (0 when there is no manager or no character). In ad-hoc game mode
   (`NetAdhocConn.mode` == 2) it first builds frames (`NetCharaSyncFrames`) and refreshes the
   header (`NetCharaPrepareHeader`), takes the payload length from `outHdr.lenFlags` and sets flag
   `0x800`, then writes the 0x34-byte header and the payload. Otherwise it stores `outLen` in
   `lenFlags` with `0x800` cleared and writes the header, the 0x7c-byte NetPlay peer table (from
   peer 0 via `NetPlayGetPeer`, zeroes when there is none) and the payload (`outBuf`, `outLen`
   bytes, skipped when `outLen` <= 0). */

int NetCharaBuildPacket(void *buf)
{
    u8 *dst = (u8 *)buf;
    int len = 0;
    s32 mode;
    NetCharaListNode *node;
    NetChara *chara;
    u8 *peers;

    if (g_netCharaMgr != NULL) {
        mode = 0;
        if (NetAdhocHasManager()) {
            mode = ((NetAdhocConn *)NetAdhocGetManager())->mode;
        }
        CoreLockAcquire(g_netCharaMgr->lock);
        node = (NetCharaListNode *)NetCharaListFirst(g_netCharaMgr->list);
        if (node != NULL && (chara = node->chara) != NULL) {
            CoreLockAcquire(chara->lock);
            if (mode == 2) {
                NetCharaSyncFrames(chara);
                NetCharaPrepareHeader(chara);
                chara->outLen = (s16)chara->outHdr.lenFlags & 0x7ff;
                chara->outHdr.lenFlags = chara->outHdr.lenFlags | 0x800;
                len = 0x34;
                memcpy(dst, &chara->outHdr, 0x34);
                dst += sizeof(NetCharaPacketHeader);
            } else {
                chara->outHdr.lenFlags =
                    (u16)((chara->outHdr.lenFlags & ~0x7ff) | ((u16)chara->outLen & 0x7ff));
                chara->outHdr.lenFlags = chara->outHdr.lenFlags & ~0x800;
                memcpy(dst, &chara->outHdr, 0x34);
                dst += sizeof(NetCharaPacketHeader);
                peers = NULL;
                if (NetPlayHasManager()) {
                    peers = NetPlayGetPeer((NetPlay *)NetPlayGetManager(), 0);
                }
                if (peers != NULL) {
                    memcpy(dst, peers, 0x7c);
                } else {
                    memset(dst, 0, 0x7c);
                }
                dst += sizeof(chara->peerBlock);
                len = 0xb0;
            }
            if (chara->outLen > 0) {
                memcpy(dst, chara->outBuf, chara->outLen);
                len += chara->outLen;
            }
            CoreLockRelease(chara->lock);
        }
        CoreLockRelease(g_netCharaMgr->lock);
    }
    return len;
}
