/* The cell-packing idiom: pieces of one nondet value stored through
 * casts of bitops — `(short)(x & 0xFFFF)`, `(short)((x >> 16) &
 * 0xFFFF)`. A pure bitvector computation over immutable leaves is an
 * immutable value; refusing it de-constants the whole containing
 * aggregate, and the concrete loop bound stored beside it never folds.
 * At --unwind 20000 this verifies in constant time only when it does. */
unsigned int nondet_u4(void);

struct frame
{
  short cells[16];
  unsigned char sp;
};
static struct frame f;

int main(void)
{
  unsigned int x = nondet_u4();
  f.cells[0] = (short)(x & 0xFFFF);
  f.cells[1] = (short)((x >> 16) & 0xFFFF);
  f.sp = 2;
  int n = 0;
  for (unsigned char i = 0; i < f.sp; i++)
    n++;
  __ESBMC_assert(n == 2, "the bound beside the packed cells folds");
  return 0;
}
