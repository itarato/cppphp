#ifndef CC_TOKEN_RULE
#define CC_TOKEN_RULE

#include <iostream>
#include <map>
#include "source_file_reader.cc"
#include "token.cc"
#include "types.cc"

using namespace std;

struct MaybeToken {
  bool is_token;
  union {
    Token token;
    bool error;
  };

  MaybeToken() : error(true), is_token(false) {};
  MaybeToken(Token t) : token(t), is_token(true) {};
  MaybeToken(const MaybeToken & mt) : is_token(mt.is_token) {
    if (is_token) {
      token = mt.token;
    } else {
      error = true;
    }
  };

  ~MaybeToken() {};
};

MaybeToken token_rule__php_open_tag(shared_ptr<SourceFileReader> reader) {
  if (reader->peek_n(5) == "<?php") {
    return MaybeToken({reader->get_n(5), TokenType::PHP_START});
  }

  return MaybeToken();
}

MaybeToken token_rule__variable(shared_ptr<SourceFileReader> reader) {
  if (reader->peek_n(1) == "$") {
    // @todo use the proper formula (cannot start with number, etc)
    return MaybeToken({
        reader->get_while(
            {{'$', '$'}, {'a', 'z'}, {'A', 'Z'}, {'0', '9'}, {'_', '_'}}),
        TokenType::VARIABLE});
  }
  return MaybeToken();
}

MaybeToken token_rule__keyword(shared_ptr<SourceFileReader> reader) {
  vector<string> keywords = {"__halt_compiler",
                             "abstract",
                             "and",
                             "array",
                             "as",
                             "break",
                             "callable",
                             "case",
                             "catch",
                             "class",
                             "clone",
                             "const",
                             "continue",
                             "declare",
                             "default",
                             "die",
                             "do",
                             "echo",
                             "else",
                             "elseif",
                             "empty",
                             "enddeclare",
                             "endfor",
                             "endforeach",
                             "endif",
                             "endswitch",
                             "endwhile",
                             "eval",
                             "exit",
                             "extends",
                             "final",
                             "finally",
                             "for",
                             "foreach",
                             "function",
                             "global",
                             "goto",
                             "if",
                             "implements",
                             "include",
                             "include_once",
                             "instanceof",
                             "insteadof",
                             "interface",
                             "isset",
                             "list",
                             "namespace",
                             "new",
                             "or",
                             "print",
                             "private",
                             "protected",
                             "public",
                             "require",
                             "require_once",
                             "return",
                             "static",
                             "switch",
                             "throw",
                             "trait",
                             "try",
                             "unset",
                             "use",
                             "var",
                             "while",
                             "xor",
                             "yield"};
  for (const auto& keyword : keywords) {
    if (reader->peek_n(keyword.size()) == keyword) {
      reader->get_n(keyword.size());
      return MaybeToken({keyword, TokenType::KEYWORD});
    }
  }
  return MaybeToken();
}

MaybeToken token_rule__string(shared_ptr<SourceFileReader> reader) {
  const char_type ch = reader->peek();
  if (ch == '"' || ch == '\'') {
    reader->get();
    const string s = reader->get_until(ch);
    reader->get();
    return MaybeToken({s, TokenType::STRING});
  }
  return MaybeToken();
}

MaybeToken token_rule__special_chars(shared_ptr<SourceFileReader> reader) {
  map<char_type, TokenType> special_chars = {
      {'(', TokenType::PARENTHESIS_OPEN}, {')', TokenType::PARENTHESIS_CLOSE},
      {'{', TokenType::BRACKET_OPEN},     {'}', TokenType::BRACKET_CLOSE},
      {';', TokenType::SEMICOLON},        {',', TokenType::COMMA},
      {'=', TokenType::OP_ASSIGNMENT},    {'+', TokenType::OP_PLUS},
      {'-', TokenType::OP_MINUS},         {'*', TokenType::OP_MULTIPLY},
      {'/', TokenType::OP_DIVIDE},        {'%', TokenType::OP_MODULO}};
  const char_type ch = reader->peek();
  for (auto it = special_chars.begin(); it != special_chars.end(); it++) {
    if (it->first == ch) {
      reader->get();
      return MaybeToken({{ch}, it->second});
    }
  }
  return MaybeToken();
}

MaybeToken token_rule__spec_name(shared_ptr<SourceFileReader> reader) {
  const char_type ch = reader->peek();
  if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) {
    return MaybeToken({
        reader->get_while({{'a', 'z'}, {'A', 'Z'}, {'0', '9'}, {'_', '_'}}),
        TokenType::SPEC_NAME});
  }
  return MaybeToken();
}

MaybeToken token_rule__numeric(shared_ptr<SourceFileReader> reader) {
  const char_type ch = reader->peek();
  if (ch >= '0' && ch <= '9') {
    return MaybeToken({reader->get_while({{'0', '9'}, {'.', '.'}, {'x', 'x'}}),
                 TokenType::NUMERIC});
  }
  return MaybeToken();
}

#endif