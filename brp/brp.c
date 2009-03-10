#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

void showusage() {
  fprintf(stderr, "usage: brp [-i input] output1 output2 output3...\n");
  exit(1);
}

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

int main(int argc, char * argv[])
{
  FILE * pin = stdin;
  int i, num_out = 0, out_argc = 1;
  char buf[8192];
  char * delim;
  
  if (argc < 2)
    showusage();
  if (strcmp(argv[1], "-i") == 0) {
    out_argc = 3;
    if (argc < 4)
      showusage();
    pin = try_open(argv[2], "rb");
  }
  
  num_out = argc - out_argc;
  FILE ** pouts = (FILE **) malloc( num_out * sizeof(FILE *) );
  for (i = 0; i != num_out; ++i)
    pouts[i] = try_open(argv[i + out_argc], "wb");
  
  while (fgets(buf, sizeof(buf), pin)) {
    // find the delimiter
    for (delim = buf; !isspace(*delim); ++delim) {}
    // write it to the correct file
    fputs(buf, pouts[fnv_hash(buf, delim) % num_out]);
  }
  
  if (pin != stdin)
    fclose(pin);
    
  for (i = 0; i != num_out; ++i)
    fclose(pouts[i]);
  
  return 0;
}