#include "lexer.h"
#include "parser.h"

#include <cstdio>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
    return EXIT_FAILURE;
  }
  const auto file = argv[1];
  auto token_table = LexerParse(file);
  PrintTokenTable(token_table);
  auto context = Context{.token_table = token_table, .index = 0};
  // Print syntax tree in gdbgui
  auto expr = ParseArithmeticExpr(context);
  return 0;
}
