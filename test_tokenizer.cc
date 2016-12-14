#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "source_file_reader.cc"
#include "test_case.cc"
#include "tokenizer.cc"

using namespace std;

class TokenizerTest : public TestCase {
 private:
  void testTokens() {
    Tokenizer tkn(make_shared<SourceFileReader>("test_cases/token_test.php"));
    auto tokens = tkn.get_tokens();

    assert(tokens.size() == 20, "Token numbers", 20, tokens.size());

    TokenType expected[]{
        TokenType::PHP_START,

        TokenType::VARIABLE,
        TokenType::OP_ASSIGNMENT,
        TokenType::NUMERIC,
        TokenType::SEMICOLON,

        TokenType::KEYWORD,
        TokenType::SPEC_NAME,
        TokenType::PARENTHESIS_OPEN,
        TokenType::PARENTHESIS_CLOSE,
        TokenType::BRACKET_OPEN,
        TokenType::BRACKET_CLOSE,

        TokenType::KEYWORD,
        TokenType::SPEC_NAME,
        TokenType::PARENTHESIS_OPEN,
        TokenType::VARIABLE,
        TokenType::COMMA,
        TokenType::VARIABLE,
        TokenType::PARENTHESIS_CLOSE,
        TokenType::BRACKET_OPEN,
        TokenType::BRACKET_CLOSE,
    };

    for (int i = 0; i < sizeof(expected) / sizeof(TokenType); i++) {
      assert(tokens[i].type == expected[i], "Token type", expected[i],
             tokens[i].type);
    }
  }

 public:
  virtual string getName() { return "Tokenizer"; };
  virtual void run() { testTokens(); }
};