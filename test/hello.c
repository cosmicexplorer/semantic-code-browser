int __a() {
  return 3;
}

/*
  struct {
  int a;
  } bb = {.a = 3};
*/

#define AAAA (__a())
#define BBBB (AAAA + AAAA)

int ___a() {
  return BBBB;
}

static int A = 4;

/* #include <stdio.h> */

#define A 3

#define F(x, y) x##y

typedef int NEW_TYPE_YO;
NEW_TYPE_YO b;
int a;

#ifdef BBB
int _a() {
  int _b;
  _b = A;
  return _b;
}
#else
int _a();
int c = 3;
int _a() {
  int _b;
  _b = A;
  return _b;
}
#endif

extern int AAAAAA;
int AAAAAA = 3;
/* struct q { */
/*   int b; */
/* }; */
/* void _b() { */
/*   struct q BBBBBB = {.b = AAAAAA}; */
/* } */

int main();

#define A 3

int main(int argc, char ** argv) {
  /* int * F(a, ptr) = NULL; */
  int * F(a, ptr) = 0;
  aptr = &a;
  *aptr = A;
  b = _a();
#undef A
  int c = A;
  c = argc;
  char ** res __attribute__((unused)) = argv;
  /* printf("%d\n", a); */
}
