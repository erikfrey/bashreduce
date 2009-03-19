#ifndef __BR_UTILS_H__
#define __BR_UTILS_H__

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
}  line_t;

bool find_col(int col, line_t * pline) {
  for (pline->col_beg = pline->buf; col != 0; ++*pline->col_beg) {
    if ( isspace(*pline->col_beg) )
      --col;
    if ( *pline->col_beg == '\n' )
      return false;
  }
  for (pline->col_end = pline->col_beg; !isspace(*pline->col_end); ++*pline->col_end) {}
  return true;
}

#endif // __BR_UTILS_H__
