#include <stdio.h>

/* return <0 if s < t, 0 if s == t, >0 if s > t */
int mystrcmp(char *s, char *t) {
  for (; *s == *t; s++, t++)
    if (*s == '\0')
      return 0;

  return *s - *t;
}

int main(int argc, char **argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage: strcmp STRING1 STRING2\n");
    return 1;
  }
  /* display result (addressing values in argv pointer-style) */
  printf("%d\n", mystrcmp(*(argv + 1), *(argv + 2)));

  return 0;
}
