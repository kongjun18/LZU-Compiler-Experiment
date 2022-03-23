#include "lexer.h"

#include <cstdio>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
    return EXIT_FAILURE;
  }
  auto file = argv[1];
  auto token_table = LexerParse(file);
  PrintTokenTable(token_table);
  return 0;
}
