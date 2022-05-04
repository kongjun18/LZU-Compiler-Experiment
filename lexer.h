#pragma once

#include <string>
#include <utility>
#include <variant>
#include <vector>

enum class TokenType : int {
  kInvalid = 0,
  kInt,
  kChar,
  kString,
  kIf,
  kElse,
  kFor,
  kWhile,
  kReturn,
  kIdentifier,
  kNumber,
  kAssign,
  kAdd,
  kSub,
  kMULTIPLY,
  kDivide,
  kOpenParen,
  kCloseParen,
  kOpenBacket,
  kCloseBacket,
  kOpenCurly,
  kCloseCurly,
  kComma,
  kColon,
  kSemiColon,
  kGreater,
  kLess,
  kGreaterEqual,
  kLessEqual,
  kEqual,
  kNotEqual,
};

// NOTE: extend it in future.
struct TokenAttr {};

// using TokenValue = std::variant<int, std::string>;
using TokenValue = std::string;

struct Token {
  Token() = default;
  Token(TokenType type, TokenValue &&value, TokenAttr &&attr)
      : type_(type), value_(std::move(value)), attr_(std::move(attr)) {}
  TokenType type_;
  TokenValue value_;
  TokenAttr attr_;
};

using TokenTable = std::vector<Token>;

std::string TokenType2String(TokenType token_type);
std::string TokenValue2String(const TokenValue &token_value);
std::string TokenAttr2String([[maybe_unused]] const TokenAttr &token_attr);
void PrintTokenTable(const TokenTable &token_table);

TokenTable LexerParse(const std::string &source_file);
TokenType TokenId(const std::string &token);

bool IsOperator(const std::string &word);
bool IsDelimiter(char c);
bool IsKeyword(const std::string &word);

void FatalError(const std::string &msg);
