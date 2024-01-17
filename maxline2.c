/* find longest line; ugly version with `extern` keyword */
#include <stdio.h>

#define MAXLINE 1000 /* maximum input line size */

/* DEFINITIONS */
char line[MAXLINE]; /* input line */
char save[MAXLINE]; /* longest line saved here */
int max;            /* length of longest line seen so far */

int mygetline();
void mycopy();

int main() {
  int len;
  extern int max; /* DECLARATIONS */
  extern char save[];

  max = 0;
  while ((len = mygetline()) > 0) {
    if (len > max) {
      max = len;
      mycopy();
    }
  }
  if (max > 0) /* there was a line */
    printf("%s", save);

  return 0;
}

int mygetline() /* specialized version */
{
  int c, i;
  extern char line[];

  for (i = 0; i < MAXLINE - 1 && (c = getchar()) != EOF && c != '\n'; ++i)
    line[i] = c;
  if (c == '\n') {
    line[i] = c;
    ++i;
  }
  line[i] = '\0';

  return i;
}

void mycopy() /* specialized version */
{
  int i;
  extern char line[], save[];

  i = 0;
  while ((save[i] = line[i]) != '\0')
    ++i;
}
