#ifndef CC_TOKEN_RULE
#define CC_TOKEN_RULE

#include <iostream>
#include <map>
#include "source_file_reader.cc"
#include "types.cc"

using namespace std;

Token* token_rule__php_open_tag(SourceFileReader* reader) {
  if (reader->peek_n(5) == "<?php") {
    return new Token(reader->get_n(5), TokenType::PHP_START);
  }

  return nullptr;
}

Token* token_rule__variable(SourceFileReader* reader) {
  if (reader->peek_n(1) == "$") {
    // @todo use the proper formula (cannot start with number, etc)
    return new Token(
        reader->get_while(
            {{'$', '$'}, {'a', 'z'}, {'A', 'Z'}, {'0', '9'}, {'_', '_'}}),
        TokenType::VARIABLE);
  }
  return nullptr;
}

Token* token_rule__keyword(SourceFileReader* reader) {
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
      return new Token(keyword, TokenType::KEYWORD);
    }
  }
  return nullptr;
}

Token* token_rule__string(SourceFileReader* reader) {
  const char_type ch = reader->peek();
  if (ch == '"' || ch == '\'') {
    reader->get();
    const string s = reader->get_until(ch);
    reader->get();
    return new Token(s, TokenType::STRING);
  }
  return nullptr;
}

Token* token_rule__special_chars(SourceFileReader* reader) {
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
      return new Token({ch}, it->second);
    }
  }
  return nullptr;
}

Token* token_rule__spec_name(SourceFileReader* reader) {
  const char_type ch = reader->peek();
  if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) {
    return new Token(
        reader->get_while({{'a', 'z'}, {'A', 'Z'}, {'0', '9'}, {'_', '_'}}),
        TokenType::SPEC_NAME);
  }
  return nullptr;
}

Token* token_rule__numeric(SourceFileReader* reader) {
  const char_type ch = reader->peek();
  if (ch >= '0' && ch <= '9') {
    return new Token(reader->get_while({{'0', '9'}, {'.', '.'}, {'x', 'x'}}),
                     TokenType::NUMERIC);
  }
  return nullptr;
}

#endif