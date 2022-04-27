#include "lexer.h"

#include "fmt/core.h"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <numeric>
#include <stdexcept>
#include <string>
#include <unordered_map>

// -------------------------------------------------------------------------
// String Auxiliary
// -------------------------------------------------------------------------

std::string TokenType2String(TokenType token_type) {
  // TokenType's underlying type is int.
  // It is reasonable to convert TokenType to int.
  return std::to_string(static_cast<int>(token_type));
}

std::string TokenValue2String(const TokenValue &token_value) {
  switch (token_value.index()) {
  case 0: { // int
    return std::to_string(std::get<int>(token_value));
  }
  case 1: // string
    return std::get<std::string>(token_value);
  default: // unknown
    throw std::runtime_error("Invalid TokenValue Type");
    break;
  }
  return std::string{}; // Unreachable!!!
}

// NOTE: not implemented.
std::string TokenAttr2String([[maybe_unused]] const TokenAttr &token_attr) {
  return "";
}

std::string Token2String(const Token &token) {
  // (value, type, [attributes])
  return fmt::format("({}, {}, [{}])", TokenType2String(token.type_),
                     TokenValue2String(token.value_),
                     TokenAttr2String(token.attr_));
}

void PrintTokenTable(const TokenTable &token_table) {
  for (const auto &token : token_table) {
    fmt::print("{}\n", Token2String(token));
  }
}

// -------------------------------------------------------------------------
// Lexer Implementation
// -------------------------------------------------------------------------

bool IsKeyword(const std::string &word) {
  static std::unordered_map<std::string, TokenType> keywords{
      {"int", TokenType::kInt},      {"char", TokenType::kChar},
      {"if", TokenType::kIf},        {"else", TokenType::kElse},
      {"for", TokenType::kFor},      {"while", TokenType::kWhile},
      {"return", TokenType::kReturn}};
  return (keywords.find(word) != keywords.cend());
}

bool IsDelimiter(char c) {
  static std::unordered_map<char, TokenType> delimiters{
      {'(', TokenType::kOpenParen},  {')', TokenType::kCloseParen},
      {'[', TokenType::kOpenBacket}, {']', TokenType::kCloseBacket},
      {'{', TokenType::kOpenCurly},  {'}', TokenType::kCloseCurly},
      {',', TokenType::kComma},      {':', TokenType::kColon},
      {';', TokenType::kSemiColon}};
  return (delimiters.find(c) != delimiters.cend());
}

bool IsOperator(const std::string &word) {
  static std::unordered_map<std::string, TokenType> operators{
      {">", TokenType::kGreater},       {"<", TokenType::kLess},
      {">=", TokenType::kGreaterEqual}, {"<=", TokenType::kLessEqual},
      {"==", TokenType::kEqual},        {"!=", TokenType::kNotEqual},
      {"+", TokenType::kAdd},           {"-", TokenType::kSub},
      {"*", TokenType::kMULTIPLY},      {"/", TokenType::kDivide}};
  return (operators.find(word) != operators.cend());
}

void FatalError(const std::string &msg) {
  fmt::print(msg);
  exit(EXIT_FAILURE);
}

TokenType TokenId(const std::string &token) {
  static std::unordered_map<std::string, TokenType> token2type{
      {"int", TokenType::kInt},       {"char", TokenType::kChar},
      {"if", TokenType::kIf},         {"else", TokenType::kElse},
      {"for", TokenType::kFor},       {"while", TokenType::kWhile},
      {"return", TokenType::kReturn}, {"=", TokenType::kEqual},
      {"+", TokenType::kAdd},         {"-", TokenType::kSub},
      {"*", TokenType::kMULTIPLY},    {"/", TokenType::kDivide},
      {"(", TokenType::kOpenParen},   {")", TokenType::kCloseParen},
      {"[", TokenType::kOpenBacket},  {"]", TokenType::kCloseBacket},
      {"{", TokenType::kOpenCurly},   {"}", TokenType::kCloseCurly},
      {",", TokenType::kComma},       {":", TokenType::kColon},
      {";", TokenType::kSemiColon},   {">", TokenType::kGreater},
      {"<", TokenType::kLess},        {">=", TokenType::kGreaterEqual},
      {"<=", TokenType::kLessEqual},  {"==", TokenType::kEqual},
      {"!=", TokenType::kNotEqual},
  };
  auto type = token2type.find(token);
  return (type == token2type.cend()) ? TokenType::kInvalid : type->second;
}

TokenTable LexerParse(std::string source_file) {
  auto path = std::filesystem::path{source_file};
  if (!std::filesystem::exists(path) ||
      !std::filesystem::is_regular_file(path)) {
    throw std::runtime_error{fmt::format(
        "{0} don't exist or {0} is not a regular file.", path.string())};
  }

  auto input_file = std::ifstream{source_file};
  if (!input_file.is_open()) {
    throw std::runtime_error{fmt::format("Fail to open {}", path.string())};
  }
  auto source_code = std::string(std::istreambuf_iterator<char>{input_file},
                                 std::istreambuf_iterator<char>{});
  auto token_table = TokenTable{};

  for (auto index = source_code.cbegin(); index < source_code.cend();) {
    if (isdigit(*index)) { // Expect number
      auto start = index;
      auto end = std::find_if(index, source_code.cend(),
                           [](auto c) { return !isdigit(c); });
      // Process error
      if (end < source_code.cend()) {
        if (isalpha(*end)) { // Invalid token like 123abc
          FatalError(
              fmt::format("Invalid Identifier: {}", std::string{start, end}));
        }
      }
      // TODO: test number range
      token_table.emplace_back(TokenType::kIdentifier, std::string{start, end},
                               TokenAttr{});
      index = end;
    } else if (isalpha(*index)) { // Expect identifier or keyword
      auto end = std::find_if(index, source_code.cend(),
                           [](auto c) { return !isalnum(c); });
      auto token = std::string{index, end};

      if (IsKeyword(token)) { // Keyword
        token_table.emplace_back(TokenId(token), std::move(token), TokenAttr{});
      } else { // Identifier
        token_table.emplace_back(TokenType::kIdentifier, std::move(token),
                                 TokenAttr{});
      }
      index = end;
    } else if (*index == '"') { // Expect string
      auto end = std::find_if(index + 1, source_code.cend(),
                           [](auto c) { return c == '"'; });
      if (end == source_code.cend()) {
        FatalError("String is not close");
      }
      token_table.emplace_back(TokenType::kString, std::string{index + 1, end},
                               TokenAttr{});
      index = end + 1;
    } else if (isspace(*index)) { // Skip space
      index = std::find_if(index + 1, source_code.cend(),
                        [](auto c) { return !isspace(c); });
    } else {                     // Expect operators and delimiters
      if (IsDelimiter(*index)) { // Delimiters
        token_table.emplace_back(TokenId(std::string{*index}),
                                 std::string{*index}, TokenAttr{});
        ++index;
      } else { // Operators
        auto end = index + 1 + (*index == '>' || *index == '<');
        auto word = std::string{index, end};
        if (!IsOperator(word)) {
          FatalError(fmt::format("Unknown identifier/operator {}", word));
        }
        token_table.emplace_back(TokenId(word), std::move(word), TokenAttr{});
        index = end;
      }
    }
  }
  return token_table;
}
