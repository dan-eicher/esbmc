/* A nondet value stored into one aggregate element must not stop the
 * other elements from constant-propagating: the loop bound f.sp is a
 * plain concrete store, so the copy loop below runs exactly once and
 * --unwind 2 suffices with the unwinding assertion left on. Before the
 * fix, the nondet in stack[0] (dead: overwritten on the next line) made
 * the whole struct opaque, sp never folded, and the unwinding assertion
 * was violated. */
unsigned short nondet_ushort(void);

struct frame
{
  short stack[16];
  short saved[16];
  unsigned char sp;
};
static struct frame f;

static int handler(struct frame *fr)
{
  int sum = 0;
  for (unsigned char i = 0; i < fr->sp; i++)
  {
    fr->saved[i] = fr->stack[i];
    sum += fr->saved[i];
  }
  return sum;
}

int main(void)
{
  f.stack[0] = (short)nondet_ushort(); /* dead: overwritten next */
  f.stack[0] = 3;
  f.sp = 1;
  __ESBMC_assert(handler(&f) == 3, "one iteration over the concrete slot");
  return 0;
}
