// bdc 0x088dfd00 ActorSyncRouteGroups
#include "bdc.h"

/* Synchronises route groups (placement `routeGroup`, up to 5 groups): when every actor of a group
   stands on a route step of type 3 ("wait for group"), advances all of them to the next step
   (`routeStep`). */

typedef struct RouteGroupSlot {
  s32 id;   /* group id, -1 = free slot */
  s32 open; /* 1 while every actor seen so far waits on a type-3 step */
} RouteGroupSlot;

void ActorSyncRouteGroups(void)
{
  RouteGroupSlot groups[5];
  RouteGroupSlot *slot;
  Actor *head;
  Actor *actor;
  ActorNpcPlacement *place;
  s32 count;
  s32 group;
  s32 i;
  s32 waiting;

  count = 0;
  head = *(Actor **)g_actorList;
  for (i = 0; i < 5; i++) {
    groups[i].id = -1;
    groups[i].open = 1;
  }

  for (actor = head; actor != NULL; actor = (Actor *)actor->base.base.next) {
    place = (ActorNpcPlacement *)actor->placement;
    if (place == NULL) {
      continue;
    }
    group = place->routeGroup;
    if (group == 0) {
      continue;
    }
    slot = groups;
    for (i = 0; i < 5; i++, slot++) {
      if (slot->id == -1) {
        slot->id = group;
        count++;
      }
      if (slot->id == group) {
        if (slot->open == 1) {
          if (actor->route == NULL) {
            waiting = 0;
          } else {
            waiting = ((const s16 *)(actor->route + 1))[actor->routeStep * 4] == 3;
          }
          if (!waiting) {
            slot->open = 0;
          }
        }
        break;
      }
    }
  }

  for (actor = head; actor != NULL; actor = (Actor *)actor->base.base.next) {
    place = (ActorNpcPlacement *)actor->placement;
    if (place == NULL) {
      continue;
    }
    group = place->routeGroup;
    if (group == 0) {
      continue;
    }
    slot = groups;
    for (i = 0; i < count; i++, slot++) {
      if (slot->id == group && slot->open == 1) {
        actor->routeStep++;
        break;
      }
    }
  }
}
