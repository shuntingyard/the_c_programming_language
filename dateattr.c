#include <stdio.h>

/* days in normal years and leap years */
static int days_in_month[2][13] = {
    {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}};

/* static makes external, but private to the file */
static char *month_name[13] = {"invalid month", "Jan", "Feb", "Mar", "Apr",
                               "May",           "Jun", "Jul", "Aug", "Sep",
                               "Oct",           "Nov", "Dec"};

/* addressable with 0 to 6, from Monday */
static char *day_name[7] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};

/* atoi, book version, fails to check for overflow */
int myatoi(const char *nptr) {
  int i, n;
  i = n = 0;

  int sign = 1;
  if (nptr[i] == '-') { /* check sign */
    sign = -1;
    i++;
  }

  for (; nptr[i] != '\0'; i++) {
    if ('0' <= nptr[i] && nptr[i] <= '9')
      n = 10 * n + nptr[i] - '0';
    else
      return 0; /* as glibc, musl and more, we return 0 on error */
  }

  return sign * n;
}

struct date {
  int Y;              /* year */
  int m;              /* month */
  char *mname_ptr;    /* pointer to short month name */
  int d;              /* day */
  char *dname_ptr;    /* pointer to short day name */
  int dayinyear;      /* day number */
  int weekinyear;     /* week number */
  char datelabel[32]; /* formatted as with `date +"%Y-%m-%d"` [b.c.] */
};

/* gregorian */
int is_leap(int year) {
  return (year % 4 == 0 && year % 100 != 0 || year % 400 == 0) ? 1 : 0;
}

/* for a given year and day number get the index for day_name */
int get_day_index(int year, int dayinyear) {
#define WEEKDAY_ANCHOR 2024 /* Jan 1, 2024 is a Monday */

  int day_index, days, y;

  /* TODO: needs circle detection (O(n) noticeable (seconds!) for years in the
   * 10^8 range or above) and guards against overflow */
  if (year < WEEKDAY_ANCHOR) {
    days = 6; /* starting on Sun: 6 */
    for (y = year; y < WEEKDAY_ANCHOR; y++)
      days -= (is_leap(y) ? 2 : 1); /* 2 = 366 % 7, 1 = 365 % 7 */
  } else {
    days = -1; /* subtraction from dayinyear for Mon: 0 */
    for (y = WEEKDAY_ANCHOR; y < year; y++)
      days +=
          (is_leap(y) ? 2
                      : 1); /* mod 7 arithmetics for leap year and year again */
  }
  day_index = (days + dayinyear) % 7;
  if (day_index < 0)
    day_index += 7;

  return day_index;
}

int main(int argc, char **argv) {
  /* argument check and init */
  struct date ds; /* this is allocation */
  if (argc < 4) {
    fprintf(stderr, "Usage: multidim [-]YEAR MONTH DAY as integers\n");
    return 1;
  } else {
    ds.Y = myatoi(argv[1]);
    ds.m = myatoi(argv[2]);
    ds.d = myatoi(argv[3]);
    if (ds.m < 1 || ds.m > 12) {
      fprintf(stderr, "invalid month: %s\n", argv[2]);
      return 1;
    }
    if (ds.d < 1 || ds.d > days_in_month[is_leap(ds.Y)][ds.m]) {
      fprintf(stderr, "invalid day: %s\n", argv[3]);
      return 1;
    }
  }

  /* calc date attributes */
  ds.mname_ptr = month_name[ds.m];

  int month, days = 0;
  for (month = 1; month < ds.m; month++)
    days += days_in_month[is_leap(ds.Y)][month];
  ds.dayinyear = ds.d + days;

  ds.dname_ptr = day_name[get_day_index(ds.Y, ds.dayinyear)];

  int jan_1_index = get_day_index(ds.Y, 1);
  ds.weekinyear =
      (ds.dayinyear - 1 + ((jan_1_index == 0) ? 7 : jan_1_index)) / 7;

  sprintf(ds.datelabel, "%4d-%02d-%02d %s", (ds.Y < 0) ? -ds.Y : ds.Y, ds.m,
          ds.d, (ds.Y < 0) ? "b.c." : "");

  if (argc == 4) {
    /* display results */
    printf("date attributes\n");
    printf(" ISO 8601    %s\n", (&ds)->datelabel);
    printf(" month       %s\n", ds.mname_ptr);
    printf(" day         %s\n", ds.dname_ptr);
    printf(" day number  %3d\n", ds.dayinyear);
    printf(" week number %3d\n", ds.weekinyear);
  } else
    /* fuzz */
    printf("%s %02d", ds.dname_ptr, ds.weekinyear);

  return 0;
}
