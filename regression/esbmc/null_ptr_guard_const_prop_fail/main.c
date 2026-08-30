/* Negative twin: a pointer that may genuinely be null must NOT fold —
 * the nondet choice keeps both branches live and the wrong assertion
 * is refuted on the null path. */
unsigned short nondet_u2(void);

struct pool
{
  unsigned short n;
};
static struct pool cp;

int main(void)
{
  struct pool *p = nondet_u2() ? &cp : (struct pool *)0;
  __ESBMC_assert(p != (struct pool *)0, "may be null: must be refuted");
  return 0;
}
