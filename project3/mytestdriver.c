#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "mymalloc.h"

#define MALLOC my_worstfit_malloc
#define FREE my_free

void test1(){
	void* a = MALLOC(100);
	void* b = MALLOC(200);
	void* c = MALLOC(150);
	void* d = MALLOC(75);
	void* e = MALLOC(1);
	printf("_brk after test 1 malloc: %p\n", sbrk(0));

	FREE(e);
	printf("_brk after test 1 free (var e - %p): %p\n", e, sbrk(0));
	FREE(c);
	printf("_brk after test 1 free (var c - %p): %p\n", c, sbrk(0));

	void* f = MALLOC(100);
	printf("_brk after test 1 malloc (var f - %p): %p\n", f, sbrk(0));
	void* g = MALLOC(25);
	printf("_brk after test 1 malloc (var g - %p): %p\n", g, sbrk(0));

	FREE(g);
	FREE(f);
	FREE(d);
	FREE(b);
	FREE(a);
	printf("_brk after freeing all of test 1: %p\n", sbrk(0));

	void* h = MALLOC(500);
	printf("_brk after mallocing 500 new bytes after resetting the linked list: %p\n", sbrk(0));

	FREE(h);
	printf("_brk final for test 1: %p\n", sbrk(0));
}

void test2(){
	void* a = MALLOC(5);
	void* b = MALLOC(100);
	void* c = MALLOC(50);
	printf("_brk after allocating 155 bytes: %p\n", sbrk(0));

	FREE(b);
	FREE(c);
	printf("_brk after freeing 150 bytes: %p\n", sbrk(0));

	void* d = MALLOC(500);
	printf("_brk after allocating 500 bytes: %p\n", sbrk(0));

	FREE(a);
	FREE(d);
	printf("_brk final for test 2: %p\n", sbrk(0));
}

int main(){
	test1();
	printf("------------------\n");
	test2();
	return 0;
}
