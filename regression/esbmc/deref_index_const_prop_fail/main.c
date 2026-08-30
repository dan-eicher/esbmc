/* Soundness twin: a GENUINELY symbolic index must still case-split —
 * the store may land on any cell, so asserting one specific cell
 * unchanged has to be refuted. */
unsigned char nondet_u1(void);
typedef union { short s; unsigned short r; } slot_t;
struct fr
{
  slot_t stack[16];
};
static struct fr g;

static void poke(struct fr *f, unsigned char i)
{
  f->stack[i].s = 7;
}

int main(void)
{
  unsigned char i = nondet_u1();
  if (i >= 16)
    return 0;
  poke(&g, i);
  __ESBMC_assert(g.stack[3].s == 0, "cell 3 may have been hit");
  return 0;
}
