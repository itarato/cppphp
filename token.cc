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
 private:
  string orig;
  TokenType type;

 public:
  Token(string _orig, TokenType _type) : orig(_orig), type(_type){};
  friend ostream &operator<<(ostream &, const Token &);
};

ostream &operator<<(ostream &os, const Token &token) {
  os << "Token: " << token.orig;
  return os;
}

#endif