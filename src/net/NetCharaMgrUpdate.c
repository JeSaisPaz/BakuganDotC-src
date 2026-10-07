// bdc 0x089d122c NetCharaMgrUpdate
#include "bdc.h"

/* Per-frame update of the `CONetChara` manager `g_netCharaMgr` (from the net update
   `NetPdpStep`); does nothing until the manager and its lock exist. Under the manager lock it
   runs `NetCharaUpdate` on every character and stores `charaCount`, `role1Count` (`hasRole` and
   `outHdr.handshake[0] == 1`) and `role2Count` (`matched` and `handshake[0] == 2`). Then, when
   the adhoc manager exists:
   - `mode` 1 (host) with a first (local) character: clears the local outgoing payload
     (`NetCharaSetOutData`) and for every remote character either appends its sync data
     (connected and not `recreateOnDrop`: copies `inHdr.handshake[0..1]` to `outHdr.handshake[2..3]`
     and the local `outHdr.flags`, then appends 8 bytes from its MAC and its first 0x2c header
     bytes with `NetCharaAppendOutData`), or, when not `persistent`, unlinks and destroys it
     (`NetCharaDtor(chara, 3)`) and, if `recreateOnDrop`, recreates one for the same MAC with
     `NetCharaFindOrCreate` keeping its `id`; a `persistent` one is destroyed only once
     disconnected;
   - `mode` 2: destroys every disconnected remote character.
   Finally it frees the nodes flagged removed (`NetCharaListPurgeRemoved`) and releases the lock. */

void NetCharaMgrUpdate(void)
{
  NetCharaListNode *first;
  NetCharaListNode *node;
  NetChara *chara;
  NetChara *local;
  NetAdhocConn *conn;
  int mode;
  int total;
  int role1;
  int role2;
  u8 recreate;
  s32 id;
  NetChara *created;
  u8 mac[8];

  if (g_netCharaMgr == NULL || g_netCharaMgr->lock == NULL) {
    return;
  }
  CoreLockAcquire(g_netCharaMgr->lock);
  first = NetCharaListFirst(g_netCharaMgr->list);
  total = 0;
  role1 = 0;
  role2 = 0;
  for (node = first; node != NULL; node = node->next) {
    chara = node->chara;
    if (chara == NULL) {
      continue;
    }
    NetCharaUpdate(chara);
    total++;
    if (chara->hasRole && chara->outHdr.handshake[0] == 1) {
      role1++;
    }
    if (chara->matched && chara->outHdr.handshake[0] == 2) {
      role2++;
    }
  }
  g_netCharaMgr->charaCount = total;
  g_netCharaMgr->role1Count = role1;
  g_netCharaMgr->role2Count = role2;

  if (NetAdhocHasManager()) {
    conn = (NetAdhocConn *)NetAdhocGetManager();
    mode = conn->mode;
    if (mode < 2) {
      if (mode > 0 && first != NULL && (local = first->chara) != NULL) {
        NetCharaSetOutData(local, NULL, 0);
        node = first->next;
        while (node != NULL) {
          chara = node->chara;
          node = node->next;
          if (chara == NULL) {
            continue;
          }
          if (NetCharaIsConnected(chara) && !chara->recreateOnDrop) {
            chara->outHdr.handshake[2] = chara->inHdr.handshake[0];
            chara->outHdr.handshake[3] = chara->inHdr.handshake[1];
            chara->outHdr.flags = local->outHdr.flags;
            NetCharaAppendOutData(local, chara->outHdr.mac, 8);
            NetCharaAppendOutData(local, &chara->outHdr, 0x2c);
          }
          else if (chara->persistent == 0) {
            recreate = chara->recreateOnDrop;
            id = chara->id;
            if (recreate) {
              memcpy(mac, chara->outHdr.mac, 6);
            }
            NetCharaListRemove((CoreList *)g_netCharaMgr->list, chara);
            NetCharaDtor(chara, 3);
            if (recreate) {
              /* the binary also passes "c:/bullets/bkn2pspsys/src/pspsys/sys/Net/CONetChara.cpp",
                 0x224, which the release build ignores */
              created = NetCharaFindOrCreate(mac);
              created->id = id;
            }
          }
          else if (!NetCharaIsConnected(chara)) {
            NetCharaListRemove((CoreList *)g_netCharaMgr->list, chara);
            NetCharaDtor(chara, 3);
          }
        }
      }
    }
    else if (mode < 3 && first != NULL) {
      for (node = first->next; node != NULL; ) {
        chara = node->chara;
        node = node->next;
        if (chara != NULL && !NetCharaIsConnected(chara)) {
          NetCharaListRemove((CoreList *)g_netCharaMgr->list, chara);
          NetCharaDtor(chara, 3);
        }
      }
    }
  }
  NetCharaListPurgeRemoved(g_netCharaMgr->list);
  CoreLockRelease(g_netCharaMgr->lock);
}
