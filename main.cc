#include <iostream>
#include "ast.cc"
#include "source_file_reader.cc"
#include "tokenizer.cc"
#include "types.cc"

using namespace std;

int main() {
  SourceFileReader sfr("simple.php");

  Tokenizer tkn(&sfr);
  tkn.run();

  AST ast(tkn.get_tokens());

  return 0;
}
