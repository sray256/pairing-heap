/* Implements three simple tests covering core operations of heap. */

#include <assert.h>
#include <string.h>
#include "heap.h"

struct int_wrapper
  {
    int value;
    struct heap_elem elem;
  };

bool int_less (const struct heap_elem *a,
               const struct heap_elem *b,
               void *aux)
{
  return heap_entry (a, struct int_wrapper, elem)->value
         < heap_entry (b, struct int_wrapper, elem)->value;
}

struct str_wrapper
  {
    char *value;
    struct heap_elem elem;
  };

bool str_less (const struct heap_elem *a,
               const struct heap_elem *b,
               void *aux)
{
  char *a_str, *b_str;
  a_str = heap_entry (a, struct str_wrapper, elem)->value;
  b_str = heap_entry (b, struct str_wrapper, elem)->value;
  return strncmp (a_str, b_str, 16) < 0;
}

struct pair
  {
    int a, b; 
    struct heap_elem elem;
  }; 

bool pair_less (const struct heap_elem *a,
                const struct heap_elem *b,
                void *aux)
{
  struct pair *x, *y;
  x = heap_entry (a, struct pair, elem);
  y = heap_entry (b, struct pair, elem);
  return x->a + x->b < y->a + y->b; 
}

void
test_int (void)
{
  struct heap h;
  heap_init (&h, int_less);

  assert (heap_size (&h) == 0);
  assert (heap_empty (&h));
  assert (heap_peek (&h) == NULL);

  struct int_wrapper a, b, c, d;
  a.value = 1;
  b.value = 2;
  c.value = 3;
  d.value = 3;

  assert (heap_entry (&a.elem, struct int_wrapper, elem) == &a);

  heap_insert (&h, &a.elem, NULL);

  assert (heap_size (&h) == 1);
  assert (!heap_empty (&h));
  assert (heap_peek (&h) == &a.elem);
  assert (heap_pop (&h, NULL) == &a.elem);
  assert (heap_size (&h) == 0);
  assert (heap_empty (&h));
  assert (heap_peek (&h) == NULL);

  heap_insert (&h, &c.elem, NULL);
  heap_insert (&h, &d.elem, NULL);

  assert (heap_size (&h) == 2);

  heap_insert (&h, &b.elem, NULL);

  assert (heap_size (&h) == 3);
  assert (heap_peek (&h) == &b.elem);

  heap_insert (&h, &a.elem, NULL);

  assert (heap_size (&h) == 4);
  assert (heap_peek (&h) == &a.elem);
  assert (heap_pop (&h, NULL) == &a.elem);
  assert (heap_size (&h) == 3);
  assert (heap_pop (&h, NULL) == &b.elem);
  assert (heap_size (&h) == 2);
  
  heap_pop (&h, NULL);
  heap_pop (&h, NULL);

  assert (heap_size (&h) == 0);
  assert (heap_empty (&h));
}

void
test_str (void)
{
  struct heap h;
  heap_init (&h, str_less);

  assert (heap_size (&h) == 0);
  assert (heap_empty (&h));
  assert (heap_peek (&h) == NULL);

  struct str_wrapper a, b, c, d;
  a.value = "ant";
  b.value = "ant";
  c.value = "quokka";
  d.value = "dog";

  assert (heap_entry (&a.elem, struct str_wrapper, elem) == &a);

  heap_insert (&h, &a.elem, NULL);
  heap_insert (&h, &c.elem, NULL);
  heap_insert (&h, &d.elem, NULL);
  heap_insert (&h, &b.elem, NULL);

  assert (heap_size (&h) == 4);
  assert (!heap_empty (&h));

  heap_pop (&h, NULL);
  heap_pop (&h, NULL);

  assert (heap_pop (&h, NULL) == &d.elem);
  assert (heap_pop (&h, NULL) == &c.elem);
  assert (heap_size (&h) == 0);
  assert (heap_empty (&h));

  heap_entry (&a.elem, struct str_wrapper, elem);
}

void
test_pair (void)
{
  struct heap h;
  heap_init (&h, pair_less);

  assert (heap_size (&h) == 0);
  assert (heap_empty (&h));
  assert (heap_peek (&h) == NULL);

  struct pair x, y, z;
  x = (struct pair) { 1, 2 };
  y = (struct pair) { -5, 3 };
  z = (struct pair) { 21, 0 };

  assert (heap_entry (&x.elem, struct pair, elem) == &x);

  heap_insert (&h, &x.elem, NULL);

  assert (heap_size (&h) == 1);
  assert (!heap_empty (&h));
  assert (heap_peek (&h) == &x.elem);

  x.a = 2;
  heap_increase (&h, &x.elem, NULL);

  assert (heap_size (&h) == 1);
  assert (!heap_empty (&h));
  assert (heap_peek (&h) == &x.elem);

  x.a = 1;
  heap_decrease (&h, &x.elem, NULL);

  assert (heap_size (&h) == 1);
  assert (!heap_empty (&h));
  assert (heap_peek (&h) == &x.elem);

  heap_remove (&h, &x.elem, NULL);

  assert (heap_size (&h) == 0);
  assert (heap_empty (&h));
  assert (heap_peek (&h) == NULL);

  heap_insert (&h, &x.elem, NULL);
  heap_insert (&h, &y.elem, NULL);
  heap_insert (&h, &z.elem, NULL);

  assert (heap_size (&h) == 3);
  assert (!heap_empty (&h));
  assert (heap_peek (&h) == &y.elem);

  z.a = -3;
  heap_decrease (&h, &z.elem, NULL);

  assert (heap_peek (&h) == &z.elem);

  z.a = -2;
  heap_increase (&h, &z.elem, NULL);
  
  assert (heap_peek (&h) == &z.elem || heap_peek (&h) == &y.elem);

  y.a = 10;
  heap_increase (&h, &y.elem, NULL);
  
  assert (heap_peek (&h) == &z.elem);
}

int
main (int argc, char *argv[])
{
  test_int ();
  test_str ();
  test_pair ();
  return 0;
}
