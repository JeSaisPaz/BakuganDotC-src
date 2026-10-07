// bdc 0x088f619c GameQuestFreePathTable
#include "bdc.h"

/* Frees a quest path set built from the `.ptlb` path table: for every section
   (`GameQuestPathNodeVec`) frees each node's links, the node's link buffer and the node,
   then the section's node buffer and the section; finally the set's section buffer, and the
   set itself when `flags` bit 0 is set (GCC deleting-destructor flags). Every element access
   goes through the inlined bounds-checked vector accessor, which falls back to a zeroed
   global slot when the index is out of range. */

static GameQuestPathNodeVec **SectionSlot(GameQuestPathSet *set, int i)
{
  if (i < 0) {
    memset(&g_gameQuestPathNullSection, 0, 4);
    return &g_gameQuestPathNullSection;
  }
  return (GameQuestPathNodeVec **)&set->data[i];
}

static GameQuestPathNode **NodeSlot(GameQuestPathNodeVec *sec, int i)
{
  if (i < 0 || i >= sec->count) {
    memset(&g_gameQuestPathNullNode, 0, 4);
    return &g_gameQuestPathNullNode;
  }
  return &sec->data[i];
}

static GameQuestPathLink **LinkSlot(GameQuestPathNode *node, int i)
{
  if (i < 0 || i >= node->linkCount) {
    memset(&g_gameQuestPathNullLink, 0, 4);
    return &g_gameQuestPathNullLink;
  }
  return &node->links[i];
}

void GameQuestFreePathTable(GameQuestPathSet *set, u32 flags)
{
  int i;
  int j;
  int k;
  GameQuestPathNodeVec *sec;
  GameQuestPathNode *node;
  GameQuestPathLink *link;
  void *buf;

  if (set == NULL) {
    return;
  }
  for (i = 0; i < set->count; i++) {
    sec = *SectionSlot(set, i);
    if (sec == NULL) {
      continue;
    }
    for (j = 0; j < sec->count; j++) {
      if (*NodeSlot(sec, j) == NULL) {
        continue;
      }
      node = *NodeSlot(sec, j);
      if (node != NULL) {
        for (k = 0; k < node->linkCount; k++) {
          if (*LinkSlot(node, k) == NULL) {
            continue;
          }
          link = *LinkSlot(node, k);
          MemLock();
          MemFree(link, NULL, 0);
          MemUnlock();
          *LinkSlot(node, k) = NULL;
        }
        buf = node->links;
        if (buf != NULL) {
          MemLock();
          MemFree(buf, NULL, 0);
          MemUnlock();
          node->links = NULL;
        }
        MemLock();
        MemFree(node, NULL, 0);
        MemUnlock();
      }
      *NodeSlot(sec, j) = NULL;
    }
    buf = sec->data;
    if (buf != NULL) {
      MemLock();
      MemFree(buf, NULL, 0);
      MemUnlock();
      sec->data = NULL;
    }
    MemLock();
    MemFree(sec, NULL, 0);
    MemUnlock();
  }
  buf = set->data;
  if (buf != NULL) {
    MemLock();
    MemFree(buf, NULL, 0);
    MemUnlock();
    set->data = NULL;
  }
  if ((flags & 1) != 0) {
    MemLock();
    MemFree(set, NULL, 0);
    MemUnlock();
  }
}
