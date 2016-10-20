#ifndef CC_TOKEN
#define CC_TOKEN

#include <iostream>
#include <string>

using namespace std;

enum TokenType {
  PHP_START,
  VARIABLE,
  KEYWORD,
  STRING,
  PARENTHESIS_OPEN,
  PARENTHESIS_CLOSE,
  BRACKET_OPEN,
  BRACKET_CLOSE,
  SEMICOLON,
  COMMA,
  OP_ASSIGNMENT,
  OP_PLUS,
  OP_MINUS,
  OP_MULTIPLY,
  OP_DIVIDE,
  OP_MODULO,
  SPEC_NAME,
  NUMERIC
};

class Token {
 public:
  string orig;
  TokenType type;
  Token(){};
  Token(string _orig, TokenType _type) : orig(_orig), type(_type){};
  friend ostream &operator<<(ostream &, const Token &);
};

ostream &operator<<(ostream &os, const Token &token) {
  string type;
  if (token.type == TokenType::PHP_START) type = "PHP_START";
  if (token.type == TokenType::VARIABLE) type = "VARIABLE";
  if (token.type == TokenType::KEYWORD) type = "KEYWORD";
  if (token.type == TokenType::STRING) type = "STRING";
  if (token.type == TokenType::PARENTHESIS_OPEN) type = "PARENTHESIS_OPEN";
  if (token.type == TokenType::PARENTHESIS_CLOSE) type = "PARENTHESIS_CLOSE";
  if (token.type == TokenType::BRACKET_OPEN) type = "BRACKET_OPEN";
  if (token.type == TokenType::BRACKET_CLOSE) type = "BRACKET_CLOSE";
  if (token.type == TokenType::SEMICOLON) type = "SEMICOLON";
  if (token.type == TokenType::COMMA) type = "COMMA";
  if (token.type == TokenType::OP_ASSIGNMENT) type = "OP_ASSIGNMENT";
  if (token.type == TokenType::OP_PLUS) type = "OP_PLUS";
  if (token.type == TokenType::OP_MINUS) type = "OP_MINUS";
  if (token.type == TokenType::OP_MULTIPLY) type = "OP_MULTIPLY";
  if (token.type == TokenType::OP_DIVIDE) type = "OP_DIVIDE";
  if (token.type == TokenType::OP_MODULO) type = "OP_MODULO";
  if (token.type == TokenType::SPEC_NAME) type = "SPEC_NAME";
  if (token.type == TokenType::NUMERIC) type = "NUMERIC";
  os << "T(" << type << "=" << (token.orig.size() ? token.orig : "?") << ")";
  return os;
}

Token string_to_token(string type, string value) {
  if (type == "PHP_START") {
    return Token(value, TokenType::PHP_START);
  } else if (type == "VARIABLE") {
    return Token(value, TokenType::VARIABLE);
  } else if (type == "KEYWORD") {
    return Token(value, TokenType::KEYWORD);
  } else if (type == "STRING") {
    return Token(value, TokenType::STRING);
  } else if (type == "PARENTHESIS_OPEN") {
    return Token(value, TokenType::PARENTHESIS_OPEN);
  } else if (type == "PARENTHESIS_CLOSE") {
    return Token(value, TokenType::PARENTHESIS_CLOSE);
  } else if (type == "BRACKET_OPEN") {
    return Token(value, TokenType::BRACKET_OPEN);
  } else if (type == "BRACKET_CLOSE") {
    return Token(value, TokenType::BRACKET_CLOSE);
  } else if (type == "SEMICOLON") {
    return Token(value, TokenType::SEMICOLON);
  } else if (type == "COMMA") {
    return Token(value, TokenType::COMMA);
  } else if (type == "OP_ASSIGNMENT") {
    return Token(value, TokenType::OP_ASSIGNMENT);
  } else if (type == "OP_PLUS") {
    return Token(value, TokenType::OP_PLUS);
  } else if (type == "OP_MINUS") {
    return Token(value, TokenType::OP_MINUS);
  } else if (type == "OP_MULTIPLY") {
    return Token(value, TokenType::OP_MULTIPLY);
  } else if (type == "OP_DIVIDE") {
    return Token(value, TokenType::OP_DIVIDE);
  } else if (type == "OP_MODULO") {
    return Token(value, TokenType::OP_MODULO);
  } else if (type == "SPEC_NAME") {
    return Token(value, TokenType::SPEC_NAME);
  } else if (type == "NUMERIC") {
    return Token(value, TokenType::NUMERIC);
  }

  throw "Invalid type";
}

#endif