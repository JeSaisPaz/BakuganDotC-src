// bdc 0x088fa4cc GameQuestCamCtrlSelectCamSet
#include "bdc.h"

/* Selects the camera set for path node `node` (-1: nothing) of the current quest path
   (`g_questPathSet->cur`) in the quest-field camera controller (tables from
   `GameQuestParsePathTable`/`GameQuestParseCamTable`): walks all links of the node and, for
   every link whose `fromNode` is -1 or equals the previously visited node `self->node` (when that
   is not -1), makes camera set `camSet` (unless -1) current in `g_questCamTable->cur`; the last
   matching link wins. Out-of-range indices read the zeroed scratch slots
   `g_gameQuestPathNullNode`/`g_gameQuestPathNullLink`/`g_gameQuestCamNullSet` (the inlined
   vector accessor; the link index is only checked against 0). */

void GameQuestCamCtrlSelectCamSet(GameQuestCamCtrl *self, s16 node)

{
  GameQuestPathNodeVec *path;
  GameQuestPathNode *pathNode;
  GameQuestPathLink *link;
  GameQuestCamPtrVec *camSet;
  int index;
  int i;

  if (node == -1) {
    return;
  }
  index = node;
  path = g_questPathSet->cur;
  if ((-1 < index) && (index < path->count)) {
    pathNode = path->data[index];
  }
  else {
    memset(&g_gameQuestPathNullNode, 0, 4);
    pathNode = g_gameQuestPathNullNode;
  }
  for (i = 0; i < pathNode->linkCount; i++) {
    if (-1 < i) {
      link = pathNode->links[i];
    }
    else {
      memset(&g_gameQuestPathNullLink, 0, 4);
      link = g_gameQuestPathNullLink;
    }
    if ((link->fromNode == -1) || ((self->node != -1) && (link->fromNode == self->node))) {
      if (link->camSet != -1) {
        index = link->camSet;
        if ((-1 < index) && (index < g_questCamTable->count)) {
          /* the table's elements are camera sets, see GameQuestCamTableHasCam */
          camSet = g_questCamTable->data[index];
        }
        else {
          memset(&g_gameQuestCamNullSet, 0, 4);
          camSet = g_gameQuestCamNullSet;
        }
        g_questCamTable->cur = camSet;
      }
    }
  }
  return;
}
