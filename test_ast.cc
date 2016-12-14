#pragma once

#include <string>

#include "ast.cc"
#include "source_file_reader.cc"
#include "test_case.cc"
#include "tokenizer.cc"

using namespace std;

class AstTest : public TestCase {
 private:
  void testAst() {
    Tokenizer tkn(
        make_shared<SourceFileReader>("test_cases/ast_simple_test.php"));
    AST ast(tkn.get_tokens());
  }

 public:
  virtual string getName() { return "AST"; };
  virtual void run() { testAst(); }
};
