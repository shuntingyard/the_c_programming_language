/* reverse print string, char by char */

#include <stdio.h>
int main(int argc, char **argv) {
  /* arg check */
  if (argc < 2) {
    fprintf(stderr, "Usage: strprnr STRING_TO_REVERSE\n");
    return 1;
  }

  char *src = argv[1];

  while (*src != '\0')
    ++src; /* make src point to termination */
  if (src > argv[1])
    --src; /* if was not empty point to 1 before string termination */

  /* compare array addresses for termination */
  for (; src >= argv[1]; --src)
    printf("%c", *src); /* under the (Linux) hood all these go to a */
  printf("\n");         /* buffered writer, so we just see 1 syscall */

  return 0;
}
