#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#include "brutils.h"

void showusage() {
  fprintf(stderr, "usage: brm output column-index input1 input2 input3...\n");
  fprintf(stderr, "       can specify '-' for output, to write to stdout\n");
  exit(1);
}

int main(int argc, char * argv[])
{
  FILE * pout = stdout;
  int i, col_index;
  
  if (argc < 4)
    showusage();
  if (strcmp(argv[1], "-") != 0)
    pout = try_open(argv[1], "wb");
  col_index = atoi(argv[2]);
  
  int pins_len = argc - 3;
  FILE ** pins = (FILE **) malloc( pins_len * sizeof(FILE *) );
  for (i = 0; i != pins_len; ++i)
    pins[i] = try_open(argv[i + 3], "rb");
  
  line_t * plines = (line_t *) malloc( pins_len * sizeof( line_t ) );
  
  char buf[8192];
  const char * col_beg, * col_end;
  while (fgets(buf, sizeof(buf), pin)) {
    // find the correct column
    col_beg = buf;
    find_col(col_index, &col_beg, &col_end);
    // write it to the correct file
    fputs(buf, pouts[fnv_hash(col_beg, col_end) % pouts_len]);
  }
  
  if (pin != stdin)
    fclose(pin);
  
  for (i = 0; i != pouts_len; ++i)
    fclose(pouts[i]);
  
  return 0;
}
