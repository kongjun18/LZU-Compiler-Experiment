#include "parser.h"
#include "lexer.h"

#include <cctype>
#include <string>

// Forward declarations to meet dependencies.
static Factor *factor(Context &C);
static Expr *expr(Context &C);
static Expr1 *expr1(Context &C);
static Term *term(Context &C);
static Term1 *term1(Context &C);

static Factor *factor(Context &C) {
  const auto &t = C.GetTokenRef();
  switch (t.type_) {
  case TokenType::kIdentifier:
    return new Factor{.type_ = FactorType::kId, .id_ = t.value_};
  case TokenType::kNumber:
    return new Factor{.type_ = FactorType::kNumber,
                      .number_ = stoi(t.value_),
                      .expr_ = nullptr};
  case TokenType::kOpenParen: {
    auto f = new Factor{
        .type_ = FactorType::kExpr,
        .expr_ = expr(C),
    };
    const auto next_token = C.GetTokenRef();
    if (next_token.type_ != TokenType::kCloseParen) {
      FatalError("Unclosed (");
    }
    return f;
  }
  default:
    FatalError("Fail in factor()");
  }
  return nullptr;
}

static Term1 *term1(Context &C) {
  const auto &token = C.GetTokenRef();
  if (token.type_ == TokenType::kMULTIPLY ||
      token.type_ == TokenType::kDivide) {
    auto factor_ = factor(C);
    auto term1_ = term1(C);
    return new Term1{
        .type_ = Term1Type::kMulOp,
        .mulop_ = token.value_,
        .factor_ = factor_,
        .term1_ = term1_,
    };
  } else {
    C.RollBack();
    return new Term1{
        .type_ = Term1Type::kNull,
    };
  }

  return nullptr; // Unreachable!
}

static Expr1 *expr1(Context &C) {
  const auto &token = C.GetTokenRef();
  if (token.type_ == TokenType::kAdd || token.type_ == TokenType::kSub) {
    auto term_ = term(C);
    auto expr1_ = expr1(C);
    return new Expr1{
        .type_ = Expr1Type::kAddOp,
        .addop_ = token.value_,
        .term_ = term_,
        .expr1_ = expr1_,
    };
  } else {
    C.RollBack();
    return new Expr1{
        .type_ = Expr1Type::kNull,
    };
  }

  return nullptr; // Unreachable!
}

static Term *term(Context &C) {
  auto f = factor(C);
  auto t = term1(C);
  return new Term{f, t};
}

static Expr *expr(Context &C) {
  auto t = term(C);
  auto e = expr1(C);
  return new Expr{t, e};
}

// Arithmetic Expresion Generators:
// <Expr> → <Term> <Expr1>
// <Expr1> → <AddOp> <Term> <Expr1> | empty
// <Term> → <Factor> <Term1>
// <Term1> → <MulOp> <Factor> <Term1> | empty
// <Factor> → id |number | ( <Expr> )
// <AddOp> → + | -
// <MulOp> → * | /
Expr *ParseArithmeticExpr(Context &C) { return expr(C); }
