#include <stdio.h>
#include <stdint.h>
#include <string.h>

int main(int argc, char* argv[]) {
  FILE *out_file = NULL;
  char file_name[64];
  int file_number = 0;
  int done = 0;
  int newline = 1;
  while (!done) {
    int c;
    if ((c = getchar()) != EOF) {
      if (c == '>' && newline) {
        if (out_file != NULL) {
          fclose(out_file);  
        }
        if (sprintf(file_name, "seq%d.fasta", file_number)) {
          out_file = fopen(file_name, "w");
          file_number += 1;
        } else {
          fprintf(stderr, "Problem forming filename.\n");
          return 1; 
        }
      }
      newline = (c == '\n');
      putc(c, out_file);
    } else {
      done = 1;
    }
  }
  fclose(out_file);
	return 0;
}

