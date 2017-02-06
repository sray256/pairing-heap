# pairing-heap
This repository contains an implementation of a generic pairing heap in C. A pairing heap supports the usual heap operations with [impressive theoretical and empirical performance](https://en.wikipedia.org/wiki/Pairing_heap). This implementation is written in GNU style and based on [the original pairing heap paper](https://www.cs.cmu.edu/afs/cs/user/sleator/www/papers/pairing-heaps.pdf) by Fredman et al. and [this document](https://www.cise.ufl.edu/~sahni/dsaaj/enrich/c13/pairing.htm) by Sartaj Sahni. In addition, elements of the design and API are adapted from data structures in [the Pintos kernel](http://www.scs.stanford.edu/07au-cs140/pintos/pintos_14.html).

This heap is an intrusive container. In order to link elements together, it uses an auxiliary data member embedded within each element instead of e.g. dynamically allocated nodes containing element pointers.

Here is an example of how to use a heap of `int`s:

```c
struct int_wrapper
  {
     int value;             /* Payload. */
     struct heap_elem elem; /* Embedded linking element. */
  };
  
bool int_less (const struct heap_elem *a,
               const struct heap_elem *b, void *aux)
{
    return heap_entry (a, struct int_wrapper, elem)
           < heap_entry (b, struct int_wrapper, elem);
}

int main (int argc, char *argv[])
{
  struct heap h;
  heap_init (&h, int_less);
  struct int_wrapper a, b, c;
  a.value = 1;
  b.value = -1;
  c.value = 0;
  
  heap_insert (&h, &a->elem, NULL); /* Inserts a. */
  heap_insert (&h, &b->elem, NULL); /* Inserts b. */
  heap_insert (&h, &c->elem, NULL); /* Inserts c. */
  heap_pop (&h, NULL);              /* Pops b. */
  heap_pop (&h, NULL);              /* Pops c. */
  
  /* Pops a, so that e == &a->elem. */
  struct heap_elem *e = heap_pop (&h, NULL);
  
  /* Obtains the adress of the enclosing int_wrapper from e, so that p == &a. */
  struct int_wrapper *p = heap_entry (e, struct int_wrapper, elem);
  
  return 0;
}
```
  
Note that the heap API does not know about the type `struct int_wrapper`. Instead, it deals in `struct heap_elem *`s, requiring the client to use `struct heap_elem *`s as arguments and use the macro `heap_entry()` to obtain a `struct intr_wrapper *` from a pointer to its embedded element. For more information, please read the comments in heap.c/h.
