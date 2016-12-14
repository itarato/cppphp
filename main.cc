// c++ -std=c++11 main.cc -o main && ./main

#include <iostream>

#include "ast.cc"
#include "source_file_reader.cc"
#include "tokenizer.cc"
#include "types.cc"

using namespace std;

int main() {
  Tokenizer tkn(make_shared<SourceFileReader>("test_cases/simple.php"));
  AST ast(tkn.get_tokens());

  ast.debug();

  return 0;
}
