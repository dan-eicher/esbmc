/* Stores through a pointer with an ambiguous target must not evict an
 * unrelated pointer's cached resolution: the store's guarded
 * assignments bump the candidate objects' generations, and p's entry
 * depends on none of them. The VCC count pins the retention. */
typedef struct box
{
  int x;
} box;
static box a_box, b_box, c_box;
_Bool nondet_bool(void);
int main(void)
{
  box *p = &a_box;
  box *q = nondet_bool() ? &b_box : &c_box;
  p->x = 1;
  int acc = 0;
  for (int i = 0; i < 20; i++)
  {
    q->x = i;
    acc += p->x;
  }
  __ESBMC_assert(acc == 20 && q->x == 19, "entries survive unrelated stores");
  return 0;
}
