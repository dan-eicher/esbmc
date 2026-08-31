/* A dereference whose index itself reads through the pointer
 * (f->stack[f->sp - 1]) reaches the reference builder with the inner
 * access unrenamed, so the offset looks dynamic and every read and
 * write is lowered to byte stitching over the WHOLE aggregate — for
 * this frame that is a copy of the full struct per access, and the
 * solver drowns. Renaming the offset resolves f->sp to its value, the
 * offset folds to a constant, and the member-wise paths apply: the
 * shuffle below folds completely, leaving 0 VCCs. */
typedef union
{
  short s;
  unsigned short r;
} slot_t;
typedef struct
{
  const unsigned char *code;
  unsigned short pc;
  slot_t stack[256];
  unsigned char stack_types[256];
  unsigned short sp;
} frame_t;
typedef struct
{
  frame_t frame;
} vm_t;

short nondet_short(void);
unsigned char nondet_uchar(void);

static vm_t vm;

static void dup_top_under(vm_t *v)
{
  frame_t *f = &v->frame;
  slot_t saved = f->stack[f->sp - 1];
  unsigned char saved_type = f->stack_types[f->sp - 1];
  for (signed char i = 4; i >= 0; i--)
  {
    f->stack[f->sp - 5 + 1 + i] = f->stack[f->sp - 5 + i];
    f->stack_types[f->sp - 5 + 1 + i] = f->stack_types[f->sp - 5 + i];
  }
  f->stack[f->sp - 5] = saved;
  f->stack_types[f->sp - 5] = saved_type;
  f->sp += 1;
}

int main(void)
{
  vm.frame.sp = 5;
  short v0 = nondet_short();
  unsigned char t0 = nondet_uchar();
  vm.frame.stack[0].s = v0;
  vm.frame.stack_types[0] = t0;
  short v1 = nondet_short();
  unsigned char t1 = nondet_uchar();
  vm.frame.stack[1].s = v1;
  vm.frame.stack_types[1] = t1;
  short v2 = nondet_short();
  unsigned char t2 = nondet_uchar();
  vm.frame.stack[2].s = v2;
  vm.frame.stack_types[2] = t2;
  short v3 = nondet_short();
  unsigned char t3 = nondet_uchar();
  vm.frame.stack[3].s = v3;
  vm.frame.stack_types[3] = t3;
  short v4 = nondet_short();
  unsigned char t4 = nondet_uchar();
  vm.frame.stack[4].s = v4;
  vm.frame.stack_types[4] = t4;
  dup_top_under(&vm);
  __ESBMC_assert(vm.frame.sp == 6, "sp");
  __ESBMC_assert(vm.frame.stack[0].s == v4, "copy landed at depth");
  __ESBMC_assert(vm.frame.stack[1].s == v1, "wrong shuffle oracle");
  __ESBMC_assert(vm.frame.stack[5].s == v4, "original on top");
  return 0;
}
