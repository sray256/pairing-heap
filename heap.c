#include "heap.h"
#include <assert.h>

/* Initializes HEAP as an empty heap which will use LESS to compare
   elements. */
void
heap_init (struct heap *heap, heap_less_func *less)
{
  heap->size = 0;
  heap->root = NULL;
  heap->less = less;
}

/* Merges heaps A and B by making the heap with the larger root key
   the child of the smaller root key.  Returns the new root. */
static struct heap_elem *
merge (struct heap_elem *a, struct heap_elem *b,
       heap_less_func *less, void *aux)
{
  if (less (a, b, aux))
    {
      if (a->child != NULL)
        a->child->prev = b;
      if (b->next != NULL)
        b->next->prev = a;
      a->next = b->next;
      b->next = a->child;
      a->child = b;
      b->prev = a;
      return a;
    }
  if (b->child != NULL)
    b->child->prev = a;
  if (a->prev != NULL && a->prev->child != a)
    a->prev->next = b;
  b->prev = a->prev;
  a->prev = b;
  a->next = b->child;
  b->child = a;
  return b;
}

/* Inserts ELEM into HEAP, passing auxiliary data AUX to the
   HEAP->less function. */
void
heap_insert (struct heap *heap, struct heap_elem *elem, void *aux)
{
  heap->size++;

  elem->prev = NULL;
  elem->next = NULL;
  elem->child = NULL;

  if (heap->root == NULL)
    heap->root = elem;
  else
    heap->root = merge (elem, heap->root, heap->less, aux);
}

/* Merges pairs of the siblings of A from left to right.  Returns the
   last result. */
static struct heap_elem *
merge_right (struct heap_elem *a, heap_less_func *less, void *aux)
{
  struct heap_elem *b;
  for (b = NULL; a != NULL; a = b->next)
    {
      if ((b = a->next) == NULL)
        return a;
      b = merge (a, b, less, aux);
    }
  return b;
}

/* Merges the siblings of A from right to left.  Returns the single
   resulting heap. */
static struct heap_elem *
merge_left (struct heap_elem *a, heap_less_func *less, void *aux)
{
  struct heap_elem *b;
  for (b = a->prev; b != NULL; b = a->prev)
    a = merge (b, a, less, aux);
  return a;
}

/* Unlinks A from its children and returns the single heap obtained
   by merging all its children. */
static struct heap_elem *
merge_subheaps (struct heap_elem *a, heap_less_func *less, void *aux)
{
  a->child->prev = NULL;

  struct heap_elem *e;
  e = merge_right (a->child, less, aux);
  e = merge_left (e, less, aux);

  return e;
}

/* Unlinks ELEM from any children, parents, or siblings. */
void
detach_subheap (struct heap_elem *elem)
{
  if (elem->prev->child == elem)
    elem->prev->child = elem->next;
  else
    elem->prev->next = elem->next;

  if (elem->next != NULL)
    elem->next->prev = elem->prev;

  elem->prev = NULL;
  elem->next = NULL;
}

/* Removes ELEM from HEAP, passing auxiliary data AUX to
   HEAP->less. */
void
heap_remove (struct heap *heap, struct heap_elem *elem, void *aux)
{
  if (elem == heap->root)
    {
      heap_pop (heap, aux);
      return;
    }
  
  assert (heap->size > 0);

  heap->size--;
  detach_subheap (elem);
  elem = merge_subheaps (elem, heap->less, aux);
  heap->root = merge (heap->root, elem, heap->less, aux);
}

/* Removes and returns the minimum element of HEAP, passing
   auxiliary data AUX to HEAP->less. */
struct heap_elem *
heap_pop (struct heap *heap, void *aux)
{
  assert (heap->size > 0);
  
  heap->size--;

  struct heap_elem *root = heap->root;
  if (root->child == NULL)
    {
      heap->root = NULL;
      return root;
    }

  heap->root = merge_subheaps (root, heap->less, aux);
  return root;
}

/* Updates HEAP to reflect the fact that ELEM, which HEAP contains,
   has decreased, passing auxiliary data AUX to HEAP->less. */
void
heap_decrease (struct heap *heap, struct heap_elem *elem, void *aux)
{
  if (elem->prev == NULL)
    return;

  detach_subheap (elem);
  heap->root = merge (heap->root, elem, heap->less, aux); 
}

/* Updates HEAP to reflect the fact that ELEM, which HEAP contains,
   has increased, passing auxiliary data AUX to HEAP->less. */
void
heap_increase (struct heap *heap, struct heap_elem *elem, void *aux)
{
  heap_remove (heap, elem, aux);
  heap_insert (heap, elem, aux);
}

/* Returns the number of elements in HEAP. */
size_t heap_size (struct heap *heap)
{
  return heap->size;
}

/* Returns true if HEAP is empty and false otherwise. */
bool
heap_empty (struct heap *heap)
{
  return heap->root == NULL;
}

/* Returns (but does not remove) the minimum element in HEAP. */
struct heap_elem *
heap_peek (struct heap *heap)
{
  return heap->root; 
}

/* Merges the heap SRC into the heap DST, after which SRC contains
   all the elements in DST and DST is empty. */
void
heap_merge (struct heap *dst, struct heap *src, void *aux)
{
  assert (dst != src);
  assert (dst->less == src->less);

  if (dst->root == NULL)
    {
      dst->root = src->root;
      return;
    }

  dst->root = merge (dst->root, src->root, dst->less, aux);
  dst->size += src->size;
}
