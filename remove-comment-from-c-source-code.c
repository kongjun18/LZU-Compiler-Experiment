/**
 * @file remove_comment_from_file.c
 * @brief Remove comment from C/C++ source code
 * @note Usage: remove_comment_from_file intput-file output-file
 * @author Kong Jun <kongjun18@outlook.com>
 * @copyright GLP-2.0 License
 */

#include <stdio.h>
#include <stdlib.h>
enum State {
  INITIAL,
  PERHAPS_COMMENT,
  SINGLE_LINE_COMMENT,
  MULTIPLE_LINE_COMMENT_BEGIN,
  MULTIPLE_LINE_COMMENT_END,
  CHAR,
  STRING,
  CHAR_ESCAPE,
  STRING_ESCAPE,
};

#define ERROR(state)                                                           \
  fprintf(stderr, "error in state %s\n", state);                               \
  return -1;

static int remove_comment_from_file(FILE *input, FILE *output) {
  int ch;
  enum State state = INITIAL;
  while ((ch = fgetc(input)) != EOF) {
    switch (state) {
    case INITIAL:
      if (ch == '/') {
        state = PERHAPS_COMMENT;
      } else if (ch == '\'') {
        state = CHAR;
        fputc(ch, output);
      } else if (ch == '\"') {
        state = STRING;
        fputc(ch, output);
      } else {
        fputc(ch, output);
      }
      break;
    case PERHAPS_COMMENT:
      if (ch == '/') { // //
        state = SINGLE_LINE_COMMENT;
      } else if (ch == '*') // /*
      {
        state = MULTIPLE_LINE_COMMENT_BEGIN;
      } else {
        ERROR("PERHAPS_COMMENT");
      }
      break;
    case SINGLE_LINE_COMMENT:
      if (ch != '\n') { // Don't consider line escape
        state = SINGLE_LINE_COMMENT;
      } else {
        state = INITIAL;
      }
      break;
    case MULTIPLE_LINE_COMMENT_BEGIN:
      if (ch == '*') { // /*...*
        state = MULTIPLE_LINE_COMMENT_END;
      }
      break;
    case MULTIPLE_LINE_COMMENT_END:
      if (ch == '/') { // /*...*/
        state = INITIAL;
      } else {
        state = MULTIPLE_LINE_COMMENT_BEGIN; // /*...*...
      }
      break;
    case CHAR:
      if (ch == '\'') { // '...'
        state = INITIAL;
      } else if (ch == '\\') { // escape character
        state = CHAR_ESCAPE;
      } else {
        state = INITIAL;
      }
      fputc(ch, output);
      break;
    case CHAR_ESCAPE:
      state = CHAR;
      fputc(ch, output);
      break;
    case STRING:
      if (ch == '"') {
        state = INITIAL;
      } else if (ch == '\\') { // escape character in string
        state = STRING_ESCAPE;
      } else {
        state = STRING;
      }
      fputc(ch, output);
      break;
    case STRING_ESCAPE:
      state = STRING;
      fputc(ch, output);
      break;
    default:
      ERROR("default");
    }
  }
  return 0;
}

int main(int argc, char **argv) {
  if (argc != 3) {
    fprintf(stderr, "USAGE: remove_comment_from_file input-file output-file\n");
    return -1;
  }

  FILE *input = fopen(argv[1], "rb");
  FILE *output = fopen(argv[2], "wb");
  int success = remove_comment_from_file(input, output);
  return success;
}
