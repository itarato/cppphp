// c++ -std=c++11 main.cc -o main && ./main

#include <iostream>

#include "ast.cc"
#include "source_file_reader.cc"
#include "tokenizer.cc"
#include "types.cc"
#include "token.cc"

using namespace std;

int main() {
  Tokenizer tkn(make_shared<SourceFileReader>("test_cases/simple.php"));
  AST ast(tkn.get_tokens());

  auto tokens = tkn.get_tokens();
  copy(tokens.begin(), tokens.end(), ostream_iterator<Token>(cout, "\n"));
  ast.debug();

  return 0;
}
