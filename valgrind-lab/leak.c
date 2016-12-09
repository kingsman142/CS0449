#include <stdio.h>
#include <stdlib.h>

typedef
   struct _Node {
      struct _Node *l;
      struct _Node *r;
   } Node;

Node* mk(void)
{
   Node *x = malloc(sizeof(Node));
   x->l = NULL;
   x->r = NULL;
   return x;
}

Node* t;

void f(void)
{
   t       = mk();   // A
   t->l    = mk();   // B
   t->r    = mk();   // C
   t->l->l = mk();   // D
   t->l->r = mk();   // E
   t->r->l = mk();   // F
   t->r->r = mk();   // G

   free(t->l->l); //Free it before setting it to NULL
   t->l->l = NULL;
   //Free all of the right child and its children before freeing the node itself
   free(t->r->l);
   free(t->r->r);
   free(t->r);
   t->r = NULL;

   //Free the rest of the tree
   free(t->l->r);
   free(t->l);
   free(t);
}

int main(void)
{
   f();
   return 0;
}

