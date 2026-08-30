/* Negative twin of nondet_field_const_prop: the propagated struct
 * literal must still carry the true values — an assertion on the wrong
 * sum has to be refuted, proving the relaxed propagation did not fold
 * anything to an incorrect constant. */
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
  __ESBMC_assert(handler(&f) == 4, "wrong sum must be refuted");
  return 0;
}
