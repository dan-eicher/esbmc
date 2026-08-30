/* Element stores through a pointer, two calls deep, with loop counters
 * that are concrete per unrolled iteration. The dereference layer must
 * see the VALUED index (level-2, constants substituted) — built against
 * the unvalued level-1 name it dissects the whole enclosing object at
 * byte granularity per store, and at this size the formula explodes.
 * With valued indexes this verifies in about a second. */
typedef union { short s; unsigned short r; } slot_t;
struct fr
{
  slot_t stack[255];
  unsigned char types[255];
  slot_t locals[255];
  unsigned char ltypes[255];
  unsigned char sp;
};
struct vm
{
  struct fr frame;
  struct fr call_stack[2];
  unsigned char depth;
};
static struct vm g;

static void clr(struct vm *v)
{
  struct fr *f = &v->frame;
  for (unsigned char i = 0; i < 255; i++)
  {
    f->locals[i].s = 0;
    f->ltypes[i] = 0;
  }
}

static void enter(struct vm *v)
{
  clr(v);
}

int main(void)
{
  g.frame.sp = 1;
  enter(&g);
  __ESBMC_assert(g.frame.ltypes[7] == 0, "cleared through two calls");
  __ESBMC_assert(g.frame.sp == 1, "untouched member intact");
  return 0;
}
