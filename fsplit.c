#include <stdio.h>
#include <stdint.h>
#include <string.h>

const size_t bufsz = (1 << 20);

struct buffer {
  char data[bufsz];
  size_t sz, st, ed;
  char prev, cur;
  FILE *output;
};

int buf_test(struct buffer *buf) {
  return \
    (buf->prev == '\0' || buf->prev == '\n') & \
    (buf->cur == '>');
}

void buf_init(struct buffer *buf, FILE *output) {
  buf->sz = 0;
  buf->st = 0; 
  buf->ed = 0;
  buf->prev = '\0';
  buf->cur = '\0';
  buf->output = output;
}

void buf_fill(struct buffer *buf) {
  buf->sz = fread(buf->data, 1, bufsz, stdin); 
  buf->st = 0;
  buf->ed = 0;
}

void buf_flush(struct buffer *buf) {
  fwrite(buf->data + buf->st, 1, buf->ed - buf->st, buf->output);
  buf->st = buf->ed;
}

void buf_tail(struct buffer *buf) {
  fwrite(buf->data + buf->st, 1, buf->ed - buf->st - 1, buf->output);
  buf->st = buf->ed - 1;
}

int buf_eof(struct buffer *buf) {
  return buf->ed >= buf->sz && feof(stdin);
}

void buf_next(struct buffer *buf) {
  if (buf->ed < buf->sz) {
    buf->prev = buf->cur;
    buf->cur = buf->data[buf->ed++];
  } else {
    if (!feof(stdin)) {
      buf_flush(buf);
      buf_fill(buf);
      if (buf->sz > 0) {
        buf->prev = buf->cur;
        buf->cur = buf->data[buf->ed++];
      } 
    }
  }
}

FILE *next_file(int idx) {
  FILE *retval = NULL;
  char file_name[64];
  if (sprintf(file_name, "seq%d.fasta", idx)) {
    retval = fopen(file_name, "w");
  } 
  return retval;
}

int main(int argc, char* argv[]) {
  int fidx = 0;
  struct buffer buf;
  buf_init(&buf, next_file(fidx++));

  do {
    buf_next(&buf);
  }
  while (!(buf_eof(&buf) || buf_test(&buf)));

  if (buf_eof(&buf)) {
    return 0;
  } else {
    buf.st = buf.ed - 1;
  }

  do { // until end of file is reached
    buf_next(&buf);
    if (buf_test(&buf)) {
      buf_tail(&buf);
      buf.output = next_file(fidx++);
    }
  } while (!buf_eof(&buf));
  buf_flush(&buf);
	return 0;
}

