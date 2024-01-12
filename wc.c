#include <stdio.h>

#define FALSE 0
#define TRUE 1

int main() {
  int c, nl, nw, nc, inword;
  nl = nw = nc = 0;
  inword = FALSE;

  while ((c = getchar()) != EOF) {
    ++nc; /* raise for every char */
    if (c == '\n')
      ++nl;
    if (c == ' ' || c == '\t' || c == '\n')
      inword = FALSE;
    else if (!inword) {
      inword = TRUE;
      ++nw; /* raise at the start of every word */
    }
  }
  printf("%7d %7d %7d\n", nl, nw, nc);

  return 0;
}
