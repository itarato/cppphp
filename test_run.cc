// c++ -std=c++11 test_run.cc -o test_run && ./test_run

#include <iostream>
#include <vector>

#include "test_ast.cc"
#include "test_case.cc"
#include "test_tokenizer.cc"

using namespace std;

int main() {
  vector<TestCase *> tests{
      new TokenizerTest(), new AstTest(),
  };

  for (const auto &t : tests) {
    t->run();
    t->report();
  }

  return 0;
}
