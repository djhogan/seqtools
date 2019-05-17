#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define KMER_SIZE 4

int main(int argc, char *argv[]) {
	char buffer[1024];
  int readp = 0;
  int done = 0;
	int c;
  char kmer[KMER_SIZE+1];
  kmer[KMER_SIZE] = '\0';

  // fill buffer
  int bufp = 0;
  while (!done) {
    for (; bufp < 1024; bufp++) {
      if ((c = getchar()) != EOF) {
        buffer[bufp] = (char) c;
      } else {
        done = 1; 
        bufp -= 1;
        break;
      }
    }

    for (; readp < bufp-KMER_SIZE+1; readp++) {
      strncpy(kmer, buffer + readp, KMER_SIZE);
      printf("%s\n", kmer);
    }

    bufp = 0;
    for (; readp < bufp; readp++) {
      buffer[bufp] = buffer[readp];
    }
  }
	return 0;
}

