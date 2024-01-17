/* see if the original function syntax passes with more modern standards - it
 * does
 */
#include <stdint.h>
#include <stdio.h>

#define MY_LIM 1024

/* write line contents to buffer s and return length (including \0) */
int linetostr(s, lim)
char s[];
int lim;
{
  int c, i;

  for (i = 0; i < lim - 1 && (c = getchar()) != '\n' && c != EOF; ++i)
    s[i] = c;

  if (c == '\n')
    s[i++] = '\0'; /* just replace newline and terminate string */
  else if (i > 0)
    s[++i] =
        '\0'; /* we read something and reached lim-1 or EOF: terminate string */

  return i;
}

/* from fasterthanli.me - my version returns length of dst */
int decode_utf8(src, dst)
char *src;
uint32_t *dst;
{
  int i = 0;
  int j = 0;

  while (1) {
    uint8_t c = src[i];
    if (c == '\0') {
      dst[j] = 0;
      break; /* null terminator */
    }

    uint32_t scalar;
    int len;

    if (c >> 3 == 0b11110) {
      /* 4-byte sequence */
      uint32_t b1 = (uint32_t)src[i];
      uint32_t b2 = (uint32_t)src[i + 1];
      uint32_t b3 = (uint32_t)src[i + 2];
      uint32_t b4 = (uint32_t)src[i + 3];
      uint32_t mask1 = 0b000111000000000000000000;
      uint32_t mask2 = 0b000000111111000000000000;
      uint32_t mask3 = 0b000000000000111111000000;
      uint32_t mask4 = 0b000000000000000000111111;

      scalar = ((b1 << 18) & mask1) | ((b2 << 12) & mask2) |
               ((b3 << 6) & mask3) | ((b4 << 0) & mask4);
      len = 4;
    } else if (c >> 4 == 0b1110) {
      /* 3-byte sequence */
      uint32_t b1 = (uint32_t)src[i];
      uint32_t b2 = (uint32_t)src[i + 1];
      uint32_t b3 = (uint32_t)src[i + 2];
      uint32_t mask1 = 0b000000001111000000000000;
      uint32_t mask2 = 0b000000000000111111000000;
      uint32_t mask3 = 0b000000000000000000111111;

      scalar = ((b1 << 12) & mask1) | ((b2 << 6) & mask2) | ((b3 << 0) & mask3);
      len = 3;
    } else if (c >> 5 == 0b110) {
      /* 2-byte sequence */
      uint32_t b1 = (uint32_t)src[i];
      uint32_t b2 = (uint32_t)src[i + 1];
      uint32_t mask1 = 0b000000000000011111000000;
      uint32_t mask2 = 0b000000000000000000111111;

      scalar = ((b1 << 6) & mask1) | ((b2 << 0) & mask2);
      len = 2;
    } else {
      /* 1-byte sequence */
      scalar = (uint32_t)c;
      len = 1;
    }
    dst[j++] = scalar;
    i += len;
  }

  return j;
}

int main() {
  int i, len1, len2;
  char line[MY_LIM];
  uint32_t scalars[MY_LIM];

  while ((len1 = linetostr(line, MY_LIM)) > 0) {
    printf("%4d %s\\0\n", len1, line);

    /* show decoded as utf-8 */
    len2 = decode_utf8(line, scalars);
    printf("%4d ", len2);
    for (i = 0;; i++) {
      if (scalars[i] == 0) {
        break;
      }
      printf("U+%04X ", scalars[i]);
    }
    printf("\n");

    /* show bits */
    for (i = 0; i < len1; ++i) {
      uint8_t u8 = line[i];
      if ((u8 >> 7) & 1) /* we're on some utf-8: 10......, 110, 1110, 11110 */
        printf("%08b %2x\n", u8, u8);
      else
        printf("%08b %c\n", u8, line[i]);
    }
  }

  return 0;
}
