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

  int lines_len = argc - 3;
  line_t ** lines = (line_t **) malloc( lines_len * sizeof(line_t *) );
  line_t ** lines_end = lines;
  for (i = 0; i != lines_len; ++i) {
    *lines_end = (line_t *) malloc( sizeof(line_t) );
    (*lines_end)->pin = try_open(argv[i + 3], "rb");
    if (read_parse(col_index, *lines_end)) {
      ++lines_end;
      lower_bound_move(lines, lines_end);
    }
    else {
      fclose((*lines_end)->pin);
      free(*lines_end);
    }
  }

  // okay, merge!
  line_t * back;
  while (lines != lines_end) {
    // write to out
    back = *(lines_end - 1);
    *back->col_end = back->col_end_val;
    fputs(back->buf, pout);
    if (read_parse(col_index, back))
      lower_bound_move(lines, lines_end);
    else {
      fclose(back->pin);
      --lines_end;
    }
  }

  if (pout != stdout)
    fclose(pout);

  return 0;
}
