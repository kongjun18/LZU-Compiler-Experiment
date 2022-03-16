#pragma once

#include <vector>
#include <utility>
#include <string>
#include <variant>

enum class TokenType : int {
  kInvalid = 0,
  kMain,
  kInt,
  kChar,
  kIf,
  kElse,
  kFor,
  kWhile,
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

using TokenAttr = std::variant<int, std::string>;
using TokenPair = std::pair<TokenType, TokenAttr>;
using TokenPairs = std::vector<std::pair<TokenType, TokenAttr>>;
