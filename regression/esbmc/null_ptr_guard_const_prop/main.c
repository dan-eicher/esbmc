/* A null check over a pointer that provably holds a real object's
 * address must fold: no object lives at address zero, so `h->cp ==
 * NULL` below is decidably false. Before the fix the comparison stayed
 * symbolic, every assignment behind the guard became conditional,
 * constant propagation stopped, and the data-independent loop unrolled
 * to the unwind bound — at --unwind 20000 this verifies in constant
 * time only when the guard folds. */
struct pool
{
  unsigned short n;
};
struct heap
{
  struct pool *cp;
};
static struct pool cp;
static struct heap h;

static int walk(struct heap *hp)
{
  if (hp->cp == (struct pool *)0)
    return -1;
  int sum = 0;
  for (unsigned short i = 0; i < hp->cp->n; i++)
    sum++;
  return sum;
}

int main(void)
{
  cp.n = 3;
  h.cp = &cp;
  __ESBMC_assert(walk(&h) == 3, "guard folded, loop ran three times");
  return 0;
}
