#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#include "brutils.h"

void showusage() {
  fprintf(stderr, "usage: brp input column-index output1 output2 output3...\n");
  fprintf(stderr, "       can specify '-' for input, to read from stdin\n");
  exit(1);
}

int main(int argc, char * argv[])
{
  FILE * pin = stdin;
  int i, col_index;
  
  if (argc < 4)
    showusage();
  if (strcmp(argv[1], "-") != 0)
    pin = try_open(argv[1], "rb");
  col_index = atoi(argv[2]);
  
  int pouts_len = argc - 3;
  FILE ** pouts = (FILE **) malloc( pouts_len * sizeof(FILE *) );
  for (i = 0; i != pouts_len; ++i)
    pouts[i] = try_open(argv[i + 3], "wb");

  line_t line;
  while (fgets(line.buf, sizeof(line.buf), pin)) {
    if ( find_col(col_index, &line) ) // if this string has the requisite number of columns
      fputs(line.buf, pouts[fnv_hash(line.col_beg, line.col_end) % pouts_len]); // write it to the correct file
  }
  
  if (pin != stdin)
    fclose(pin);
    
  for (i = 0; i != pouts_len; ++i)
    fclose(pouts[i]);
  
  return 0;
}

