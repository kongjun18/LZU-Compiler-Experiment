#pragma once

#include "lexer.h"
#include <ctype.h>

// Forward declarations
struct Term;
struct Expr;
struct Factor;
struct Term1;
struct Expr1;

struct Context {
  const TokenTable token_table;
  mutable size_t index;

  const Token &GetTokenRef() const { return token_table[index++]; }
  void RollBack() const { --index; }
};

enum class FactorType : int {
  kInvalid = 0,
  kNumber,
  kId,
  kExpr,
};

enum class ExprType : int {
  kInvalid = 0,
  kTerm,
  kExpr,
};

enum class TermType : int {
  kInvalid = 0,
  kFactor,
  kTerm1,
};

enum class Expr1Type : int {
  kInvalid = 0,
  kAddOp,
  kNull,
};

enum class Term1Type : int {
  kInvalid = 0,
  kMulOp,
  kNull,
};

struct Term {
  Term(Factor *factor, Term1 *term1) : factor_{factor}, term1_{term1} {}
  Factor *factor_{nullptr};
  Term1 *term1_{nullptr};
};

struct Expr {
  Expr(Term *term, Expr1 *expr1) : term_{term}, expr1_{expr1} {}
  Term *term_{nullptr};
  Expr1 *expr1_{nullptr};
};

struct Factor {
  enum FactorType type_;
  std::string id_;
  int number_;
  Expr *expr_{nullptr};
};

struct Term1 {
  enum Term1Type type_;
  std::string mulop_;
  [[maybe_unused]] Factor *factor_{nullptr};
  [[maybe_unused]] Term1 *term1_{nullptr};
};

struct Expr1 {
  enum Expr1Type type_;
  std::string addop_;
  [[maybe_unused]] Term *term_{nullptr};
  [[maybe_unused]] Expr1 *expr1_{nullptr};
};

Expr *ParseArithmeticExpr(Context &C);
