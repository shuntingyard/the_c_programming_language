/* reverse copy character string */

#include <stdio.h>
int main(int argc, char **argv) {
  /* arg check */
  if (argc < 2) {
    fprintf(stderr, "Usage: strcpyr STRING_TO_REVERSE\n");
    return 1;
  }

  char buf[32]; /* DANGER, I won't check overflow */
  char *dst = buf;
  char *src = argv[1];

  while (*src != '\0')
    ++src; /* make src point to termination */
  if (src > argv[1])
    --src; /* if was not empty point to 1 before string termination */

  /* let's do a style crime combining ternary operator and assignment turned
   * into condition */
  while ((*dst++ = (src >= argv[1]) ? *src-- : '\0'))
    ;

  /* display result*/
  printf("%s\n", buf);

  return 0;
}
