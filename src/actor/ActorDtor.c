// bdc 0x088dc9f0 ActorDtor
#include "bdc.h"

/* Destructor of the base actor class (`ActorCtor`, vtable `g_actorVtbl` slot 1): reinstalls the
   actor vtable, destroys the ground shadow `+0x16c` (`BtlShadowDtor(obj, 3)`), frees the motion-slot
   table `+0x150`, destroys the two colliders `+0x170`/`+0x174` and the input helper `+0x164` through
   their virtual destructors (vtable entry 1, flags 3), releases the model's sound object
   (`GfxModelReleaseSound`) and runs `GfxModelDtor`; frees `self` when `flags & 1`. Does nothing
   for a NULL `self`. Called by the player and NPC destructors. */

typedef void (*ActorSubDtorFn)(void *obj, u32 flags);

void ActorDtor(Actor *self, u32 flags)

{
  BtlShadow *shadow;
  s16 *slots;
  CollisionCollider *collider;
  BtlInput *input;
  const VtblEntry *entry;

  if (self != (Actor *)0x0) {
    shadow = (BtlShadow *)self->shadow;
    (self->base).base.vtable = &g_actorVtbl;
    if (shadow != (BtlShadow *)0x0) {
      BtlShadowDtor(shadow,3);
      self->shadow = (void *)0x0;
    }
    slots = self->motionSlots;
    if (slots != (s16 *)0x0) {
      MemLock();
      MemFree(slots,(char *)0x0,0);
      MemUnlock();
      self->motionSlots = (s16 *)0x0;
    }
    collider = (CollisionCollider *)self->bodyCollider;
    if (collider != (CollisionCollider *)0x0) {
      entry = &((const VtblEntry *)collider->node.vtable)[1];
      ((ActorSubDtorFn)entry->fn)((void *)((uintptr_t)collider + entry->delta),3);
      self->bodyCollider = (void *)0x0;
    }
    collider = (CollisionCollider *)self->collider2;
    if (collider != (CollisionCollider *)0x0) {
      entry = &((const VtblEntry *)collider->node.vtable)[1];
      ((ActorSubDtorFn)entry->fn)((void *)((uintptr_t)collider + entry->delta),3);
      self->collider2 = (void *)0x0;
    }
    input = (BtlInput *)self->input;
    if (input != (BtlInput *)0x0) {
      entry = &input->vtbl[1];
      ((ActorSubDtorFn)entry->fn)((void *)((uintptr_t)input + entry->delta),3);
      self->input = (void *)0x0;
    }
    GfxModelReleaseSound(&self->base);
    GfxModelDtor(&self->base,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}
