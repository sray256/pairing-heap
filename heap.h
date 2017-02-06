#ifndef _HEAP_H
#define _HEAP_H

/* Pointer-based, intrusive, two-pass pairing heap.

   This implementation is based on the doubly-linked list used in the
   Pintos, which is in turn based on the list used in the linux
   kernel.  Like those lists, this heap does not use dynamically
   allocated nodes to link elements.  Instead, it uses a struct
   heap_elem embedded in the stored structs to link elements.  This
   approach has two important consequences:

      - The user must include one struct heap_elem member in an
        insertable element for each heap into which that element can
        be simultaneously inserted.

      - When inserting, accessing, or removing elements, the user
        should always use the same struct heap_elem for a particular
        heap.  This should be the same element used in the
        heap_less_func.

*/

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>


/* Heap element. */
struct heap_elem
  {
    struct heap_elem *prev;
    struct heap_elem *next;
    struct heap_elem *child;
  };

/* Compares the value of two heap elements A and B, given
   auxiliary data AUX.  Returns true if A is less than B, or
   false if A is greater than or equal to B.  */
typedef bool heap_less_func (const struct heap_elem *a,
                             const struct heap_elem *b,
                             void *aux);

/* Heap. */
struct heap
  {
    size_t size;
    struct heap_elem *root;
    heap_less_func *less;
  };

/* Converts pointer to heap element HEAP_ELEM into a pointer to
   the structure into which HEAP_ELEM is embedded. */
#define heap_entry(HEAP_ELEM, STRUCT, MEMBER)            \
        ((STRUCT *) ((uint8_t *) &(HEAP_ELEM)->child     \
                     - offsetof (STRUCT, MEMBER.child)))

/* Heap initialization. */
#define HEAP_INITIALIZER(NAME, LESS) { 0, NULL, (LESS) }

void heap_init (struct heap *heap, heap_less_func *less);

/* Heap insertion. */
void heap_insert (struct heap *heap, struct heap_elem *elem,
                  void *aux);

/* Heap removal. */
void heap_remove (struct heap *heap, struct heap_elem *elem,
                  void *aux);
struct heap_elem *heap_pop (struct heap *heap, void *aux);

/* Heap element update. */
void heap_decrease (struct heap *heap, struct heap_elem *elem,
                    void *aux);
void heap_increase (struct heap *heap, struct heap_elem *elem,
                    void *aux);

/* Heap properties. */
size_t heap_size (struct heap *heap);
bool heap_empty (struct heap *heap);

/* Peek. */
struct heap_elem *heap_peek (struct heap *heap);

/* Merge. */
void heap_merge (struct heap *dst, struct heap *src, void *aux);

#endif /* _HEAP_H */
