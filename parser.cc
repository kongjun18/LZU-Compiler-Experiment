#include "parser.h"

#include "fmt/core.h"

#include <exception>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>

// -------------------------------------------------------------------------
// String Auxiliary
// -------------------------------------------------------------------------

// TODO: rewrite to be compatible with fmtlib
std::string Token2String(TokenType token_type) {
  // TokenType's underlying type is int.
  // It is reasonable to convert TokenType to int.
  return std::to_string(static_cast<int>(token_type));
}

// Pass token attribute by reference because it is big.
std::string TokenAttr2String(const TokenAttr &token_attr) {
  if (std::holds_alternative<int>(token_attr)) {
    return std::to_string(std::get<int>(token_attr));
  } else if (std::holds_alternative<std::string>(token_attr)) {
    return std::get<std::string>(token_attr);
  }
  throw std::runtime_error("Invalid TokenAttr Type");
  return std::string{}; // Unreachable!!!
}

std::string TokenPair2String(const TokenPair &token_pair) {
  return "(" + Token2String(token_pair.first) + ", " +
         TokenAttr2String(token_pair.second) + ")";
}

// -------------------------------------------------------------------------
// Parser Implementation
// -------------------------------------------------------------------------

TokenType TokenId(std::string token) {
  static std::unordered_map<std::string, TokenType> token2type{
      {"main", TokenType::kMain},    {"int", TokenType::kInt},
      {"char", TokenType::kChar},    {"if", TokenType::kIf},
      {"else", TokenType::kElse},    {"for", TokenType::kFor},
      {"while", TokenType::kWhile},  {"=", TokenType::kEqual},
      {"+", TokenType::kAdd},        {"-", TokenType::kSub},
      {"*", TokenType::kMULTIPLY},   {"/", TokenType::kDivide},
      {"(", TokenType::kOpenParen},  {")", TokenType::kCloseParen},
      {"[", TokenType::kOpenBacket}, {"]", TokenType::kCloseBacket},
      {"{", TokenType::kOpenCurly},  {"}", TokenType::kCloseCurly},
      {",", TokenType::kComma},      {":", TokenType::kColon},
      {";", TokenType::kSemiColon},  {">", TokenType::kGreater},
      {"<", TokenType::kLess},       {">=", TokenType::kGreaterEqual},
      {"<=", TokenType::kLessEqual}, {"==", TokenType::kEqual},
      {"!=", TokenType::kNotEqual},
  };
  auto type = token2type.find(token);
  return (type == token2type.cend()) ? TokenType::kInvalid : type->second;
}

TokenPairs LexerParse(std::string source_file) {
  auto path = std::filesystem::path{source_file};
  if (std::filesystem::exists(path) ||
      !std::filesystem::is_regular_file(path)) {
    throw std::runtime_error{
        fmt::format("{0} don't exist or {0} is not a regular file.", path)};
  }

  auto token_pairs = TokenPairs{};
  // TODO: parse source code
  return token_pairs;
}

void PrintTokenPairs(const TokenPairs &token_pairs) {
  for (const auto &pair : token_pairs) {
    std::cout << TokenPair2String(pair) << std::endl;
  }
}
