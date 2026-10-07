// bdc 0x088f1ac8 GameEvent470ExecCommand
#include "bdc.h"

/* Extended-opcode dispatcher of the task-470 event class (derived from the field event base
   `GameEventCtor`, vtable `0x08af425c`, constructor `GameEvent470Ctor`; it adds 0x4c-byte
   event-actor records at `+0x284`) (vtable slot 11, `+0x5c`): a `switch` (jump table
   `0x08a99288`) on opcodes 0x68..0xb4 that calls one handler per opcode
   (`GameEventOp68Nop` ... `GameEventOpB4Nop`). Most handlers get the flag byte and the 16-bit
   argument; some get the argument (or `arg != 0`) in the flag slot. Opcodes 0x91, 0xad and
   anything outside 0x68..0xb4 do nothing. Complements the base dispatcher `GameEventExecCommand`.
   Handlers called without a flag operand receive a stale register ((op - 0x68) * 4) there;
   all of them ignore it, so the C passes `flag`. */

void GameEvent470ExecCommand(GameEvent470 *self, u8 op, u8 flag, s16 arg)
{
  switch (op) {
  case 0x68: GameEventOp68Nop(self, flag, arg); return;
  case 0x69: GameEventOp69AddActor(&self->base, flag, arg); return;
  case 0x6a: GameEventOp6ARemoveActor(&self->base, flag, arg); return;
  case 0x6b: GameEventOp6BSetActorVisible(&self->base, flag, arg != 0); return;
  case 0x6c: GameEventOp6CSetActorX(self, flag, arg); return;
  case 0x6d: GameEventOp6DSetActorY(self, flag, arg); return;
  case 0x6e: GameEventOp6ESetActorZ(self, flag, arg); return;
  case 0x6f: GameEventOp6FActorMove(self, flag, (u16)arg); return;
  case 0x70: GameEventOp70MoveActorX(self, flag, arg); return;
  case 0x71: GameEventOp71MoveActorY(self, flag, arg); return;
  case 0x72: GameEventOp72MoveActorZ(self, flag, arg); return;
  case 0x73: GameEventOp73ActorMove2(self, flag, (u16)arg); return;
  case 0x74: GameEventOp74SetActorRotX(self, flag, arg); return;
  case 0x75: GameEventOp75SetActorRotY(self, flag, arg); return;
  case 0x76: GameEventOp76SetActorRotZ(self, flag, arg); return;
  case 0x77: GameEventOp77ActorRotate(self, flag, (u16)arg); return;
  case 0x78: GameEventOp78AddActorRotX(self, flag, arg); return;
  case 0x79: GameEventOp79AddActorRotY(self, flag, arg); return;
  case 0x7a: GameEventOp7AAddActorRotZ(self, flag, arg); return;
  case 0x7b: GameEventOp7BActorRotate2(self, flag, (u16)arg); return;
  case 0x7c: GameEventOp7CFaceTalkPartner(self, flag, arg); return;
  case 0x7d: GameEventOp7DTurnBack(self, arg, arg); return;
  case 0x7e: GameEventOp7ESetMotionIndex(self, arg, arg); return;
  case 0x7f: GameEventOp7FSetMotionNoLoop(self, arg != 0, arg); return;
  case 0x80: GameEventOp80SetActorMotion(self, flag, (u8)arg); return;
  case 0x81: GameEventOp81SetActorFace(self, flag, (u8)arg); return;
  case 0x82: GameEventOp82Nop(self, flag, arg != 0); return;
  case 0x83: GameEventOp83RestartAllActors(&self->base, flag, arg); return;
  case 0x84: GameEventOp84FreezeAllActors(&self->base, flag, arg); return;
  case 0x85: GameEventOp85RestartActor(self, flag, arg); return;
  case 0x86: GameEventOp86FreezeActor(self, flag, arg); return;
  case 0x87: GameEventOp87Nop(self, flag, arg); return;
  case 0x88: GameEventOp88SetActorRoute(self, flag, (u16)arg); return;
  case 0x89: GameEventOp89Nop(self, flag, (u16)arg); return;
  case 0x8a: GameEventOp8ANop(self, flag, arg); return;
  case 0x8b: GameEventOp8BNop(self, flag, arg); return;
  case 0x8c: GameEventOp8CSetGuardConesVisible(&self->base, arg != 0, arg); return;
  case 0x8d: GameEventOp8DSetFlag(&self->base, (u16)arg, arg); return;
  case 0x8e: GameEventOp8EClearFlag(&self->base, (u16)arg, arg); return;
  case 0x8f: GameEventOp8FSetTestFlag(self, (u16)arg, arg); return;
  case 0x90: GameEventOp90IfFlagSkip(self, (u8)arg, arg); return;
  case 0x91: return; /* skip label: no action */
  case 0x92: GameEventOp92End(&self->base, flag, arg); return;
  case 0x93: GameEventOp93StartMenuTask(&self->base, (u16)arg, arg); return;
  case 0x94: GameEventOp94Nop(self, (u16)arg, arg); return;
  case 0x95: GameEventOp95RequestTransition1(&self->base, arg, arg); return;
  case 0x96: GameEventOp96SetTransitionActor(self, (u8)arg, arg); return;
  case 0x97: GameEventOp97SetTransitionParam(&self->base, arg, arg); return;
  case 0x98: GameEventOp98SetTransitionByte(&self->base, arg != 0, arg); return;
  case 0x99: GameEventOp99RequestTransition2(&self->base, flag, arg); return;
  case 0x9a: GameEventOp9ARequestTransition3(&self->base, arg != 0, arg); return;
  case 0x9b: GameEventOp9BRequestTransition4(&self->base, arg != 0, arg); return;
  case 0x9c: GameEventOp9CRequestAreaChange(&self->base, (u8)arg, arg); return;
  case 0x9d: GameEventOp9DSetRoom(&self->base, (u8)arg, arg); return;
  case 0x9e: GameEventOp9ERequestTransitionByProfile(&self->base, flag, arg); return;
  case 0x9f: GameEventOp9FSetSpawnX(&self->base, arg, arg); return;
  case 0xa0: GameEventOpA0SetSpawnY(&self->base, arg, arg); return;
  case 0xa1: GameEventOpA1SetSpawnZ(&self->base, arg, arg); return;
  case 0xa2: GameEventOpA2SetSpawnAngle(&self->base, arg, arg); return;
  case 0xa3: GameEventOpA3Nop(self, arg, arg); return;
  case 0xa4: GameEventOpA4GimmickCommand0bc7(&self->base, arg, arg); return;
  case 0xa5: GameEventOpA5StartTask176(&self->base, arg, arg); return;
  case 0xa6: GameEventOpA6Nop(self, flag, arg); return;
  case 0xa7: GameEventOpA7Nop(self, flag, arg); return;
  case 0xa8: GameEventOpA8End2(&self->base, flag, arg); return;
  case 0xa9: GameEventOpA9GimmickCommand(&self->base, arg, arg); return;
  case 0xaa: GameEventOpAANop(self, arg, arg); return;
  case 0xab: GameEventOpABNop(self, flag, arg); return;
  case 0xac: GameEventOpACDisableGimmick9(&self->base, arg, arg); return;
  case 0xad: return; /* no action */
  case 0xae: GameEventOpAESetRoomSetting(&self->base, flag, (u16)arg); return;
  case 0xaf: GameEventOpAFClearRoomSetting(&self->base, flag, arg); return;
  case 0xb0: GameEventOpB0SetFieldFlagD18(&self->base, arg != 0, arg); return;
  case 0xb1: GameEventOpB1ShowFieldIcon(&self->base, flag, (u8)arg); return;
  case 0xb2: GameEventOpB2HideFieldIcons(&self->base, flag, arg); return;
  case 0xb3: GameEventOpB3SetEventFlag2d6(self, arg != 0, arg); return;
  case 0xb4: GameEventOpB4Nop(self, flag, arg); return;
  default: return;
  }
}
