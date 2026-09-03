/* Repeated concrete walks over a static pointer-linked tree: the
 * shape that made a bytecode-verifier span index unverifiable
 * uncached. Every iteration re-reads the same l-values, so the VCC
 * count pins that resolutions are answered from the cache. */
#include <string.h>
typedef struct node
{
  const char *name;
  struct node *kids;
  int nkids;
  int val;
} node;
static node t[7] = {
  {"root", (node *)(t + 1), 2, 0},
  {"alpha", (node *)(t + 3), 2, 1},
  {"beta", (node *)(t + 5), 2, 2},
  {"gamma", 0, 0, 3},
  {"delta", 0, 0, 4},
  {"epsilon", 0, 0, 5},
  {"zeta", 0, 0, 6},
};
static const node *child(const node *n, const char *nm)
{
  for (int i = 0; i < n->nkids; i++)
    if (n->kids[i].name && strcmp(n->kids[i].name, nm) == 0)
      return &n->kids[i];
  return 0;
}
int main(void)
{
  int acc = 0;
  for (int i = 0; i < 50; i++)
  {
    const node *a = child(t, "alpha");
    const node *d = child(a, "delta");
    acc += d ? d->val : 0;
  }
  __ESBMC_assert(acc == 200, "repeated concrete walks fold");
  return 0;
}
