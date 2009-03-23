#ifndef __BR_UTILS_H__
#define __BR_UTILS_H__

#include <stdio.h>
#include <string.h>

FILE * try_open(const char * path, const char * flags) {
  FILE * p = fopen(path, flags);
  if (!p) {
    fprintf(stderr, "could not open %s: %s\n", path, strerror(errno));
    exit(1);
  }
  return p;
}

unsigned int fnv_hash(const char *p, const char *end) {
  unsigned int h = 2166136261UL;
  for (; p != end; ++p)
    h = (h * 16777619) ^ *p;
  return h;
}

typedef struct
{
  char buf[8192];
  char * col_beg;
  char * col_end;
  char col_end_val;
  FILE * pin;
}  line_t;

int find_col(int col, line_t * line) {
  for (line->col_beg = line->buf; col != 0 && *line->col_beg != 0; ++line->col_beg) {
    if ( isspace(*line->col_beg) )
      --col;
  }
  if (*line->col_beg == 0)
    return 0;
  for (line->col_end = line->col_beg; !isspace(*line->col_end); ++line->col_end) {}
  return 1;
}

int read_parse(int col, line_t * line) {
  while (fgets(line->buf, sizeof(line->buf), line->pin)) {
    if (find_col(col, line)) {
      line->col_end_val = *line->col_end;
      *line->col_end = 0;
      return 1;
    }
  }
  return 0;
}

// move end - 1 to the proper position in beg..end
void lower_bound_move(line_t ** beg, line_t ** end)
{
  if (beg == end)
    return;

  int len = end - beg - 1;
  int half;
  line_t ** mid;

  // [ * * * * x ]
  // we need to move x to its correct position in the otherwise sorted array
  while (len > 0) {
    half = len >> 1;
    mid = beg + half;
    if ( strcoll( (*mid)->col_beg, (*(end - 1))->col_beg) > 0 ) {
      beg = mid + 1;
      len = len - half - 1;
    }
    else
      len = half;
  }

  // if beg < end - 1, we need to move beg up
  if (beg < end - 1) {
    line_t * tmp = *(end - 1);
    memmove(beg + 1, beg, (end - beg - 1) * sizeof(line_t **));
    *beg = tmp;
  }
}

#endif // __BR_UTILS_H__
