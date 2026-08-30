/* Negative twin: the packed cells still hold SYMBOLIC content — a
 * concrete-value assertion over one must be refuted. */
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
  f.sp = 1;
  __ESBMC_assert(f.cells[0] == 7, "a packed nondet cell is not 7");
  return 0;
}
